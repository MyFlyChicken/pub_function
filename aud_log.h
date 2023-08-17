/**
 * @file log_print.h
 * @author your name (you@domain.com)
 * @brief 参考lv_log文件
 * @version 0.1
 * @date 2022-08-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __AUD_LOG_H__
#define __AUD_LOG_H__
#include <stdarg.h>

#define AUD_LOG_EN       (1U)
#define AUD_LOG_COLOR_EN (1U)
#define AUD_ASSERT_EN    (1U)
/*****************************************************************/
/*********************** 进行DEBUG ****************************/
#if AUD_LOG_EN
#define AUD_LOG_PRINT rt_kprintf /* 打印接口 */
#else
#define AUD_LOG_PRINT(...)
#endif

typedef enum
{
    AUD_LOG_ERROR = 0,
    AUD_LOG_WARNING,
    AUD_LOG_INFO,
    AUD_LOG_DBG,
} aud_log_e;

#ifndef AUD_LOG_TAG
#define AUD_LOG_TAG "DBG"
#endif

#ifndef AUD_LOG_LEVEL
#define AUD_LOG_LEVEL AUD_LOG_INFO
#endif

/*
 * The color for terminal (foreground)
 * BLACK    30
 * RED      31
 * GREEN    32
 * YELLOW   33
 * BLUE     34
 * PURPLE   35
 * CYAN     36
 * WHITE    37
 */
#ifdef AUD_LOG_COLOR_EN
#define _AUD_LOG_COLOR(n) AUD_LOG_PRINT("\033[" #n "m")
#define _AUD_LOG_HDR(lvl_name, color_n) \
    AUD_LOG_PRINT("\033[" #color_n "m[" lvl_name "/" AUD_LOG_TAG "] ")
#define _AUD_LOG_X_END \
    AUD_LOG_PRINT("\033[0m\n")
#else
#define _AUD_LOG_COLOR(n)
#define _AUD_LOG_HDR(lvl_name, color_n) \
    AUD_LOG_PRINT("[" lvl_name "/" AUD_LOG_TAG "] ")
#define _AUD_LOG_X_END \
    AUD_LOG_PRINT("\n")
#endif /* DBG_COLOR */

#define AUD_LOG_LINE(lvl, color_n, fmt, ...) \
    do                                            \
    {                                             \
        _AUD_LOG_HDR(lvl, color_n);          \
        AUD_LOG_PRINT(fmt, ##__VA_ARGS__);   \
        _AUD_LOG_X_END;                      \
    } while (0)

#if (AUD_LOG_LEVEL >= AUD_LOG_DBG)
#define AUD_LOG_D(fmt, ...) AUD_LOG_LINE("D", 0, fmt, ##__VA_ARGS__)
#else
#define AUD_LOG_D(...)
#endif

#if (AUD_LOG_LEVEL >= AUD_LOG_INFO)
#define AUD_LOG_I(fmt, ...) AUD_LOG_LINE("I", 32, fmt, ##__VA_ARGS__)
#else
#define AUD_LOG_I(...)
#endif

#if (AUD_LOG_LEVEL >= AUD_LOG_WARNING)
#define AUD_LOG_W(fmt, ...) AUD_LOG_LINE("W", 33, fmt, ##__VA_ARGS__)
#else
#define AUD_LOG_W(...)
#endif

#if (AUD_LOG_LEVEL >= AUD_LOG_ERROR)
#define AUD_LOG_E(fmt, ...) AUD_LOG_LINE("E", 31, fmt, ##__VA_ARGS__)
#else
#define AUD_LOG_E(...)
#endif

/*********************************************************************/
/***************************   校验实参   *****************************/
/* clang-format off */
#if AUD_ASSERT_EN
#define AUD_ASSERT(n)                                              \
    do                                                                  \
    {                                                                   \
        if (!(n))                                                       \
        {                                                               \
            AUD_LOG_PRINT("%s, %s, %s\n", __FILE__, __LINE__, n);  \
            while (1);                                                  \
        }                                                               \
    }while (0)
    /* clang-format on */
#else
#define AUD_ASSERT(n)
#endif

#endif/* __LOG_PRINT_H__*/
