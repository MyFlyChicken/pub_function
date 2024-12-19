/**
 * @file algo_search.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-12-13
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-12-13 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "algo_search.h"

#define _BINARY_SEARCH_LCRC(src)    \
                                    \
    uint32_t m;                     \
    uint32_t i = 0, j = len - 1;    \
    tmp = src;                      \
    while (i <= j) {                \
        m = i + (j - i) / 2;        \
        if (tmp[m] < target) {      \
            i = m + 1;              \
        }                           \
        else if (tmp[m] > target) { \
            j = m - 1;              \
        }                           \
        else {                      \
            return m;               \
        }                           \
    }                               \
    return -1;

#define _BINARY_SEARCH_LCRO(src)    \
                                    \
    uint32_t m;                     \
    int      i = 0, j = len;        \
    tmp = src;                      \
    while (i < j) {                 \
        m = i + (j - i) / 2;        \
        if (tmp[m] < target) {      \
            i = m + 1;              \
        }                           \
        else if (tmp[m] > target) { \
            j = m;                  \
        }                           \
        else {                      \
            return m;               \
        }                           \
    }                               \
                                    \
    return -1;

/**
 * @brief 二分查找，双闭区间。即左右边界均包含自身，时间复杂度为log2n
 * @param [in] src 查找的数据源，数据源必须已经排序完毕，从大到小或从小到大
 * @param [in] len 数据长度，如查找a[6]，则len为6
 * @param [in] target 查找数据源内的指定元素
 * @param [in] type 数据源的类型
 * @return uint32_t 查找到的索引 ，-1表示未找到
 * 
 * @details 返回的uint32_t为索引
 */
uint32_t algo_binarysearch_lcrc(void* src, uint32_t len, uint32_t target, uint8_t type)
{
    // uint32_t m;
    // // 初始化双闭区间 [0, n-1] ，即 i, j 分别指向数组首元素、尾元素
    // uint32_t i = 0, j = len - 1;
    // // 循环，当搜索区间为空时跳出（当 i > j 时为空）
    // while (i <= j) {
    //     m = i + (j - i) / 2;                // 计算中点索引 m
    //     if (((uint32_t*)src)[m] < target) { // 此情况说明 target 在区间 [m+1, j] 中
    //         i = m + 1;
    //     }
    //     else if (((uint32_t*)src)[m] > target) { // 此情况说明 target 在区间 [i, m-1] 中
    //         j = m - 1;
    //     }
    //     else { // 找到目标元素，返回其索引
    //         return m;
    //     }
    // }
    // // 未找到目标元素，返回 -1
    // return -1;
    switch (type) {
    case _ALGO_SEARCH_I8: {
        int8_t* tmp;
        _BINARY_SEARCH_LCRC(src);
    }
    case _ALGO_SEARCH_U8: {
        uint8_t* tmp;
        _BINARY_SEARCH_LCRC(src);
    }
    case _ALGO_SEARCH_I16: {
        int16_t* tmp;
        _BINARY_SEARCH_LCRC(src);
    }
    case _ALGO_SEARCH_U16: {
        uint16_t* tmp;
        _BINARY_SEARCH_LCRC(src);
    }
    case _ALGO_SEARCH_I32: {
        int32_t* tmp;
        _BINARY_SEARCH_LCRC(src);
    }
    case _ALGO_SEARCH_U32: {
        uint32_t* tmp;
        _BINARY_SEARCH_LCRC(src);
    }
    default:
        return -1;
    }
}

/**
 * @brief 二分查找，双闭区间。即左右边界均包含自身
 * @param [in] src 查找的数据源，数据源必须已经排序完毕，从大到小或从小到大
 * @param [in] len 数据长度，如查找a[6]，则len为6
 * @param [in] target 查找数据源内的指定元素
 * @param [in] type 数据源的类型
 * @return uint32_t 查找到的索引 ，-1表示未找到
 * 
 * @details 返回的uint32_t为索引
 */
uint32_t algo_binarysearch_lcro(void* src, uint32_t len, uint32_t target, uint8_t type)
{
    // uint32_t m;
    // // 初始化左闭右开区间 [0, n) ，即 i, j 分别指向数组首元素、尾元素+1
    // int i = 0, j = len;
    // // 循环，当搜索区间为空时跳出（当 i = j 时为空）
    // while (i < j) {
    //     m = i + (j - i) / 2;                // 计算中点索引 m
    //     if (((uint32_t*)src)[m] < target) { // 此情况说明 target 在区间 [m+1, j) 中
    //         i = m + 1;
    //     }
    //     else if (((uint32_t*)src)[m] > target) { // 此情况说明 target 在区间 [i, m) 中
    //         j = m;
    //     }
    //     else { // 找到目标元素，返回其索引
    //         return m;
    //     }
    // }
    // // 未找到目标元素，返回 -1
    // return -1;
    switch (type) {
    case _ALGO_SEARCH_I8: {
        int8_t* tmp;
        _BINARY_SEARCH_LCRO(src);
    }
    case _ALGO_SEARCH_U8: {
        uint8_t* tmp;
        _BINARY_SEARCH_LCRO(src);
    }
    case _ALGO_SEARCH_I16: {
        int16_t* tmp;
        _BINARY_SEARCH_LCRO(src);
    }
    case _ALGO_SEARCH_U16: {
        uint16_t* tmp;
        _BINARY_SEARCH_LCRO(src);
    }
    case _ALGO_SEARCH_I32: {
        int32_t* tmp;
        _BINARY_SEARCH_LCRO(src);
    }
    case _ALGO_SEARCH_U32: {
        uint32_t* tmp;
        _BINARY_SEARCH_LCRO(src);
    }
    default:
        return -1;
    }
}
