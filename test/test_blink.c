#include "../func/mulit_blink.h"
#include <stdint.h>
#include <stdio.h>

uint32_t tick_sim = 0;

void test_blink_init(void)
{
    mulit_blink_init(0xFFFFFFFF);
}

void blink1_on(void)
{
    printf("\033[1;0m blink1 is \033[1;32m on\033[1;0m \r\n");
}

void blink2_on(void)
{
    printf("\033[1;0m blink2 is \033[1;32m on\033[1;0m \r\n");
}

void blink3_on(void)
{
    printf("\033[1;0m blink3 is \033[1;32m on\033[1;0m \r\n");
}

void blink4_on(void)
{
    printf("\033[1;0m blink4 is \033[1;32m on\033[1;0m \r\n");
}

void blink1_off(void)
{
    printf("\033[1;0m blink1 is \033[1;31m off\033[1;0m \r\n");
}

void blink2_off(void)
{
    printf("\033[1;0m blink2 is \033[1;31m off\033[1;0m \r\n");
}

void blink3_off(void)
{
    printf("\033[1;0m blink3 is \033[1;31m off\033[1;0m \r\n");
}

void blink4_off(void)
{
    printf("\033[1;0m blink4 is \033[1;31m off\033[1;0m \r\n");
}

/* clang-format off */
mulit_blink_map_t blink_map[4] = {
{.ops.on=blink1_on, .ops.off=blink1_off, .ops.toggle=NULL,.flag=0,.index=0,.action=BLINK_ON,.time_on=100,.time_off=100,.tick_last=0},
{.ops.on=blink2_on, .ops.off=blink2_off, .ops.toggle=NULL,.flag=0,.index=1,.action=BLINK_OFF,.time_on=100,.time_off=100,.tick_last=0},
{.ops.on=blink3_on, .ops.off=blink3_off, .ops.toggle=NULL,.flag=0,.index=2,.action=BLINK_TOGGLE,.time_on=100,.time_off=100,.tick_last=0},
{.ops.on=blink4_on, .ops.off=blink4_off, .ops.toggle=NULL,.flag=0,.index=3,.action=BLINK_TOGGLE,.time_on=100,.time_off=500,.tick_last=0},
};
/* clang-format on */
uint32_t
mulit_blink_tick_get()
{
    return tick_sim;
}

void test_blink_main(void)
{
    for (uint32_t i = 0; i < 2000; i++) {
        tick_sim++;
        if (0 == (tick_sim % 100)) {
            printf("\r\ntick_sim = %d\r\n", tick_sim);
            mulit_blink_main(blink_map, 4);
        }
    }
}
