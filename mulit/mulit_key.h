#ifndef MULIT_KEY_H
#define MULIT_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MULIT_KEY_PRINTF (0)
#define MULIT_KEY_ASSERT (0)

#if MULIT_KEY_PRINTF
#define LOG_PRINT printf
#else
#define LOG_PRINT(...)
#endif

#if MULIT_KEY_ASSERT
#define ASSERT(x)                                                 \
    if (!(x)) {                                                   \
        LOG_PRINT("Assert failed, %s, %u\n", __FILE__, __LINE__); \
        while (1)                                                 \
            ;                                                     \
    }
#else
#define ASSERT(x)
#endif

#define KEY_DETECTION_ADC (0U)
#define KEY_DETECTION_DIG (1U)

#define KEY_PHY_DEFAULT (KEY_DETECTION_DIG)

#if (KEY_PHY_DEFAULT == KEY_DETECTION_ADC)
#define KEY_ADC_DELT (62U) /* 0.05 / 3.3 * 4096 = 62 按键判断误差在0.05V */
#endif

#define KEY_SCAN_TICKS   (5U) //ms
#define KEY_FILTER_TICKS (40 / KEY_SCAN_TICKS)
#define KEY_SHORT_TICKS  (100 / KEY_SCAN_TICKS)
#define KEY_LONG_TICKS   (1000 / KEY_SCAN_TICKS)

typedef enum {
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

/* 按键回调函数 */
typedef void (*key_callback)(void*);
/* 按键检测函数 */
typedef uint32_t (*key_raw_value)(void);

struct mulit_key {
    struct
    {
        uint16_t ticks;
        uint8_t  repeat     : 4;
        uint8_t  event      : 4;
        uint8_t  state      : 4;
        uint8_t  filter_cnt : 4;
        uint32_t active_level;
        uint32_t button_level;
    } key;

    key_raw_value     value;
    key_callback      cbk;
    struct mulit_key* next;
};
typedef struct mulit_key* mulit_key_t;

void     mulit_key_init(mulit_key_t key, uint32_t active_level, key_raw_value value, key_callback cbk);
uint16_t mulit_key_numbers(void);
void     mulit_key_process(void);

#ifdef __cplusplus
}
#endif
#endif
