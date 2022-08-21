/**
 * @file fifo.h
 * @author YYF (NONE)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>

/**
 * @brief 该类型为fifo的buffer缓存的类型，由用户自定义
 * 
 */
typedef uint16_t fifo_buffer_type;

#define FIFO_BUFFER_SIZE    0x20UL

typedef struct _FIFO_
{
    /* data */
    fifo_buffer_type buffer[FIFO_BUFFER_SIZE];
    /* 直接向变量的内存地址取值 */
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t size;
}FIFO;

void fifo_init(FIFO *p_fifo);
void fifo_clear(FIFO *p_fifo);
uint16_t fifo_push_bytes(FIFO *p_fifo, const fifo_buffer_type *src, uint16_t len);
uint16_t fifo_pop_bytes(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len);
uint16_t fifo_pop_bytes_clear(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len);

void fifo_push_byte(FIFO *p_fifo, const fifo_buffer_type src);
uint8_t fifo_pop_byte(FIFO *p_fifo, fifo_buffer_type *dest);
uint8_t fifo_pop_byte_clear(FIFO *p_fifo, fifo_buffer_type *dest);

#endif/* __PUB_MACRO_H__ */
