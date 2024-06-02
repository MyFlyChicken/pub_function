#include "../mr_mem/mr_mem.h"
#include "stdlib.h"

void test_mrmem_main(void)
{
    int   i = 10;
    void* p;

    mr_heap_init();

    while (i--) {
        p = (void*)mr_malloc(20);
        printf("usable is %d i=%d\r\n", (int)mr_malloc_usable_size(p), i);
        mr_free(p);
    }
}
