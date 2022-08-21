/**
 * @file log_print.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "log_print.h"


#if LOG_PRINT_EN

#ifndef NULL
#define NULL 0
#endif/* NULL */

log_print_port g_log_print = NULL;
static log_print_callback _log_cb = NULL;
static const char * log_level[] = {"Para", "Warn", "Error", "User"};

void log_print_init(log_print_port log_print, log_print_callback log_cb)
{
        g_log_print = log_print;
        _log_cb    = log_cb;
}

/**
 * @brief 用于打印一些固定参数
 * 
 * @param level 
 * @param file 
 * @param line 
 * @param func 
 */
void log_print_info(LOG_PRINT_LEVEL level, const char * file, int line, const char * func)
{
        if ((NULL == g_log_print)||(level > LOG_PRINT_USER))
        {
                return;
        }
        g_log_print("%s: %s, %d, %s, ", log_level[level], file, line, func);
}

#endif /* LOG_PRINT_EN */