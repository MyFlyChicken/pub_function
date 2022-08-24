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
#ifndef __LOG_PRINT_H__
#define __LOG_PRINT_H__
#include <stdarg.h>


#define LOG_PRINT_EN    1U

#if LOG_PRINT_EN

typedef void (* log_print_callback)(char * src);
typedef int (* log_print_port)(const char * format, ...);

typedef enum __LOG_PRINT_LEVEL__{
        LOG_PRINT_INFO = 0,
        LOG_PRINT_WARN,
        LOG_PRINT_ERROR,
        LOG_PRINT_USER,
        LOG_PRINT_NONE
}LOG_PRINT_LEVEL;

extern log_print_port g_log_print;

void log_print_init(log_print_port log_print, log_print_callback log_cb);
void log_print_info(LOG_PRINT_LEVEL level, const char * file, int line, const char * func);

#define LOG_PRINT_LEVEL_NUMBER  LOG_PRINT_USER

#if LOG_PRINT_LEVEL_NUMBER <= LOG_PRINT_PARA
#define LOG_PRINTF_INFO(...) do                    \
                         {                     \
                                log_print_info(LOG_PRINT_INFO, __FILE__, __LINE__, __func__); \
                                if (g_log_print) g_log_print(__VA_ARGS__);                    \
                         } while (0);
#endif

#if LOG_PRINT_LEVEL_NUMBER <= LOG_PRINT_WARN
#define LOG_PRINTF_WARN(...) do                    \
                         {                     \
                                log_print_info(LOG_PRINT_WARN, __FILE__, __LINE__, __func__); \
                                if (g_log_print) g_log_print(__VA_ARGS__);                    \
                         } while (0);
#endif

#if LOG_PRINT_LEVEL_NUMBER <= LOG_PRINT_ERROR
#define LOG_PRINTF_ERROR(...) do                    \
                         {                     \
                                log_print_info(LOG_PRINT_ERROR, __FILE__, __LINE__, __func__); \
                                if (g_log_print) g_log_print(__VA_ARGS__);                     \
                         } while (0);
#endif

#if LOG_PRINT_LEVEL_NUMBER <= LOG_PRINT_USER
#define LOG_PRINTF_USER(...) do                    \
                         {                     \
                                log_print_info(LOG_PRINT_USER, __FILE__, __LINE__, __func__); \
                                if (g_log_print) g_log_print(__VA_ARGS__);                    \
                         } while (0);
#endif

#endif/* LOG_PRINT_EN */

#endif/* __LOG_PRINT_H__*/