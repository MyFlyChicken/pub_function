/**
 * @file poll.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-05-14
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">轮询</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-05-14 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "../inc/polling.h"
#include <stdio.h>

/*
 * @brief       超时判断
 * @param[in]   start   - 起始时间
 * @param[in]   timeout - 超时时间(ms)
 */
poll_bool_e is_timeout(unsigned int start, unsigned int timeout)
{
    return (poll_get_tick() - start) > timeout;
}

/*
 * @brief       空处理,用于定位段入口
 */
static void nop_process(void)
{
}

//第一个初始化项
const poll_item_t poll_tbl_start SECTION("poll.item.0") = {
    "",
    nop_process};
//最后个初始化项
const poll_item_t poll_tbl_end SECTION("poll.item.4") = {
    "",
    nop_process};

//第一个任务项
const poll_task_t poll_task_start SECTION("poll.task.0") = {
    "",
    nop_process};
//最后个任务项
const poll_task_t poll_task_end SECTION("poll.task.2") = {
    "",
    nop_process};

/*
 * @brief       模块初始处理
 *              初始化模块优化级 system_init > driver_init > module_init
 * @param[in]   none
 * @return      none
 */
void poll_module_init(void)
{
    const poll_item_t* it = &poll_tbl_start;
    while (it < &poll_tbl_end)
    {
        it++->init();
    }
}

/*
 * @brief       任务轮询处理
 * @param[in]   none
 * @return      none
 */
void poll_process(void)
{
    const poll_task_t* t;
    for (t = &poll_task_start + 1; t < &poll_task_end; t++)
    {
        if ((poll_get_tick() - *t->timer) >= t->interval)
        {
            *t->timer = poll_get_tick();
            t->handle();
        }
    }
}
