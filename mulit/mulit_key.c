/**
 * @file mulit_key.c
 * @author yuyf ()
 * @brief 移植自 https://github.com/0x1abin/MultiTimer，最多支持双击，双击以上需要魔改！！！
 * @version 0.1
 * @date 2024-06-14
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-06-14 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "mulit_key.h"

#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif

mulit_key_t _mulit_key_head = NULL;

/**
 * @brief 按键扫描，并执行对应事件
 * @param [in] key 
 * 
 * @details 
 */
static void mulit_key_scan(mulit_key_t key)
{
    ASSERT(key != NULL);

    uint32_t input;

    if (key->key.state > 0) {
        key->key.ticks++;
    }

    input = key->value();
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
    /*------------button debounce handle---------------*/
    if (input != key->key.button_level) { // not equal to prev one
        // continue read 3 times same new level change
        if (++(key->key.filter_cnt) >= KEY_FILTER_TICKS) {
            key->key.button_level = input;
            key->key.filter_cnt   = 0;
        }
    }
    else { // leved not change ,counter reset.
        key->key.filter_cnt = 0;
    }
#else
    if ((input >= (key->key.active_level - KEY_ADC_DELT)) && (input <= (key->key.active_level + KEY_ADC_DELT))) {
        if ((key->key.button_level >= (key->key.active_level - KEY_ADC_DELT)) && (key->key.button_level <= (key->key.active_level + KEY_ADC_DELT))) {
            key->key.filter_cnt = 0;
        }
        else {
            if (++(key->key.filter_cnt) >= KEY_FILTER_TICKS) {
                key->key.button_level = input;
                key->key.filter_cnt   = 0;
            }
        }
    }
    else {
        if ((key->key.button_level >= (key->key.active_level - KEY_ADC_DELT)) && (key->key.button_level <= (key->key.active_level + KEY_ADC_DELT))) {
            if (++(key->key.filter_cnt) >= KEY_FILTER_TICKS) {
                key->key.button_level = input;
                key->key.filter_cnt   = 0;
            }
        }
        else {
            key->key.filter_cnt = 0;
        }
    }
#endif
    /*-----------------State machine-------------------*/
    switch (key->key.state) {
    case 0:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (key->key.button_level == key->key.active_level)
#else
        if ((key->key.button_level >= (key->key.active_level - KEY_ADC_DELT)) && (key->key.button_level <= (key->key.active_level + KEY_ADC_DELT)))
#endif
        { // start press down
            key->key.event  = (unsigned char)KEY_PRESS_DOWN;
            key->key.ticks  = 0;
            key->key.repeat = 1;
            key->key.state  = 1;
            LOG_PRINT("%8x key_state is %d\n", key, key->key.event);
        }
        else {
            key->key.event = (unsigned char)KEY_NONE_PRESS;
        }
        break;
    case 1:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (key->key.button_level != key->key.active_level)
#else
        if ((key->key.button_level < (key->key.active_level - KEY_ADC_DELT)) || (key->key.button_level > (key->key.active_level + KEY_ADC_DELT)))
#endif
        { // released press up
            key->key.event = (unsigned char)KEY_PRESS_UP;
            key->key.ticks = 0;
            key->key.state = 2;
        }
        else if (key->key.ticks > KEY_LONG_TICKS) {
            key->key.event = (unsigned char)KEY_LONG_PRESS_START;
            key->key.state = 5;
        }
        LOG_PRINT("%8x key_state is %d\n", key, key->key.event);
        break;

    case 2:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (key->key.button_level == key->key.active_level)
#else
        if ((key->key.button_level >= (key->key.active_level - KEY_ADC_DELT)) && (key->key.button_level <= (key->key.active_level + KEY_ADC_DELT)))
#endif
        { // press down again
            key->key.event = (unsigned char)KEY_PRESS_DOWN;
            key->key.repeat++;
            key->key.ticks = 0;
            key->key.state = 3;
        }
        else if (key->key.ticks > KEY_SHORT_TICKS) { // released timeout
            if (key->key.repeat == 1) {
                key->key.event = (unsigned char)KEY_SINGLE_CLICK;
            }
            else if (key->key.repeat == 2) {
                key->key.event = (unsigned char)KEY_DOUBLE_CLICK;
            }
            key->key.state = 0;
        }
        LOG_PRINT("%8x key_state is %d\n", key, key->key.event);
        break;

    case 3:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (key->key.button_level != key->key.active_level)
#else
        if ((key->key.button_level < (key->key.active_level - KEY_ADC_DELT)) || (key->key.button_level > (key->key.active_level + KEY_ADC_DELT)))
#endif
        { // released press up
            key->key.event = (unsigned char)KEY_PRESS_UP;
            if (key->key.ticks < KEY_SHORT_TICKS) {
                key->key.ticks = 0;
                key->key.state = 2; // repeat press
            }
            else {
                key->key.state = 0;
            }
        }
        else if (key->key.ticks > KEY_SHORT_TICKS) { // long press up
            key->key.state = 0;
        }
        LOG_PRINT("%8x key_state is %d\n", key, key->key.event);
        break;

    case 5:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (key->key.button_level == key->key.active_level)
#else
        if ((key->key.button_level >= (key->key.active_level - KEY_ADC_DELT)) && (key->key.button_level <= (key->key.active_level + KEY_ADC_DELT)))
#endif
        {
            // continue hold trigger
            key->key.event = (unsigned char)KEY_LONG_PRESS_HOLD;
        }
        else { // releasd
            key->key.event = (unsigned char)KEY_PRESS_UP;
            key->key.state = 0; // reset
        }
        LOG_PRINT("%8x key_state is %d\n", key, key->key.event);
        break;
    default:
        key->key.state = 0; // reset
        break;
    }
}

void mulit_key_init(mulit_key_t key, uint32_t active_level, key_raw_value value, key_callback cbk)
{
    mulit_key_t tmp = NULL;

    if (_mulit_key_head) {
        tmp = _mulit_key_head;
        while (NULL != tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = key;
    }
    else {
        _mulit_key_head = key;
    }

    memset(key, 0, sizeof(struct mulit_key));
    key->next             = NULL;
    key->cbk              = cbk;
    key->value            = value;
    key->key.active_level = active_level;
}

uint16_t mulit_key_numbers(void)
{
    uint16_t    num = 0;
    mulit_key_t key = _mulit_key_head;

    if (!key) {
        return 0;
    }

    num = 1;
    while (key->next) {
        num++;
        key = key->next;
    }

    return num;
}

void mulit_key_process(void)
{
    mulit_key_t key = _mulit_key_head;

    if (!key) {
        return;
    }

    while (key->next) {
        mulit_key_scan(key);
        ASSERT(key->cbk);
        key->cbk(key);
        key = key->next;
    }
    /* 最后一个next为空，跳出循环后这里需要执行 */
    mulit_key_scan(key);
    ASSERT(key->cbk);
    key->cbk(key);
}
