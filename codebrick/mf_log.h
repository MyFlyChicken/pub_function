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
#ifndef __MF_LOG_H__
#define __MF_LOG_H__
#include <stdarg.h>
#include <stdio.h>

#define MF_LOG_EN       (1U)
#define MF_LOG_COLOR_EN (1U)

#ifndef MF_ASSERT_EN
#define MF_ASSERT_EN (1U) /* 默认开启断言 */
#endif
/*****************************************************************/
/*********************** 进行DEBUG ****************************/
#if MF_LOG_EN
#define MF_LOG_PRINT printf /* 打印接口 */
#else
#define MF_LOG_PRINT(...)
#endif

typedef enum {
    MF_LOG_ERROR = 0,
    MF_LOG_WARNING,
    MF_LOG_INFO,
    MF_LOG_DBG,
} mf_log_e;

#ifndef MF_LOG_TAG
#define MF_LOG_TAG "DBG"
#endif

#ifndef MF_LOG_LEVEL
#define MF_LOG_LEVEL MF_LOG_INFO
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
#ifdef MF_LOG_COLOR_EN
#define _MF_LOG_COLOR(n) MF_LOG_PRINT("\033[" #n "m")
#define _MF_LOG_HDR(lvl_name, color_n) \
    MF_LOG_PRINT("\033[" #color_n "m[" lvl_name "/" MF_LOG_TAG "] ")
#define _MF_LOG_X_END \
    MF_LOG_PRINT("\033[0m\n")
#else
#define _MF_LOG_COLOR(n)
#define _MF_LOG_HDR(lvl_name, color_n) \
    MF_LOG_PRINT("[" lvl_name "/" MF_LOG_TAG "] ")
#define _MF_LOG_X_END \
    MF_LOG_PRINT("\n")
#endif /* DBG_COLOR */

#define MF_LOG_LINE(lvl, color_n, fmt, ...) \
    do {                                    \
        _MF_LOG_HDR(lvl, color_n);          \
        MF_LOG_PRINT(fmt, ##__VA_ARGS__);   \
        _MF_LOG_X_END;                      \
    } while (0)

#if (MF_LOG_LEVEL >= MF_LOG_DBG)
#define MF_LOG_D(fmt, ...) MF_LOG_LINE("D", 0, fmt, ##__VA_ARGS__)
#else
#define MF_LOG_D(...)
#endif

#if (MF_LOG_LEVEL >= MF_LOG_INFO)
#define MF_LOG_I(fmt, ...) MF_LOG_LINE("I", 32, fmt, ##__VA_ARGS__)
#else
#define MF_LOG_I(...)
#endif

#if (MF_LOG_LEVEL >= MF_LOG_WARNING)
#define MF_LOG_W(fmt, ...) MF_LOG_LINE("W", 33, fmt, ##__VA_ARGS__)
#else
#define MF_LOG_W(...)
#endif

#if (MF_LOG_LEVEL >= MF_LOG_ERROR)
#define MF_LOG_E(fmt, ...) MF_LOG_LINE("E", 31, fmt, ##__VA_ARGS__)
#else
#define MF_LOG_E(...)
#endif

/*********************************************************************/
/***************************   校验实参   *****************************/
/* clang-format off */
#if MF_ASSERT_EN
#define MF_ASSERT(n)                                              \
    do                                                                  \
    {                                                                   \
        if (!(n))                                                       \
        {                                                               \
            MF_LOG_PRINT("%s, %d\n", __FILE__, __LINE__);  \
            while (1);                                                  \
        }                                                               \
    }while (0)
/* clang-format on */
#else
#define MF_ASSERT(n)
#endif

#endif /* __LOG_PRINT_H__*/
