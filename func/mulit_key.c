/**
 * 移植自https://github.com/0x1abin/MultiTimer
*/
#include "mulit_key.h"

#ifndef NULL
#define NULL 0
#endif

/**
 * 按键扫描，返回按键事件
 * @param input 检测输入
 * @param handle 按键句柄
 * @return None
 */
void mulit_key_scan(unsigned long input, mulit_key_t* handle)
{
    ASSERT(handle != NULL);

    if (handle->state > 0)
    {
        handle->ticks++;
    }
    
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
    /*------------button debounce handle---------------*/
    if (input != handle->button_level)
    { // not equal to prev one
        // continue read 3 times same new level change
        if (++(handle->filter_cnt) >= KEY_FILTER_TICKS)
        {
            handle->button_level = input;
            handle->filter_cnt   = 0;
        }
    }
    else
    { // leved not change ,counter reset.
        handle->filter_cnt = 0;
    }
#else
    if ((input >= (handle->active_level - KEY_ADC_DELT)) && (input <= (handle->active_level + KEY_ADC_DELT)))
    {
        if ((handle->button_level >= (handle->active_level - KEY_ADC_DELT)) && (handle->button_level <= (handle->active_level + KEY_ADC_DELT)))
        {
            handle->filter_cnt  = 0;
        }
        else
        {
            if (++(handle->filter_cnt) >= KEY_FILTER_TICKS)
            {
                handle->button_level = input;
                handle->filter_cnt   = 0;
            }
        }
    }
    else
    {
        if ((handle->button_level >= (handle->active_level - KEY_ADC_DELT)) && (handle->button_level <= (handle->active_level + KEY_ADC_DELT)))
        {
            if (++(handle->filter_cnt) >= KEY_FILTER_TICKS)
            {
                handle->button_level = input;
                handle->filter_cnt   = 0;
            }
        }
        else
        {
            handle->filter_cnt  = 0;
        }
    }
#endif
    /*-----------------State machine-------------------*/
    switch (handle->state)
    {
    case 0:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (handle->button_level == handle->active_level)
#else
        if ((handle->button_level >= (handle->active_level - KEY_ADC_DELT)) && (handle->button_level <= (handle->active_level + KEY_ADC_DELT)))
#endif
        { // start press down
            handle->event  = (unsigned char)KEY_PRESS_DOWN;
            handle->ticks  = 0;
            handle->repeat = 1;
            handle->state  = 1;
            LOG_PRINT("%8x key_state is %d", handle, handle->event);
        }
        else
        {
            handle->event = (unsigned char)KEY_NONE_PRESS;
        }

        break;

    case 1:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (handle->button_level != handle->active_level)
#else
        if ((handle->button_level < (handle->active_level - KEY_ADC_DELT)) || (handle->button_level > (handle->active_level + KEY_ADC_DELT)))
#endif
        { // released press up
            handle->event = (unsigned char)KEY_PRESS_UP;
            handle->ticks = 0;
            handle->state = 2;
        }
        else if (handle->ticks > KEY_LONG_TICKS)
        {
            handle->event = (unsigned char)KEY_LONG_PRESS_START;
            handle->state = 5;
        }
        LOG_PRINT("%8x key_state is %d", handle, handle->event);
        break;

    case 2:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (handle->button_level == handle->active_level)
#else
        if ((handle->button_level >= (handle->active_level - KEY_ADC_DELT)) && (handle->button_level <= (handle->active_level + KEY_ADC_DELT)))
#endif
        { // press down again
            handle->event = (unsigned char)KEY_PRESS_DOWN;
            handle->repeat++;
            handle->ticks = 0;
            handle->state = 3;
        }
        else if (handle->ticks > KEY_SHORT_TICKS)
        { // released timeout
            if (handle->repeat == 1)
            {
                handle->event = (unsigned char)KEY_SINGLE_CLICK;
            }
            else if (handle->repeat == 2)
            {
                handle->event = (unsigned char)KEY_DOUBLE_CLICK;
            }
            handle->state = 0;
        }
        LOG_PRINT("%8x key_state is %d", handle, handle->event);
        break;

    case 3:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (handle->button_level != handle->active_level)
#else
        if ((handle->button_level < (handle->active_level - KEY_ADC_DELT)) || (handle->button_level > (handle->active_level + KEY_ADC_DELT)))
#endif
        { // released press up
            handle->event = (unsigned char)KEY_PRESS_UP;
            if (handle->ticks < KEY_SHORT_TICKS)
            {
                handle->ticks = 0;
                handle->state = 2; // repeat press
            }
            else
            {
                handle->state = 0;
            }
        }
        else if (handle->ticks > KEY_SHORT_TICKS)
        { // long press up
            handle->state = 0;
        }
        LOG_PRINT("%8x key_state is %d", handle, handle->event);
        break;

    case 5:
#if (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
        if (handle->button_level == handle->active_level)
#else
        if ((handle->button_level >= (handle->active_level - KEY_ADC_DELT)) && (handle->button_level <= (handle->active_level + KEY_ADC_DELT)))
#endif
        {
            // continue hold trigger
            handle->event = (unsigned char)KEY_LONG_PRESS_HOLD;
        }
        else
        { // releasd
            handle->event = (unsigned char)KEY_PRESS_UP;
            handle->state = 0; // reset
        }
        LOG_PRINT("%8x key_state is %d", handle, handle->event);
        break;
    default:
        handle->state = 0; // reset
        break;
    }
}

