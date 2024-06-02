#include "mulit_blink.h"

#define __WEAK __attribute__((weak))

uint32_t _tick_max = 0xFFFFFFFF;

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
void mulit_blink_init(uint32_t tick_max)
{
    _tick_max = tick_max;
}

/**
 * @brief 对mulit_blink_map_t内的参数进行初始化
 * @param [in] p_map 指针
 * @param [in] action blink的动作
 * @param [in] timeout1 翻转超时1
 * @param [in] timeout2 翻转超时2
 * 
 * @details 
 */
void mulit_blink_action_set(mulit_blink_map_t* p_map, BLINK_ACTION action, uint16_t time_on, uint16_t time_off)
{
    p_map->flag      = 0;
    p_map->action    = action;
    p_map->time_on   = time_on;
    p_map->time_off  = time_off;
    p_map->tick_last = 0;
    p_map->time_cnt  = 0;
}

/**
 * @brief 拷贝mulit_blink_map_t的相关设置
 * @param [in] dst_map 目标map
 * @param [in] src_map 数据源
 * 
 * @details 
 */
void mulit_blink_action_copy(mulit_blink_map_t* dst_map, const mulit_blink_map_t* src_map)
{
    dst_map->flag      = 0;
    dst_map->action    = src_map->action;
    dst_map->time_on   = src_map->time_on;
    dst_map->time_off  = src_map->time_off;
    dst_map->tick_last = 0;
    dst_map->time_cnt  = 0;
}

/**
 * @brief blink控制函数，周期调用
 * @param [in] blink_map mulit_blink_map_t数组指针
 * @param [in] group mulit_blink_map_t的组数
 * 
 * @details 
 */
void mulit_blink_main(mulit_blink_map_t* blink_map, uint8_t group)
{
    uint32_t tick, delt;
    uint8_t  i;

    for (i = 0; i < group; i++) {
        if (BLINK_TOGGLE == blink_map[i].action) {
            if (blink_map[i].time_cnt < blink_map[i].time_on) {
                blink_map[i].ops.on();
            }
            else if (blink_map[i].time_cnt < (blink_map[i].time_on + blink_map[i].time_off)) {
                blink_map[i].ops.off();
            }
            else {
                blink_map[i].time_cnt = 0;
                blink_map[i].ops.on();
            }

            tick = mulit_blink_tick_get();
            delt = (tick >= blink_map[i].tick_last) ? (tick - blink_map[i].tick_last) : (_tick_max - blink_map[i].tick_last + tick);

            blink_map[i].time_cnt  += delt;
            blink_map[i].tick_last  = tick;
        }
        else if (BLINK_ON == blink_map[i].action) {
            blink_map[i].ops.on();
        }
        else if (BLINK_OFF == blink_map[i].action) {
            blink_map[i].ops.off();
        }
    }
}
