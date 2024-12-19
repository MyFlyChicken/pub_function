#include "test.h"
#include "../func/func_queue.h"
#include <stdint.h>
#include <stdio.h>

void test_func_queue(void)
{
    bool         ret;
    func_queue_t queue;
    uint8_t      i, tmp;
    uint8_t      queue_buffer[10];

    TEST_FUNCTION_BEGIN();

    ret = func_queue_create(&queue, queue_buffer, 10, 1);
    printf("creat queue %d\n", ret);

    for (i = 0; i < 12; i++) {
        ret = func_queue_put(&queue, &i);
        printf("put queue ret=%d,put value is %u\n", ret, i);
    }
    printf("get queue len %d\n", func_queue_size(&queue));

    for (i = 0; i < 6; i++) {
        ret = func_queue_get(&queue, &tmp);
        printf("get queue ret=%d,get value is %u\n", ret, i);
    }

    for (i = 0; i < 6; i++) {
        ret = func_queue_put(&queue, &i);
        printf("put queue ret=%d,put value is %u\n", ret, i);
    }

    for (i = 0; i < 12; i++) {
        ret = func_queue_get(&queue, &tmp);
        printf("get queue ret=%d,get value is %u\n", ret, i);
    }

    TEST_FUNCTION_END();
}
