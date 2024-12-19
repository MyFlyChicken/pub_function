/**
 * @file pp_ringbuffer.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-12-11 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */

#include "../inc/pp_ringbuffer.h"
#include <string.h>

#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))

uint32_t ringbuffer_get_size(ringbuffer_t* rb)
{
    return rb->buffer_size;
}

ringbuffer_state_e ringbuffer_status(ringbuffer_t* rb)
{
    if (rb->read_index == rb->write_index) {
        if (rb->read_mirror == rb->write_mirror) {
            return RINGBUFFER_EMPTY;
        }
        else {
            return RINGBUFFER_FULL;
        }
    }

    return RINGBUFFER_HALFFULL;
}

void ringbuffer_init(ringbuffer_t* rb, uint8_t* pool, int64_t size)
{
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    rb->buffer_ptr  = pool;
    rb->buffer_size = size;
}

uint32_t ringbuffer_put(ringbuffer_t* rb, const uint8_t* ptr, uint32_t length)
{
    uint32_t size;

    size = ringbuffer_space_len(rb);

    if (size == 0) {
        return 0;
    }

    if (size < length) {
        length = size;
    }

    if (rb->buffer_size - rb->write_index > length) {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);

        rb->write_index += length;

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index], &ptr[0], rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index], length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index  = length - (rb->buffer_size - rb->write_index);

    return length;
}

uint32_t ringbuffer_put_force(ringbuffer_t* rb, const uint8_t* ptr, uint32_t length)
{
    uint32_t space_length;

    space_length = ringbuffer_space_len(rb);

    if (length > rb->buffer_size) {
        ptr    = &ptr[length - rb->buffer_size];
        length = rb->buffer_size;
    }

    if (rb->buffer_size - rb->write_index > length) {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);

        rb->write_index += length;

        if (length > space_length) {
            rb->read_index = rb->write_index;
        }

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index], &ptr[0], rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index], length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index  = length - (rb->buffer_size - rb->write_index);

    if (length > space_length) {
        if (rb->write_index <= rb->read_index) {
            rb->read_mirror = ~rb->read_mirror;
        }

        rb->read_index = rb->write_index;
    }

    return length;
}

uint32_t ringbuffer_get(ringbuffer_t* rb, uint8_t* ptr, uint32_t length)
{
    uint32_t size;

    /* whether has enough data  */
    size = ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    /* less data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->read_index > length) {
        /* copy all of data */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);

        rb->read_index += length;

        return length;
    }

    memcpy(&ptr[0], &rb->buffer_ptr[rb->read_index], rb->buffer_size - rb->read_index);
    memcpy(&ptr[rb->buffer_size - rb->read_index], &rb->buffer_ptr[0], length - (rb->buffer_size - rb->read_index));

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index  = length - (rb->buffer_size - rb->read_index);

    return length;
}

uint32_t ringbuffer_peek(ringbuffer_t* rb, uint8_t** ptr)
{
    uint32_t size;

    *ptr = NULL;

    /* whether has enough data  */
    size = ringbuffer_data_len(rb);

    /* no data */
    if (size == 0) {
        return 0;
    }

    *ptr = &rb->buffer_ptr[rb->read_index];

    if ((uint32_t)(rb->buffer_size - rb->read_index) > size) {
        rb->read_index += size;

        return size;
    }

    size = rb->buffer_size - rb->read_index;

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index  = 0;

    return size;
}

uint32_t ringbuffer_putchar(ringbuffer_t* rb, const uint8_t ch)
{
    /* whether has enough space */
    if (!ringbuffer_space_len(rb)) {
        return 0;
    }

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size - 1) {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index  = 0;
    }
    else {
        rb->write_index++;
    }

    return 1;
}

uint32_t ringbuffer_putchar_force(ringbuffer_t* rb, const uint8_t ch)
{
    ringbuffer_state_e old_state;

    old_state = ringbuffer_status(rb);

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size - 1) {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index  = 0;
        if (old_state == RINGBUFFER_FULL) {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index  = rb->write_index;
        }
    }
    else {
        rb->write_index++;
        if (old_state == RINGBUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}

uint32_t ringbuffer_getchar(ringbuffer_t* rb, uint8_t* ch)
{
    /* ringbuffer is empty */
    if (!ringbuffer_data_len(rb)) {
        return 0;
    }

    /* put byte */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size - 1) {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index  = 0;
    }
    else {
        rb->read_index++;
    }

    return 1;
}

uint32_t ringbuffer_data_len(ringbuffer_t* rb)
{
    switch (ringbuffer_status(rb)) {
    case RINGBUFFER_EMPTY:
        return 0;
    case RINGBUFFER_FULL:
        return rb->buffer_size;
    case RINGBUFFER_HALFFULL:
    default: {
        uint32_t wi = rb->write_index, ri = rb->read_index;

        if (wi > ri) {
            return wi - ri;
        }
        else {
            return rb->buffer_size - (ri - wi);
        }
    }
    }
}

void ringbuffer_reset(ringbuffer_t* rb)
{
    rb->read_mirror  = 0;
    rb->read_index   = 0;
    rb->write_mirror = 0;
    rb->write_index  = 0;
}
