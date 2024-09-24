#ifndef __algo_sort_H_ //shift+U转换为大写
#define __algo_sort_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define _ALGO_SORT_I8  0
#define _ALGO_SORT_U8  1
#define _ALGO_SORT_I16 2
#define _ALGO_SORT_U16 3
#define _ALGO_SORT_I32 4
#define _ALGO_SORT_U32 5

void BubbleSort(void* arr, int size, uint8_t type);
void SelectionSort(void* arr, int size, uint8_t type);
void InsertionSort(void* arr, int size, uint8_t type);

#ifdef __cplusplus
}
#endif

#endif
