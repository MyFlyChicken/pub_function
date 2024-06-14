#include "test.h"
#include "../mr_mem/mr_mem.h"
#include "stdlib.h"

void test_mrmem_main(void)
{
    int   i = 10;
    void* p;
    TEST_FUNCTION_BEGIN();
    mr_heap_init();

    while (i--) {
        p = (void*)mr_malloc(20);
        printf("usable is %d i=%d\r\n", (int)mr_malloc_usable_size(p), i);
        mr_free(p);
    }
    TEST_FUNCTION_END();
}
