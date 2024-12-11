/**
 * @file pp_list.c
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

#include <stddef.h>
#include "../inc/pp_list.h"

void list_init(struct list_s* list)
{
    list->next = list;
    list->prev = list;
}

void list_add(struct list_s* lnew, struct list_s* lhead)
{
    lhead->next->prev = lnew;
    lnew->next        = lhead->next;
    lnew->prev        = lhead;
    lhead->next       = lnew;
}

void list_del(struct list_s* pos)
{
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;

    pos->next = NULL;
    pos->prev = NULL;
}
