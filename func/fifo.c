/**
  ******************************************************************************
  * @file    fifo.c
  * @author  YYF
  * @brief   注意，fifo满了会进行数据覆盖
  *          
  ******************************************************************************
  */
#include "fifo.h"

void fifo_init(FIFO *p_fifo, fifo_buffer_type *p_buff, uint16_t numbers)
{
	p_fifo->numbers = numbers;
	p_fifo->pbuffer = p_buff;
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

	if (0 == p_fifo)
		return 0;

	j = p_fifo->head + len;
	for (i = p_fifo->head, k = 0; i < j; i++, k++)
	{
		/*自动覆盖*/
		*(p_fifo->pbuffer + (i % p_fifo->numbers)) = *(src + k);
	}
	p_fifo->size += len;
	/* 满了 */
	if (p_fifo->size > p_fifo->numbers)
	{
		p_fifo->head = i % p_fifo->numbers;
		p_fifo->tail = i % p_fifo->numbers;
		p_fifo->size = p_fifo->numbers;
	}
	else
	{
		p_fifo->head = i % p_fifo->numbers;
	}

	return len;
}

/**
 * @brief 弹出数据至dest内，不进行清除
 *
 * @param p_fifo FIFO指针
 * @param dest 目标源
 * @param len 实际弹出的数据长度
 * @return uint16_t 实际
 */
uint16_t fifo_pop_bytes(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len)
{
	uint16_t i, cnt;

	if (0 == p_fifo)
		return 0;
	if (0 == p_fifo->size)
		return 0;
	cnt = p_fifo->tail;
	for (i = 0; i < len; i++)
	{
		cnt %= p_fifo->numbers;
		*(dest + i) = *(p_fifo->pbuffer + cnt++);
	}
	return len;
}

/**
 * @brief 弹出多个fifo缓存数据,并清除数据
 *
 * @param p_fifo 数据指针
 * @param dest 目标
 * @param len 长度
 * @return uint16_t 实际弹出的数据长度
 */
uint16_t fifo_pop_bytes_clear(FIFO *p_fifo, fifo_buffer_type *dest, uint16_t len)
{
	uint16_t i;

	if (0 == p_fifo)
		return 0;
	if (0 == p_fifo->size)
		return 0;
	for (i = 0; i < len; i++)
	{
		p_fifo->tail %= p_fifo->numbers;
		*(dest + i) = *(p_fifo->pbuffer + p_fifo->tail++);
		p_fifo->size--;
		if (0 == p_fifo->size)
			return (i + 1);
	}
	return len;
}


void fifo_push_byte(FIFO *p_fifo, const fifo_buffer_type src)
{
	if (0 == p_fifo)
		return;

	if (p_fifo->size >= p_fifo->numbers)
	{
		*(p_fifo->pbuffer + p_fifo->head) = src;
		p_fifo->head = (p_fifo->head + 1) % p_fifo->numbers;
		p_fifo->tail = p_fifo->head;
		p_fifo->size = p_fifo->numbers;
	}
	else
	{
		*(p_fifo->pbuffer + p_fifo->head) = src;
		p_fifo->head = (p_fifo->head + 1) % p_fifo->numbers;
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
	if (0 == p_fifo)
		return 1;
	/* 无数据 */
	if (0 == p_fifo->size)
		return 1;
	*dest = *(p_fifo->pbuffer + p_fifo->tail);
	return 0;
}

uint8_t fifo_pop_byte_clear(FIFO *p_fifo, fifo_buffer_type *dest)
{
	if (0 == p_fifo)
		return 1;
	/* 无数据 */
	if (0 == p_fifo->size)
		return 1;
	*dest = *(p_fifo->pbuffer + p_fifo->tail);
	p_fifo->tail = (p_fifo->tail + 1) % p_fifo->numbers;
	p_fifo->size--;
	return 0;
}
