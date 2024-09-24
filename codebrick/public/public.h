#ifndef __PUBLIC_H__
#define __PUBLIC_H__
#include <stdint.h>
#include "pub_memory.h"
#include "pub_macro.h"
#include "../mf_log.h"
#include "../memory_operate.h"

#define VA_SPRINTF_BUFFER 512

#define TIME_COUNTER_YEAR_EN 1
typedef union __TIME_COUNTER__ {
    uint32_t count;
    struct
    {
        uint32_t second : 6;
        uint32_t minute : 6;
        uint32_t hour   : 5;
#if TIME_COUNTER_YEAR_EN
        uint32_t day  : 9;
        uint32_t year : 6;
#else
        uint32_t day : 15;
#endif
    } mid;
} TIME_COUNTER;

uint32_t pub_bcd2dec(uint32_t value, uint8_t size);
uint32_t pub_dec2bcd(uint32_t value);
char*    pub_sprintf(char* format, ...);
void     pub_time_counte(TIME_COUNTER* p_time_count);
#endif
