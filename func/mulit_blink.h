#ifndef __MULIT_BLINK_H_ //shift+U转换为大写
#define __MULIT_BLINK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
typedef enum {
    BLINK_ON,     /*!< 常量 */
    BLINK_OFF,    /*!< 常灭 */
    BLINK_TOGGLE, /*!< blink翻转 */
    blink_action_num,
} BLINK_ACTION;

/* 保留 */
typedef struct {
    void (*on)(uint32_t id);
    void (*off)(uint32_t id);
    void (*toggle)(uint32_t id);
} blink_ops_t;

typedef struct {
    struct {
        void (*on)(void);
        void (*off)(void);
        void (*toggle)(void);
    } ops;
    uint8_t      flag  : 1;
    uint8_t      index : 7; /*!< 保留，blink操作函数需要传入参数时调用该值 */
    BLINK_ACTION action;
    uint16_t     time_on;
    uint16_t     time_off;
    uint16_t     time_cnt;
    uint32_t     tick_last;
} mulit_blink_map_t;

extern void mulit_blink_init(uint32_t tick_max);
extern void mulit_blink_main(mulit_blink_map_t* blink_map, uint8_t group);
extern void mulit_blink_action_copy(mulit_blink_map_t* dst_map, const mulit_blink_map_t* src_map);
extern void mulit_blink_action_set(mulit_blink_map_t* p_map, BLINK_ACTION action, uint16_t time_on, uint16_t time_off);

#ifdef __cplusplus
}
#endif

#endif
