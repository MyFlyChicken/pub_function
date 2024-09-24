#include "test.h"
#include "../os/polling/inc/polling.h"

#include <stdlib.h>
#include <stdio.h>

poll_u32_t tick = 0;

poll_u32_t poll_get_tick(void)
{
    return tick;
}

void* poll_malloc(poll_u32_t size)
{
    return malloc(size);
}

void poll_free(void* memory)
{
    free(memory);
}

void bsp_init(void)
{
    printf("bsp_init\n");
}
driver_init("bsp_init", bsp_init);

void sys_init(void)
{
    printf("sys_init\n");
}
system_init("sys_init", sys_init);

void modulee_init(void)
{
    printf("module_init\n");
}
module_init("module_init", modulee_init);

void task_printf1(void)
{
    printf("printf1, %u\n", tick);
}
poll_task_register("p1", task_printf1, 100);

void task_printf2(void)
{
    printf("printf2, %u\n", tick);
}
poll_task_register("p2", task_printf2, 200);

void task_printf3(void)
{
    printf("printf3, %u\n", tick);
}
poll_task_register("p3", task_printf3, 300);

void test_polling(void)
{
    TEST_FUNCTION_BEGIN();
    poll_module_init();

    for (int i = 0; i < 1000; i++) {
        poll_process();
        tick++;
    }
    TEST_FUNCTION_END();
}
