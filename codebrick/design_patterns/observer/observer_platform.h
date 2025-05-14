/**
 * @file observer_platform.h
 * @brief 观察者模式平台抽象层接口
 * @version 1.0
 * @date 2024-03-23
 */

#ifndef OBSERVER_PLATFORM_H
#define OBSERVER_PLATFORM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief 平台类型枚举
 */
    typedef enum
    {
        PLATFORM_LINUX,   /**< Linux平台 */
        PLATFORM_RTOS,    /**< RTOS平台 */
        PLATFORM_WINDOWS, /**< Windows平台 */
        PLATFORM_UNKNOWN  /**< 未知平台 */
    } PlatformType;

    /**
 * @brief 平台配置结构体
 */
    typedef struct
    {
        PlatformType type;            /**< 平台类型 */
        void* mutex;                  /**< 平台互斥锁句柄 */
        void* (*malloc)(size_t);      /**< 内存分配函数 */
        void (*free)(void*);          /**< 内存释放函数 */
        void* (*mutex_create)(void);  /**< 创建互斥锁函数 */
        void (*mutex_lock)(void*);    /**< 加锁函数 */
        void (*mutex_unlock)(void*);  /**< 解锁函数 */
        void (*mutex_destroy)(void*); /**< 销毁互斥锁函数 */
    } PlatformConfig;

    /**
 * @brief 初始化平台配置
 * @param config 平台配置结构体指针
 * @return bool 是否初始化成功
 */
    bool observer_platform_init(void);

    /**
 * @brief 获取当前平台配置
 * @return PlatformConfig* 平台配置结构体指针
 */
    PlatformConfig* observer_platform_get_config(void);

    /**
 * @brief 设置平台配置
 * @param config 平台配置结构体指针
 * @return bool 是否设置成功
 */
    bool observer_platform_set_config(const PlatformConfig* config);

    /**
 * @brief 平台内存分配
 * @param size 分配大小
 * @return void* 分配的内存指针
 */
    void* observer_platform_malloc(size_t size);

    /**
 * @brief 平台内存释放
 * @param ptr 要释放的内存指针
 */
    void observer_platform_free(void* ptr);

    /**
 * @brief 平台互斥锁加锁
 * @param mutex 互斥锁句柄
 */
    void observer_platform_mutex_lock(void* mutex);

    /**
 * @brief 平台互斥锁解锁
 * @param mutex 互斥锁句柄
 */
    void observer_platform_mutex_unlock(void* mutex);

    /**
 * @brief 平台互斥锁创建
 * @return void* 互斥锁句柄
 */
    void* observer_platform_mutex_create(void);

    /**
 * @brief 平台互斥锁销毁
 * @param mutex 互斥锁句柄
 */
    void observer_platform_mutex_destroy(void* mutex);

    /**
 * @brief 反初始化平台配置
 */
    void observer_platform_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* OBSERVER_PLATFORM_H */
