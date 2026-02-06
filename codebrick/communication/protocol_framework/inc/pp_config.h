/**
 * @file pp_config.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-12-11 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */

#ifndef inc_pp_config_h
#define inc_pp_config_h

#ifdef __cplusplus
extern "C" {
#endif

/* User must create this file, and make the compiler can find it. */
/* User can modify some macro to custom pp. */
#include "pp_custom_config.h"

/* define the products we support now */

/* The len of stack rx buffer */
#ifndef PP_RINGBUFFER_LEN
#define PP_RINGBUFFER_LEN (4 * 1024)
#endif

/* The len of the buffer to save one frame */
#ifndef PP_FRAMEBUFFER_LEN
#define PP_FRAMEBUFFER_LEN (1024 + 20)
#endif

/* Enable or Disable the timeout detect of wait ack */
#ifndef PP_TIMEOUT_DET_EN
#define PP_TIMEOUT_DET_EN (1)
#endif

/* The max len of data that low level stack can send.
   The max num can be (65535 - 9), if you have enough memery. */
#ifndef PP_PAYLOAD_MAX_LEN
#define PP_PAYLOAD_MAX_LEN (1024 + 20)
#endif

/* Output logs */
#ifndef PP_PRINTF
#include <stdio.h>
#define PP_PRINTF printf
#endif

#ifndef PP_LOG_DEBUG_EN
#define PP_LOG_DEBUG_EN (1)
#endif

#ifndef PP_LOG_ERROR_EN
#define PP_LOG_ERROR_EN (1)
#endif

#if (defined PP_PRINTF) && PP_LOG_DEBUG_EN
#define PP_LOG_E(format, ...) PP_PRINTF("[E]%s,%d:" format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PP_LOG_E(format, ...)
#endif

#if (defined PP_PRINTF) && PP_LOG_ERROR_EN
#define PP_LOG_D(format, ...) PP_PRINTF("[D]" format "\n", ##__VA_ARGS__)
#else
#define PP_LOG_D(format, ...)
#endif

/* Dynamic Memory Allocation */
#ifndef PP_MALLOC
#include <stdlib.h>
#define PP_MALLOC malloc
#endif

#ifndef PP_FREE
#include <stdlib.h>
#define PP_FREE free
#endif

/* If there is no data for a long time, we will terminate this frame. */
#ifndef FRAME_TIMEOUT
#define FRAME_TIMEOUT (2000)
#endif

/* parse frame delay, release some time to run other thread */
#ifndef POLL_DELAY
#define POLL_DELAY (usleep(10000))
#endif

/* ASSERT */
#ifndef PP_ASSERT
#define PP_ASSERT(EX)                                                      \
    if (!(EX)) {                                                           \
        PP_PRINTF("assert fail: %s, %s, %d", #EX, __func__, __LINE__); \
        while (1)                                                          \
            ;                                                              \
    }
#endif

/* Compiler attribute used to ignore some functions */
#define PP_WEAK __attribute__((weak))

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_config_h */
