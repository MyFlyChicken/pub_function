#ifndef MULIT_KEY_H
#define MULIT_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_PRINT(...) 

#define ASSERT(x)   if (!(x))   \
                    {           \
                        LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__); \
                    }

#define KEY_DETECTION_ADC (0U)
#define KEY_DETECTION_DIG (1U)

#define KEY_PHY_DEFAULT   (KEY_DETECTION_DIG)

#if (KEY_PHY_DEFAULT == KEY_DETECTION_ADC)
#define KEY_ADC_DELT (62U) /* 0.05 / 3.3 * 4096 = 62 按键判断误差在0.05V */
#elif (KEY_PHY_DEFAULT == KEY_DETECTION_DIG)
#define KEY_ACTIVE_LEVEL (1U)
#endif

#define KEY_SCAN_TICKS   (5U) //ms
#define KEY_FILTER_TICKS (40 / KEY_SCAN_TICKS)
#define KEY_SHORT_TICKS  (100 / KEY_SCAN_TICKS)
#define KEY_LONG_TICKS   (1000 / KEY_SCAN_TICKS)

typedef enum
{
    KEY_PRESS_DOWN = 0,
    KEY_PRESS_UP,
    KEY_PRESS_REPEAT,
    KEY_SINGLE_CLICK,
    KEY_DOUBLE_CLICK,
    KEY_LONG_PRESS_START,
    KEY_LONG_PRESS_HOLD,
    _key_number_of_event,
    KEY_NONE_PRESS
} key_press_event_e;

typedef struct
{
    unsigned short ticks;
    unsigned char  repeat     : 4;
    unsigned char  event      : 4;
    unsigned char  state      : 4;
    unsigned char  filter_cnt : 4;
    unsigned long active_level;
    unsigned long button_level;
} mulit_key_t;

/* 按键检测处理函数 */
typedef void (*key_event_deal)(void);
/* 按键周期扫描函数 */
void mulit_key_scan(unsigned long input, mulit_key_t* handle);    

#ifdef __cplusplus
}
#endif
#endif
