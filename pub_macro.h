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

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE*)0)->MEMBER)

#if !defined(__GNUC__)
#define container_of(ptr, type, member) ( \
    (type*)((char*)(ptr)-offsetof(type, member)))
#else
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) ); })
#endif

/**
 * @brief 强制转换宏
 * 
 */
#define CONV2INT8(val)   (int8_t(val))
#define CONV2UINT8(val)  (uint8_t(val))
#define CONV2INT16(val)  (int16_t(val))
#define CONV2UINT16(val) (uint16_t(val))
#define CONV2INT32(val)  (int32_t(val))
#define CONV2UINT32(val) (uint32_t(val))

#define CONV2INT8_P(val)   ((int8_t*)(val))
#define CONV2UINT8_P(val)  ((uint8_t*)(val))
#define CONV2INT16_P(val)  ((int16_t*)(val))
#define CONV2UINT16_P(val) ((uint16_t*)(val))
#define CONV2INT32_P(val)  ((int32_t*)(val))
#define CONV2UINT32_P(val) ((uint32_t*)(val))

/**
 * @brief 转换数字到布尔类型
 * 
 */
#define VALUE2BOOL(x) (!!x)

/**
 * @brief 简易数学计算
 * 
 */
#define MATH_ABS(val)  ((val > 0) ? val : (0 - val))
#define MATH_MAX(A, B) (A > B ? A : B)
#define MATH_MIN(A, B) (A < B ? A : B)

/**
 * @brief 位操作
 * 
 */
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U << (x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
#define GET_BITS(regval, start, end) (((regval) & BITS((start), (end))) >> (start))

/**
 * @brief 获取结构体成员偏移
 * 
 */
#define GET_STRUCT_MEMBER_OFFSET(type, member)       (uint32_t)(&(((type*)0)->member))
#define GET_STRUCT_MEMBER_FR_ADDR(ptr, type, member) ((type*)((char*)(ptr)-GET_OFFSET(type, member)))

/**
 * @brief 获取数组大小
 * 
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) \
    ((int)((sizeof(array) / sizeof((array)[0]))))
#endif

/**
 * @brief TODO 高低位交换
 * 
 */
#ifndef BSWAP16
#define BSWAP16(u16) (__builtin_bswap16(u16))
#endif
#ifndef BSWAP32
#define BSWAP32(u32) (__builtin_bswap32(u32))
#endif

/**
 * @brief 数据拼接，field为指针
 * 
 */
#define GET_BE16(field) \
    (((uint16_t)(field)[0] << 8) | ((uint16_t)(field)[1]))

#define GET_BE32(field) \
    (((uint32_t)(field)[0] << 24) | ((uint32_t)(field)[1] << 16) | ((uint32_t)(field)[2] << 8) | ((uint32_t)(field)[3] << 0))

#define SET_BE16(field, value)                \
    do {                                      \
        (field)[0] = (uint8_t)((value) >> 8); \
        (field)[1] = (uint8_t)((value) >> 0); \
    } while (0)

#define SET_BE24(field, value)                 \
    do {                                       \
        (field)[0] = (uint8_t)((value) >> 16); \
        (field)[1] = (uint8_t)((value) >> 8);  \
        (field)[2] = (uint8_t)((value) >> 0);  \
    } while (0)

#define SET_BE32(field, value)                 \
    do {                                       \
        (field)[0] = (uint8_t)((value) >> 24); \
        (field)[1] = (uint8_t)((value) >> 16); \
        (field)[2] = (uint8_t)((value) >> 8);  \
        (field)[3] = (uint8_t)((value) >> 0);  \
    } while (0)

/**
 * ************************************************************************
 * @brief 参数校验
 * 
 * 
 * ************************************************************************
 */
#define ASSET_ARG_UPLIMIT(arg, uplimit) \
    do {                                \
        if (arg >= uplimit) {           \
        }                               \
        else {                          \
            return;                     \
        }                               \
    } while (0);

#define ASSET_ARG_DOWNLIMIT(arg, downlimit) \
    do {                                    \
        if (arg <= downlimit) {             \
        }                                   \
        else {                              \
            return;                         \
        }                                   \
    } while (0);

#define ASSET_ARG_LIMIT(arg, up, down)        \
    do {                                      \
        if ((arg <= down) || (arg >= down)) { \
        }                                     \
        else {                                \
            return;                           \
        }                                     \
    } while (0);

#endif /* __PUB_MACRO_H__ */
