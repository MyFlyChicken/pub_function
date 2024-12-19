/**
 * @file memory_operate.h 内存读取，支持flash读取和RAM读写，
 * @author YYF (NONE)
 * @brief 
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __MEMORY_OPERATE_H__
#define __MEMORY_OPERATE_H__

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

#define MEMORY_WR_ADDR_GROUPS   1
extern const uint32_t c_memory_wr_groups[MEMORY_WR_ADDR_GROUPS][2];

#define MEMORY_RO_ADDR_GROUPS   1
extern const uint32_t c_memory_ro_groups[MEMORY_RO_ADDR_GROUPS][2];

typedef void (* memory_operate_callback)(void);
typedef int (* memory_operate_print)(const char * format, ...);

void memory_operate_init(memory_operate_callback __callback, memory_operate_print __print);

uint8_t memory_write_byte(uint8_t *dest, const uint8_t *src, uint16_t length);
uint8_t memory_write_halfword(uint16_t *dest, const uint16_t *src, uint16_t length);
uint8_t memory_write_word(uint32_t *dest, const uint32_t *src, uint16_t length);

uint8_t memory_read_byte(uint8_t *dest, const uint8_t *src, uint16_t length);
uint8_t memory_read_halfword(uint16_t *dest, const uint16_t *src, uint16_t length);
uint8_t memory_read_word(uint32_t *dest, const uint32_t *src, uint16_t length);

#endif