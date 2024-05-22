#include "../func/mulit_led.h"
#include <stdint.h>
#include <stdio.h>

uint32_t tick_sim = 0;

void test_led_init(void)
{
    mulit_led_init(0xFFFFFFFF);
}

void led1_on(void)
{
    printf("\033[1;0m led1 is \033[1;32m on\033[1;0m \r\n");
}

void led2_on(void)
{
    printf("\033[1;0m led2 is \033[1;32m on\033[1;0m \r\n");
}

void led3_on(void)
{
    printf("\033[1;0m led3 is \033[1;32m on\033[1;0m \r\n");
}

void led4_on(void)
{
    printf("\033[1;0m led4 is \033[1;32m on\033[1;0m \r\n");
}

void led1_off(void)
{
    printf("\033[1;0m led1 is \033[1;31m off\033[1;0m \r\n");
}

void led2_off(void)
{
    printf("\033[1;0m led2 is \033[1;31m off\033[1;0m \r\n");
}

void led3_off(void)
{
    printf("\033[1;0m led3 is \033[1;31m off\033[1;0m \r\n");
}

void led4_off(void)
{
    printf("\033[1;0m led4 is \033[1;31m off\033[1;0m \r\n");
}

/* clang-format off */
mulit_led_map_t led_map[4] = {
{.ops.on=led1_on, .ops.off=led1_off, .ops.toggle=NULL,.flag=0,.index=0,.action=LED_ON,.time_on=100,.time_off=100,.tick_last=0},
{.ops.on=led2_on, .ops.off=led2_off, .ops.toggle=NULL,.flag=0,.index=1,.action=LED_OFF,.time_on=100,.time_off=100,.tick_last=0},
{.ops.on=led3_on, .ops.off=led3_off, .ops.toggle=NULL,.flag=0,.index=2,.action=LED_TOGGLE,.time_on=100,.time_off=100,.tick_last=0},
{.ops.on=led4_on, .ops.off=led4_off, .ops.toggle=NULL,.flag=0,.index=3,.action=LED_TOGGLE,.time_on=100,.time_off=500,.tick_last=0},
};
/* clang-format on */
uint32_t
mulit_led_tick_get()
{
    return tick_sim;
}

void test_led_main(void)
{
    for (uint32_t i = 0; i < 2000; i++)
    {
        tick_sim++;
        if (0 == (tick_sim % 100))
        {
            printf("\r\ntick_sim = %d\r\n", tick_sim);
            mulit_led_main(led_map, 4);
        }
    }
}
