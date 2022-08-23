#ifndef __PUBLIC_H__
#define __PUBLIC_H__
#include <stdint.h>
#include "fifo.h"
#include "pub_memory.h"
#include "pub_macro.h"
#include "log_print.h"
#include "memory_operate.h"

#define VA_SPRINTF_BUFFER       512












uint32_t pub_hex2bcd(uint32_t value, uint8_t size);
uint32_t pub_bcd2hex(uint32_t value);
char * pub_sprintf(char *format, ...);

#endif
