/**
  ******************************************************************************
  * @file    fifo.h
  * @author  YYF
  * @brief   无
  *          
  ******************************************************************************
  */


#ifndef __FIFO_H__
#define __FIFO_H__

/*************************************************************
 *                      头文件                                *
 ************************************************************/
#include <stdint.h>

/*************************************************************
 *                        宏                                 *
 ************************************************************/
#define FIFO_BUFFER_SIZE    0x20UL

/*************************************************************
 *                        常量                               *
 ************************************************************/

/*************************************************************
 *                      数据类型                              *
 ************************************************************/
/**
 * @brief 该类型为fifo的buffer缓存的类型，由用户自定义
 *
 */
typedef uint8_t fifo_buffer_type;

typedef struct _FIFO_
{
	/* data */
	fifo_buffer_type buffer[FIFO_BUFFER_SIZE];
	/* 直接向变量的内存地址取值 */
	volatile uint16_t head;
	volatile uint16_t tail;
	volatile uint16_t size;
} FIFO;
/*************************************************************
 *                       变量                                *
 ************************************************************/

/*************************************************************
 *                       函数原型                            *
 ************************************************************/
void fifo_init(FIFO *p_fifo);
void fifo_clear(FIFO *p_fifo);
uint16_t fifo_push_bytes(FIFO *p_fifo, const fifo_buffer_type *src, uint16_t len);
uint16_t fifo_pop_bytes(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len);
uint16_t fifo_pop_bytes_clear(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len);

void fifo_push_byte(FIFO *p_fifo, const fifo_buffer_type src);
uint8_t fifo_pop_byte(FIFO *p_fifo, fifo_buffer_type *dest);
uint8_t fifo_pop_byte_clear(FIFO *p_fifo, fifo_buffer_type *dest);

#endif/* __PUB_MACRO_H__ */
