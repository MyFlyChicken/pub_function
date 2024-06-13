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
#include <cstdint>

#define __WEAK __attribute__((weak))

uint32_t _tick_max = 0xFFFFFFFF;
mulit_blink_map_t _mulit_blink_head = NULL;

__WEAK uint32_t mulit_blink_tick_get(void)
{
    return 0;
}

/**
 * @brief 初始化mulit_blink的tick最大计数
 * @param [in] tick_max 最大tick计数
 * 
 * @details 
 */
void mulit_blink_tick_init(uint32_t tick_max)
{
    _tick_max = tick_max;
}

void mulit_blink_init(mulit_blink_map_t blink, const struct blink_ops* ops, uint16_t time_on, uint16_t time_off)
{    
    if (!_mulit_blink_head) {
        while (NULL == _mulit_blink_head->next) {
           _mulit_blink_head->next = blink;
        }
    }
    else {
        _mulit_blink_head = blink;
    }
    blink->next = NULL;
    blink->ops = ops;
    blink->time_on = time_on;
    blink->time_on = time_off;
    blink->time_cnt  = 0;
    blink->tick_last = 0;       

    _mulit_blink_head->next = NULL;
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
    blink->time_cnt  = 0;
    blink->tick_last = 0;
}

static void mulit_blink_running(mulit_blink_map_t blink)
{
    uint32_t tick,delt;

    tick = mulit_blink_tick_get();
    if (BLINK_TOGGLE == blink->action) {
        if (blink->time_cnt < blink->time_on) {
            blink->ops->on();
        }
        else if (blink->time_cnt < (blink->time_on + blink->time_off)) {
            blink->ops->off();
        }
        else {
            blink->time_cnt = 0;
            blink->ops->on();
        }

        tick = mulit_blink_tick_get();
        delt = (tick >= blink->tick_last) ? (tick - blink->tick_last) : (_tick_max - blink->tick_last + tick);

        blink->time_cnt  += delt;
        blink->tick_last  = tick;
    }
    else if (BLINK_ON == blink->action) {
        blink->ops->on();
    }
    else if (BLINK_OFF == blink->action) {
        blink->ops->off();
    }
}

/**
 * @brief blink控制函数，周期调用
 * @param [in] blink_map mulit_blink_map_t数组指针
 * @param [in] group mulit_blink_map_t的组数
 * 
 * @details 
 */
void mulit_blink_main(void)
{
    mulit_blink_map_t blink = _mulit_blink_head;

    if (!blink) {
        return;
    }

    do {
        mulit_blink_running(blink);
        blink = blink->next;
    }while (blink->next);
}

uint16_t mulit_blink_numbers(void)
{
    if (!_mulit_blink_head) {
        return 0;
    }
}
