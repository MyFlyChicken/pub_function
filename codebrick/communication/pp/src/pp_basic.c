/**
 * @file pp_basic.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2025-09-09
 * 
 * @copyright Copyright (c) 2025 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2025-09-09 <td>v1.0     <td>Yu     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "../inc/pp_basic.h"
#include <stdint.h>

static void pp_basic_dtor(struct pp_basic* const me);
static void pp_basic_poll(struct pp_basic* const me);

static void pp_basic_append_str(uint8_t** frame, const char* str, uint32_t actual_len, uint32_t demand_len);

static uint8_t  pp_basic_crc8(uint8_t init_value, const uint8_t* data, uint32_t len);
static uint16_t pp_basic_crc16(uint16_t init_value, const uint8_t* data, uint32_t len);
static uint32_t pp_basic_crc32(uint32_t init_value, const uint8_t* data, uint32_t len);

const struct ppVtbl pp_basic_vtbl = {
    /* virtual functions */
    /* .dtor       = */ pp_basic_dtor,
    /* .push_data  = */ pp_basic_poll,
    /* .append_str = */ pp_basic_append_str,
    /* .crc8      = */ pp_basic_crc8,
    /* .crc16     = */ pp_basic_crc16,
    /* .crc32     = */ pp_basic_crc32,
};

static void pp_basic_append_str(uint8_t** frame, const char* str, uint32_t actual_len, uint32_t demand_len)
{
    (void)frame;
    (void)str;
    (void)actual_len;
    (void)demand_len;

    return;
}

static void pp_basic_dtor(struct pp_basic* const me)
{
    (void)me;
    return;
}

static void pp_basic_poll(struct pp_basic* const me)
{
    (void)me;
    return;
}

static uint8_t pp_basic_crc8(uint8_t init_value, const uint8_t* data, uint32_t len)
{
    (void)init_value;
    (void)data;
    (void)len;

    return 0;
}

static uint16_t pp_basic_crc16(uint16_t init_value, const uint8_t* data, uint32_t len)
{
    (void)init_value;
    (void)data;
    (void)len;

    return 0;
}

static uint32_t pp_basic_crc32(uint32_t init_value, const uint8_t* data, uint32_t len)
{
    (void)init_value;
    (void)data;
    (void)len;

    return 0;
}

void pp_basic_ctor(struct pp_basic* const me, uint8_t* rb_pool, uint32_t rb_size, uint8_t* fb_pool, uint32_t fb_size, ringbuffer_lock lock, ringbuffer_unlock unlock)
{
    me->vptr              = &pp_basic_vtbl;
    me->rb_pool           = rb_pool;
    me->fb_pool           = fb_pool;
    me->fb_size           = fb_size;
    me->ringbuffer_lock   = lock;
    me->ringbuffer_unlock = unlock;

    ringbuffer_init(&me->rb, rb_pool, rb_size);
}

uint32_t pp_basic_push_data(struct pp_basic* const me, const uint8_t* data, uint32_t len)
{
    PP_ASSERT(me && data && (len > 0));

    me->ringbuffer_lock ? me->ringbuffer_lock() : (void)0;
    uint32_t rel = ringbuffer_put(&me->rb, data, len);
    me->ringbuffer_unlock ? me->ringbuffer_unlock() : (void)0;

    if (rel == 0)
    {
        return 0;
    }

    return rel;
}
