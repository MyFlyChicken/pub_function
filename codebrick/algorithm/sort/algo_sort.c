/**
 * @file algo_sort.c
 * @author yuyf ()
 * @brief 移植自 git@gitee.com:notrynohigh/BabyOS.git
 * @version 0.1
 * @date 2024-06-12
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-06-12 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "algo_sort.h"

#define _BUBBLE_SORT(src)                    \
    for (i = 0; i < size - 1; i++) {         \
        for (j = 0; j < size - i - 1; j++) { \
            if ((src)[j] > (src)[j + 1]) {   \
                tmp          = (src)[j];     \
                (src)[j]     = (src)[j + 1]; \
                (src)[j + 1] = tmp;          \
            }                                \
        }                                    \
    }

#define _SELECTION_SORT(src)             \
    for (i = 0; i < size - 1; i++) {     \
        k = i;                           \
        for (j = i + 1; j < size; j++) { \
            if ((src)[j] < (src)[k]) {   \
                k = j;                   \
            }                            \
        }                                \
        tmp      = (src)[k];             \
        (src)[k] = (src)[i];             \
        (src)[i] = tmp;                  \
    }

#define _INSERTION_SORT(src)                                 \
    for (i = 1; i < size; i++) {                             \
        if ((src)[i] < (src)[i - 1]) {                       \
            tmp = (src)[i];                                  \
            for (j = i - 1; j >= 0 && (src)[j] > tmp; j--) { \
                (src)[j + 1] = (src)[j];                     \
            }                                                \
            (src)[j + 1] = tmp;                              \
        }                                                    \
    }

void BubbleSort(void* arr, int size, uint8_t type)
{
    int i, j;
    switch (type) {
    case _ALGO_SORT_I8: {
        int8_t tmp;
        _BUBBLE_SORT((int8_t*)arr);
    } break;
    case _ALGO_SORT_U8: {
        uint8_t tmp;
        _BUBBLE_SORT((uint8_t*)arr);
    } break;
    case _ALGO_SORT_I16: {
        int16_t tmp;
        _BUBBLE_SORT((int16_t*)arr);
    } break;
    case _ALGO_SORT_U16: {
        uint16_t tmp;
        _BUBBLE_SORT((uint16_t*)arr);
    } break;
    case _ALGO_SORT_I32: {
        int32_t tmp;
        _BUBBLE_SORT((int32_t*)arr);
    } break;
    case _ALGO_SORT_U32: {
        uint32_t tmp;
        _BUBBLE_SORT((uint32_t*)arr);
    } break;
    }
}

void SelectionSort(void* arr, int size, uint8_t type)
{
    int i, j, k;
    switch (type) {
    case _ALGO_SORT_I8: {
        int8_t tmp;
        _SELECTION_SORT((int8_t*)arr);
    } break;
    case _ALGO_SORT_U8: {
        uint8_t tmp;
        _SELECTION_SORT((uint8_t*)arr);
    } break;
    case _ALGO_SORT_I16: {
        int16_t tmp;
        _SELECTION_SORT((int16_t*)arr);
    } break;
    case _ALGO_SORT_U16: {
        uint16_t tmp;
        _SELECTION_SORT((uint16_t*)arr);
    } break;
    case _ALGO_SORT_I32: {
        int32_t tmp;
        _SELECTION_SORT((int32_t*)arr);
    } break;
    case _ALGO_SORT_U32: {
        uint32_t tmp;
        _SELECTION_SORT((uint32_t*)arr);
    } break;
    }
}

void InsertionSort(void* arr, int size, uint8_t type)
{
    int i, j;
    switch (type) {
    case _ALGO_SORT_I8: {
        int8_t tmp;
        _INSERTION_SORT((int8_t*)arr);
    } break;
    case _ALGO_SORT_U8: {
        uint8_t tmp;
        _INSERTION_SORT((uint8_t*)arr);
    } break;
    case _ALGO_SORT_I16: {
        int16_t tmp;
        _INSERTION_SORT((int16_t*)arr);
    } break;
    case _ALGO_SORT_U16: {
        uint16_t tmp;
        _INSERTION_SORT((uint16_t*)arr);
    } break;
    case _ALGO_SORT_I32: {
        int32_t tmp;
        _INSERTION_SORT((int32_t*)arr);
    } break;
    case _ALGO_SORT_U32: {
        uint32_t tmp;
        _INSERTION_SORT((uint32_t*)arr);
    } break;
    }
}
