#include "test.h"
#include "../mulit/mulit_key.h"

struct mulit_key key1;

uint32_t test_raw_value(void)
{
    static uint32_t val = 0;

    val++;
    if (val > 10) {
        return 1;
    }
    else {
        return 0;
    }
}

void key1_handler(void* key)
{
    mulit_key_t tmp = key;

    printf("key event is %d\n", tmp->key.event);
}

void test_key(void)
{
    int i;

    TEST_FUNCTION_BEGIN();
    printf("key num is %d\n", mulit_key_numbers());
    mulit_key_init(&key1, 1, test_raw_value, key1_handler);
    printf("key num is %d\n", mulit_key_numbers());

    for (i = 0; i < 300; i++) {
        mulit_key_process();
    }

    TEST_FUNCTION_END();
}
