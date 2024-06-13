#ifndef __MULIT_BLINK_H_ //shift+U转换为大写
#define __MULIT_BLINK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef enum {
    BLINK_ON,     /*!< 常量 */
    BLINK_OFF,    /*!< 常灭 */
    BLINK_TOGGLE, /*!< blink翻转 */
    blink_action_num,
} BLINK_ACTION;

struct blink_ops {
    void (*on)(void);
    void (*off)(void);
    void (*toggle)(void);
};

struct mulit_blink_map {
    BLINK_ACTION            action;
    const struct blink_ops* ops;

    uint16_t time_on;
    uint16_t time_off;

    uint32_t time_cnt;
    uint32_t tick_last;

    struct mulit_blink_map* next;
};
typedef struct mulit_blink_map* mulit_blink_map_t;

void mulit_blink_tick_init(uint32_t tick_max);
void mulit_blink_init(mulit_blink_map_t blink, const struct blink_ops* ops, uint16_t time_on, uint16_t time_off);
void mulit_blink_action_set(mulit_blink_map_t blink, uint8_t action, uint16_t time_on, uint16_t time_off);
void mulit_blink_main();
#ifdef __cplusplus
}
#endif

#endif
