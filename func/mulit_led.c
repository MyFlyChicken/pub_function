#include "mulit_led.h"
#include <cstdint>

#define __WEAK __attribute__((weak))

uint32_t _tick_max = 0xFFFFFFFF;

__WEAK uint32_t mulit_led_tick_get(void)
{
    return 0;
}

/**
 * @brief 初始化mulit_led的tick最大计数
 * @param [in] tick_max 最大tick计数
 * 
 * @details 
 */
void mulit_led_init(uint32_t tick_max)
{
    _tick_max = tick_max;
}

/**
 * @brief 对mulit_led_map_t内的参数进行初始化
 * @param [in] p_map 指针
 * @param [in] action led的动作
 * @param [in] timeout1 翻转超时1
 * @param [in] timeout2 翻转超时2
 * 
 * @details 
 */
void mulit_led_action_set(mulit_led_map_t* p_map, LED_ACTION action, uint16_t timeout1, uint16_t timeout2)
{
    p_map->flag      = 0;
    p_map->action    = action;
    p_map->timeout1  = timeout1;
    p_map->timeout2  = timeout2;
    p_map->tick_last = 0;
}

/**
 * @brief 拷贝mulit_led_map_t的相关设置
 * @param [in] dst_map 目标map
 * @param [in] src_map 数据源
 * 
 * @details 
 */
void mulit_led_action_copy(mulit_led_map_t* dst_map, const mulit_led_map_t* src_map)
{
    dst_map->flag      = 0;
    dst_map->action    = src_map->action;
    dst_map->timeout1  = src_map->timeout1;
    dst_map->timeout2  = src_map->timeout2;
    dst_map->tick_last = 0;
}

/**
 * @brief led控制函数，周期调用
 * @param [in] led_map mulit_led_map_t数组指针
 * @param [in] group mulit_led_map_t的组数
 * 
 * @details 
 */
void mulit_led_main(mulit_led_map_t* led_map, uint8_t group)
{
    uint32_t tick, delt;
    uint8_t  i;

    for (i = 0; i < group; i++)
    {
        if (LED_ON == led_map[i].action)
        {
            led_map[i].flag = 0;
            led_map[i].ops.on();
        }
        else if (LED_OFF == led_map[i].action)
        {
            led_map[i].flag = 0;
            led_map[i].ops.off();
        }
        else if (LED_TOGGLE == led_map[i].action)
        {
            tick = mulit_led_tick_get();
            delt = (tick >= led_map[i].tick_last) ? (tick - led_map[i].tick_last) : (_tick_max - led_map[i].tick_last + tick);
            if ((delt >= led_map[i].timeout1) && (0 == led_map[i].flag))
            {
                led_map[i].flag = 1;
                led_map[i].ops.on();
            }
            else if (delt > led_map[i].timeout2)
            {
                led_map[i].flag = 0;
                led_map[i].ops.off();
                led_map[i].tick_last = mulit_led_tick_get();
            }
        }
    }
}
