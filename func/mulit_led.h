#ifndef __MULIT_LED_H_ //shift+U转换为大写
#define __MULIT_LED_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
    /* clang-format off */
    typedef enum {
        LED_ON,     /*!< 常量 */
        LED_OFF,    /*!< 常灭 */
        LED_TOGGLE, /*!< led翻转 */        
        led_action_num,
    } LED_ACTION;

    /* 保留 */
    typedef struct{
        void (*on)(uint32_t id);
        void (*off)(uint32_t id);
        void (*toggle)(uint32_t id);
    } led_ops_t;

    typedef struct {
        struct {
            void (*on)(void);
            void (*off)(void);
            void (*toggle)(void);
        }ops;
        uint8_t flag  : 1;
        uint8_t index : 7;/*!< 保留，led操作函数需要传入参数时调用该值 */
        LED_ACTION action; 
        uint16_t timeout1;
        uint16_t timeout2;
        uint32_t tick_last;
    } mulit_led_map_t;

    extern void mulit_led_init(uint32_t tick_max);
    extern void mulit_led_main(mulit_led_map_t* led_map, uint8_t group);
    extern void mulit_led_action_set(mulit_led_map_t* p_map, LED_ACTION action, uint16_t timeout1, uint16_t timeout2);
    /* clang-format on */

#ifdef __cplusplus
}
#endif

#endif
