#include "test.h"
#include "../mulit/mulit_blink.h"
#include <stdint.h>
#include <stdio.h>

uint32_t tick_sim = 0;

void blink1_on(void)
{
    printf("\033[1;0m blink1 is \033[1;32m on, %d\033[1;0m \r\n ", tick_sim);
}

void blink2_on(void)
{
    printf("\033[1;0m blink2 is \033[1;32m on, %d\033[1;0m \r\n ", tick_sim);
}

void blink3_on(void)
{
    printf("\033[1;0m blink3 is \033[1;32m on, %d\033[1;0m \r\n ", tick_sim);
}

void blink4_on(void)
{
    printf("\033[1;0m blink4 is \033[1;32m on, %d\033[1;0m \r\n ", tick_sim);
}

void blink1_off(void)
{
    printf("\033[1;0m blink1 is \033[1;32m off, %d\033[1;0m \r\n ", tick_sim);
}

void blink2_off(void)
{
    printf("\033[1;0m blink2 is \033[1;32m off, %d\033[1;0m \r\n ", tick_sim);
}

void blink3_off(void)
{
    printf("\033[1;0m blink3 is \033[1;32m off, %d\033[1;0m \r\n ", tick_sim);
}

void blink4_off(void)
{
    printf("\033[1;0m blink4 is \033[1;32m off, %d\033[1;0m \r\n ", tick_sim);
}

const struct blink_ops ops1 = {
    .on     = blink1_on,
    .off    = blink1_off,
    .toggle = NULL,
};

const struct blink_ops ops2 = {
    .on     = blink2_on,
    .off    = blink2_off,
    .toggle = NULL,
};

const struct blink_ops ops3 = {
    .on     = blink3_on,
    .off    = blink3_off,
    .toggle = NULL,
};

const struct blink_ops ops4 = {
    .on     = blink4_on,
    .off    = blink4_off,
    .toggle = NULL,
};

struct mulit_blink_map blink[4];

uint32_t mulit_blink_tick_get()
{
    return tick_sim;
}

void test_blink_main(void)
{
    TEST_FUNCTION_BEGIN();
    printf("--->>>blink num = %u\n", mulit_blink_numbers());
    mulit_blink_init(&blink[0], &ops1, BLINK_ON, 0);
    mulit_blink_init(&blink[1], &ops2, BLINK_OFF, 0);
    mulit_blink_init(&blink[2], &ops3, BLINK_TOGGLE, MULIT_TIME_SET(500, 200));
    mulit_blink_init(&blink[3], &ops4, BLINK_TOGGLE, MULIT_TIME_SET(1000, 500));
    printf("--->>>blink num = %u\n", mulit_blink_numbers());

    for (uint32_t i = 0; i < 2100; i++) {
        if (0 == (tick_sim % 100)) {
            mulit_blink_process();
        }
        tick_sim++;
    }
    TEST_FUNCTION_END();
}
