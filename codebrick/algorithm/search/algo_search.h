/**
 * @file algo_search.h
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
#ifndef __algo_search_H_ //shift+U转换为大写
#define __algo_search_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _ALGO_SEARCH_I8  0
#define _ALGO_SEARCH_U8  1
#define _ALGO_SEARCH_I16 2
#define _ALGO_SEARCH_U16 3
#define _ALGO_SEARCH_I32 4
#define _ALGO_SEARCH_U32 5

uint32_t algo_binarysearch_lcrc(void* src, uint32_t len, uint32_t target, uint8_t type);
uint32_t algo_binarysearch_lcro(void* src, uint32_t len, uint32_t target, uint8_t type);

#ifdef __cplusplus
}
#endif

#endif
