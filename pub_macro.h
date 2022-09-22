/**
 * @file pub_macro.h
 * @author YYF (NONE)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __PUB_MACRO_H__
#define __PUB_MACRO_H__

#include <stdint.h>

/**
 * @brief 强制转换宏
 * 
 */
#define CONV2INT8(val)      (int8_t(val))
#define CONV2UINT8(val)     (uint8_t(val))
#define CONV2INT16(val)     (int16_t(val))
#define CONV2UINT16(val)    (uint16_t(val))
#define CONV2INT32(val)     (int32_t(val))
#define CONV2UINT32(val)    (uint32_t(val))

#define CONV2INT8_P(val)      ((int8_t *)(val))
#define CONV2UINT8_P(val)     ((uint8_t *)(val))
#define CONV2INT16_P(val)     ((int16_t *)(val))
#define CONV2UINT16_P(val)    ((uint16_t *)(val))
#define CONV2INT32_P(val)     ((int32_t *)(val))
#define CONV2UINT32_P(val)    ((uint32_t *)(val))

/**
 * @brief 简易数学计算
 * 
 */
#define MATH_ABS(val)	((val > 0) ? val : (0-val))
#define MAX(A, B)		(A > B? A : B)
#define MIN(A, B)		(A < B? A : B)

/**
 * @brief 位操作
 * 
 */
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end)))) 
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))

#define GET_STRUCT_MEMBER_OFFSET(type, member) (uint32_t)(&(((type *)0)->member))
#define GET_STRUCT_MEMBER_FR_ADDR(ptr, type, member) ((type *)((char *)(ptr) - GET_OFFSET(type,member)))

#endif/* __PUB_MACRO_H__ */
