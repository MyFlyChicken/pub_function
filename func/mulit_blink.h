#ifndef __MULIT_BLINK_H_ //shift+U转换为大写
#define __MULIT_BLINK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define MULIT_TICK_MAX (0xFFFFFFFF)

typedef enum {
    BLINK_ON,     /*!< 常量 */
    BLINK_OFF,    /*!< 常灭 */
    BLINK_TOGGLE, /*!< blink翻转 */
    BLINK_PERIOD, /*!< 周期翻转，周期以time_on为准 */
    blink_action_num,
} BLINK_ACTION;

struct blink_ops {
    void (*on)(void);
    void (*off)(void);
    void (*toggle)(void);
};

struct mulit_blink_map {
    const struct blink_ops* ops;

    uint8_t action   : 4;
    uint8_t set_flag : 4;

    uint16_t time_on;
    uint16_t time_off;

    uint32_t time_cnt;
    uint32_t tick_last;

    struct mulit_blink_map* next;
};
typedef struct mulit_blink_map* mulit_blink_map_t;

void mulit_blink_init(mulit_blink_map_t blink, const struct blink_ops* ops, uint8_t action, uint32_t time);
void mulit_blink_action_set(mulit_blink_map_t blink, uint8_t action, uint16_t time_on, uint16_t time_off);
void mulit_blink_main();
#ifdef __cplusplus
}
#endif

#endif
