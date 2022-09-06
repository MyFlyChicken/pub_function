/**
 * @file public.c
 * @author YYF (NONE)
 * @brief 公用文件
 * @version 0.1
 * @date 2022-08-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "public.h"
#include <stdarg.h>
#include <stdio.h>
/**
 * @brief 十六进制BCD码转10进制
 * 
 * @param value 输入参数
 * @param size 参数字节数
 * @return uint32_t 
 */
uint32_t pub_hex2bcd(uint32_t value, uint8_t size)
{
    int8_t i, j;
    uint32_t tmp;
    uint32_t res;
    uint32_t bit = 0x0000000f;

    res = 0;
    size <<= 1;/* 4bit表示一个bcd码 */
    res += (value & bit);
    for (i = 1; i < size; i++)
    {
        bit <<= 4;
        tmp = (value & bit);
        if (tmp)
        {
            tmp >>= (i * 4);
        }
        else
        {
            continue;
        }
        for (j = 0; j < i; j++)
        {
            tmp *= 10;
        }
        res += tmp;  
    }
    return res;
}

/**
 * @brief 十进制转十六进制BCD码
 * 
 * @param value 
 * @return uint32_t 
 */
uint32_t pub_bcd2hex(uint32_t value)
{
    uint8_t i = 0;
    uint32_t mid, res;

    /* uint32_t最多表示八个位的十六进制数据 */
    if (value > 99999999)
        return 0;
    res = value%10;
    do
    {
        mid = value%10;
        res |= (mid << (i << 2));
        value /= 10;
        i++;
    } while (value);

    return res;
}

/**
 * @brief 格式化字符串，类似printf功能
 * 
 * @param format 
 * @param ... 
 * @return char* 
 */
char * pub_sprintf(char *format, ...)
{
    static char dest[VA_SPRINTF_BUFFER];
    va_list args;

    va_start (args, format);
    if (-1 == vsnprintf (dest, VA_SPRINTF_BUFFER, format, args))
        return NULL;
    va_end (args);
    return dest;
}

/**
 * @brief 时间计数，秒周期调用，时间自动进位
 * 
 * @param p_time_count 需要
 */
void pub_time_counte(TIME_COUNTER *p_time_count)
{
    p_time_count->mid.second++;
    if (p_time_count->mid.second >= 60)
    {
        p_time_count->mid.second = 0;
        p_time_count->mid.minute++;
    }
    else
    {
        return;
    }

    if (p_time_count->mid.minute >= 60)
    {
        p_time_count->mid.minute = 0;
        p_time_count->mid.hour++;
    }
    else
    {
        return;
    }
    
    if (p_time_count->mid.hour >= 24)
    {
        p_time_count->mid.hour = 0;
        p_time_count->mid.day++;
    }
    else
    {
        return;
    }
#if TIME_COUNTER_YEAR_EN
    if (p_time_count->mid.day >=365)
    {
        p_time_count->mid.day = 0;
        p_time_count->mid.year++;
    }
    else
    {
        return;
    }
#else
    p_time_count->mid.day++;
#endif
}