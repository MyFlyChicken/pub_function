/**
 * @file poll_port.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-05-14
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">接口</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-05-14 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "../inc/poll_port.h"

WEAK poll_u32_t poll_get_tick(void)
{
    return 0;
}

WEAK void* poll_malloc(poll_u32_t size)
{
    (void)size;
}

WEAK void poll_free(void* memory)
{
    (void)memory;
}
