#include "../mr_mem/mr_mem.h"
#include "stdlib.h"
void test_mrmem_main(void)
{
    int   i = 0;
    void* p[1000];

    mr_heap_init();

    p[i] = (void*)mr_malloc(20);
    while (p[i] != NULL)
    {
        i++;
        p[i] = (void*)mr_malloc(20);
        printf("usable is %d i=%d\r\n", mr_malloc_usable_size(p[i]), i);
        // if (0 == (i % 2))
        // {
        mr_free(p[i]);
        // }
        if (i > 998)
        {
            return;
        }
    }

    // for (int j = 0; j < i; j++)
    // {
    //     mr_free(p[j]);
    // }
}
