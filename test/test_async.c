#include "test.h"
#include "../async_work/async_work.h"
#include <stdio.h>

async_work_t w;
work_node_t  w_node[10];

void async_work_func1(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("test async_work start\r\n");
}

void async_work_func2(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func2\r\n");
}

void async_work_func3(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func3\r\n");
}

void async_work_func4(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func4\r\n");
}

void async_work_func5(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func5\r\n");
}

void async_work_func6(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func6\r\n");
}

void async_work_func7(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func7\r\n");
}

void async_work_func8(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("async_work_func8\r\n");
}

void async_work_func10(async_work_t* w, void* object, void* params);
void async_work_func11(async_work_t* w, void* object, void* params);
void async_work_func12(async_work_t* w, void* object, void* params);
void async_work_func13(async_work_t* w, void* object, void* params);

void async_work_func9(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;
    async_work_add(w, NULL, NULL, async_work_func10);
    printf("async_work_func9\r\n");
}

void async_work_func10(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    async_work_add(w, NULL, NULL, async_work_func11);
    printf("async_work_func10\r\n");
}

void async_work_func11(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    async_work_add(w, NULL, NULL, async_work_func12);
    printf("async_work_func11\r\n");
}

void async_work_func12(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;
    async_work_add(w, NULL, NULL, async_work_func13);
    printf("async_work_func12\r\n");
}

void async_work_func13(async_work_t* w, void* object, void* params)
{
    (void)w;
    (void)object;
    (void)params;

    printf("test async_work end!!!\r\n");
}

void test_async(void)
{
    int i = 13;
    TEST_FUNCTION_BEGIN();
    async_work_init(&w, w_node, sizeof(w_node) / sizeof(work_node_t));

    async_work_add(&w, NULL, NULL, async_work_func1);
    async_work_add(&w, NULL, NULL, async_work_func2);
    async_work_add(&w, NULL, NULL, async_work_func3);
    async_work_add(&w, NULL, NULL, async_work_func4);
    async_work_add(&w, NULL, NULL, async_work_func5);
    async_work_add(&w, NULL, NULL, async_work_func6);
    async_work_add(&w, NULL, NULL, async_work_func7);
    async_work_add(&w, NULL, NULL, async_work_func8);
    async_work_add(&w, NULL, NULL, async_work_func9);

    while (i--) {
        async_work_process(&w);
    }
    TEST_FUNCTION_END();
}
