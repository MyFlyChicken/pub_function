#ifndef __mr_mem_H_ //shift+U转换为大写
#define __mr_mem_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
    /* clang-format off */
#define MR_ALIGN_UP(value, align) (((value) + (align) - 1) & ~((align) - 1))

#define mr_interrupt_enable()
#define mr_interrupt_disable()

#define MR_CFG_HEAP_SIZE  1024

struct mr_heap_block
{
    struct mr_heap_block *next; /**< Point to next block */
    uint32_t size: 31;          /**< Size of this block */
    uint32_t allocated: 1;      /**< Allocated flag */
};

void mr_heap_init(void);
void* mr_malloc(size_t size);
void mr_free(void* memory);
size_t mr_malloc_usable_size(void* memory);
void* mr_calloc(size_t num, size_t size);
void* mr_realloc(void* memory, size_t size);
    /* clang-format on */

#ifdef __cplusplus
}
#endif

#endif
