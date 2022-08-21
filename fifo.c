/**
 * @file fifo.c
 * @author YYF (NONE)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "fifo.h"

void fifo_init(FIFO *p_fifo)
{
    fifo_clear(p_fifo);
}

void fifo_clear(FIFO *p_fifo)
{
    p_fifo->tail = p_fifo->head = 0;
    p_fifo->size = 0;
}

/**
 * @brief 压入数据至fifo缓冲区内，头追上尾会进行覆盖，head指向的为最新数据
 *                                                 tail指向的为最老数据
 * @param p_fifo FIFO指针
 * @param src 数据源
 * @param len 数据源长度
 * @return uint16_t 实际压入的数据长度
 */
uint16_t fifo_push_bytes(FIFO *p_fifo, const fifo_buffer_type *src, uint16_t len)
{
    uint16_t i, j, k;
    
    if(0 == p_fifo)
        return 0;

    j = p_fifo->head + len;
    for ( i = p_fifo->head, k = 0; i < j; i++, k++)
    {
        /*自动覆盖*/
        p_fifo->buffer[i%FIFO_BUFFER_SIZE] = *(src + k);
    }
    p_fifo->size += len;
    /* 满了 */
    if (p_fifo->size > FIFO_BUFFER_SIZE)
    {
        p_fifo->head = i % FIFO_BUFFER_SIZE;
        p_fifo->tail = i % FIFO_BUFFER_SIZE;
        p_fifo->size = FIFO_BUFFER_SIZE;
    }
    else
    {
        p_fifo->head = i % FIFO_BUFFER_SIZE;
    }
    
    return len;
}

/**
 * @brief 弹出数据至dest内，不进行清除
 * 
 * @param p_fifo FIFO指针
 * @param dest 目标源
 * @param len 需要弹出的数据长度
 * @return uint16_t 实际
 */
uint16_t fifo_pop_bytes(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len)
{
    uint16_t i,cnt;

    if(0 == p_fifo)
        return 0;
    cnt = p_fifo->tail;
    for ( i = 0; i < len; i++)
    {
        /* 满了,没有数据了 */
        if(0 == p_fifo->size)
            return i;
        if(cnt >= FIFO_BUFFER_SIZE)
            cnt = 0;
        *(dest + i) = p_fifo->buffer[cnt++];
    }
    return len;
}

uint16_t fifo_pop_bytes_clear(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len)
{
    uint16_t i;

    if(0 == p_fifo)
        return 0;

    for ( i = 0; i < len; i++)
    {
        /* 满了,没有数据 */
        if(0 == p_fifo->size)
            return i;
        if(p_fifo->tail >= FIFO_BUFFER_SIZE)
            p_fifo->tail = 0;
        *(dest + i) = p_fifo->buffer[p_fifo->tail++];
    }
    p_fifo->size -= i;
    return len;
}


void fifo_push_byte(FIFO *p_fifo, const fifo_buffer_type src)
{
    if(0 == p_fifo)
        return;

    if (p_fifo->size > FIFO_BUFFER_SIZE)
    {
        p_fifo->buffer[p_fifo->head] = src;
        p_fifo->head = (p_fifo->head + 1) % FIFO_BUFFER_SIZE;
        p_fifo->tail = p_fifo->head;
    }
    else
    {
        p_fifo->buffer[p_fifo->head] = src;
        p_fifo->head = (p_fifo->head + 1) % FIFO_BUFFER_SIZE;
        p_fifo->size++;
    }
}

/**
 * @brief 弹出一个字节
 * 
 * @param p_fifo FIFO指针
 * @param dest 目标源
 * @return uint8_t 0成功 1失败
 */
uint8_t fifo_pop_byte(FIFO *p_fifo, fifo_buffer_type *dest)
{
    if(0 == p_fifo)
        return 0;
    /* 无数据 */
    if(0 == p_fifo->size)
        return 1;
    *dest = p_fifo->buffer[p_fifo->tail];
    return 0;
}

uint8_t fifo_pop_byte_clear(FIFO *p_fifo, fifo_buffer_type *dest)
{
    if(0 == p_fifo)
        return 0;
    /* 无数据 */
    if(0 == p_fifo->size)
        return 1;
    *dest = p_fifo->buffer[p_fifo->tail];
    p_fifo->tail = (p_fifo->tail + 1) % FIFO_BUFFER_SIZE;
  
    return 0;
}