/**
 * @file mulit_blink.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-06-13
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-06-13 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "mulit_blink.h"

#define __WEAK __attribute__((weak))

mulit_blink_map_t _mulit_blink_head = NULL;

static void mulit_blink_time_update(mulit_blink_map_t blink);
static void mulit_blink_running(mulit_blink_map_t blink);

__WEAK uint32_t mulit_blink_tick_get(void)
{
    return 0;
}

/**
 * @brief 初始化blink
 * @param [in] blink 指针
 * @param [in] ops 操作函数
 * @param [in] action 动作
 * @param [in] time 时间，高16位time_on，低16位time_off 
 * 
 * @details 
 */
void mulit_blink_init(mulit_blink_map_t blink, const struct blink_ops* ops, uint8_t action, uint32_t time)
{
    mulit_blink_map_t tmp = NULL;

    if (_mulit_blink_head) {
        tmp = _mulit_blink_head;
        while (NULL != tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = blink;
    }
    else {
        _mulit_blink_head = blink;
    }
    blink->action    = action;
    blink->next      = NULL;
    blink->ops       = ops;
    blink->time_on   = time >> 16;
    blink->time_off  = time & 0xFFFF;
    blink->time_cnt  = MULIT_TICK_MAX;
    blink->tick_last = 0;
    blink->set_flag  = 0;
}

/**
 * @brief 对mulit_blink_map_t内的参数进行设置
 * @param [in] blink 指针
 * @param [in] action blink的动作
 * @param [in] timeout1 翻转超时1
 * @param [in] timeout2 翻转超时2
 * 
 * @details 
 */
void mulit_blink_action_set(mulit_blink_map_t blink, uint8_t action, uint16_t time_on, uint16_t time_off)
{
    blink->action    = action;
    blink->time_on   = time_on;
    blink->time_off  = time_off;
    blink->time_cnt  = MULIT_TICK_MAX;
    blink->tick_last = 0;
    blink->set_flag  = 0;
}

/**
 * @brief 更新blink的内部成员，用于判断翻转时间
 * @param [in] blink 
 * 
 * @details 
 */
static void mulit_blink_time_update(mulit_blink_map_t blink)
{
    uint32_t tick, delt;

    tick = mulit_blink_tick_get();
    delt = (tick >= blink->tick_last) ? (tick - blink->tick_last) : (MULIT_TICK_MAX - blink->tick_last + tick);

    blink->time_cnt  += delt;
    blink->tick_last  = tick;
}

static void mulit_blink_running(mulit_blink_map_t blink)
{
    if (BLINK_TOGGLE == blink->action) {
        if (blink->time_cnt >= (blink->time_on + blink->time_off)) {
            blink->ops->on();
            blink->time_cnt = 0;
            blink->set_flag = 0;
        }
        else if ((blink->time_cnt >= blink->time_on) && (0 == blink->set_flag)) {
            blink->ops->off();
            blink->set_flag = 1;
        }
        mulit_blink_time_update(blink);
    }
    else if ((BLINK_ON == blink->action) && (0 == blink->set_flag)) {
        blink->ops->on();
        blink->set_flag = 1;
    }
    else if ((BLINK_OFF == blink->action) && (0 == blink->set_flag)) {
        blink->ops->off();
        blink->set_flag = 1;
    }
}

/**
 * @brief blink控制函数，周期调用
 * @param [in] blink_map mulit_blink_map_t数组指针
 * @param [in] group mulit_blink_map_t的组数
 * 
 * @details 
 */
void mulit_blink_process(void)
{
    mulit_blink_map_t blink = _mulit_blink_head;

    if (!blink) {
        return;
    }

    while (blink->next) {
        mulit_blink_running(blink);
        blink = blink->next;
    }
    /* 最后一个next为空，跳出循环后这里需要执行 */
    mulit_blink_running(blink);
}

uint16_t mulit_blink_numbers(void)
{
    uint16_t          num   = 0;
    mulit_blink_map_t blink = _mulit_blink_head;

    if (!blink) {
        return 0;
    }

    num = 1;
    while (blink->next) {
        num++;
        blink = blink->next;
    }

    return num;
}
