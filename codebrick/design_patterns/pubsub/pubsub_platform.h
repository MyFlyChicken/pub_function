/**
 * @file pubsub_platform.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2025-05-14
 * 
 * @copyright Copyright (c) 2025 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2025-05-14 <td>v1.0     <td>Yu     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#ifndef __pubsub_platform_H_ //shift+U转换为大写
#define __pubsub_platform_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief 平台配置结构体
 */
    typedef struct
    {
        void* mutex;                  /**< 平台互斥锁句柄 */
        void* (*malloc)(size_t);      /**< 内存分配函数 */
        void (*free)(void*);          /**< 内存释放函数 */
        void* (*mutex_create)(void);  /**< 创建互斥锁函数 */
        void (*mutex_lock)(void*);    /**< 加锁函数 */
        void (*mutex_unlock)(void*);  /**< 解锁函数 */
        void (*mutex_destroy)(void*); /**< 销毁互斥锁函数 */
    } PubSubPlatformConfig_t;

    /**
 * @brief 初始化平台配置
 * @param config 平台配置结构体指针
 * @return bool 是否初始化成功
 */
    bool pubsub_platform_init(void);

    /**
 * @brief 获取当前平台配置
 * @return PubSubPlatformConfig_t* 平台配置结构体指针
 */
    PubSubPlatformConfig_t* pubsub_platform_get_config(void);

    /**
 * @brief 设置平台配置
 * @param config 平台配置结构体指针
 * @return bool 是否设置成功
 */
    bool pubsub_platform_set_config(const PubSubPlatformConfig_t* config);

    /**
 * @brief 平台内存分配
 * @param size 分配大小
 * @return void* 分配的内存指针
 */
    void* pubsub_platform_malloc(size_t size);

    /**
 * @brief 平台内存释放
 * @param ptr 要释放的内存指针
 */
    void pubsub_platform_free(void* ptr);

    /**
 * @brief 平台互斥锁加锁
 * @param mutex 互斥锁句柄
 */
    void pubsub_platform_mutex_lock(void* mutex);

    /**
 * @brief 平台互斥锁解锁
 * @param mutex 互斥锁句柄
 */
    void pubsub_platform_mutex_unlock(void* mutex);

    /**
 * @brief 平台互斥锁创建
 * @return void* 互斥锁句柄
 */
    void* pubsub_platform_mutex_create(void);

    /**
 * @brief 平台互斥锁销毁
 * @param mutex 互斥锁句柄
 */
    void pubsub_platform_mutex_destroy(void* mutex);

    /**
 * @brief 反初始化平台配置
 */
    void pubsub_platform_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
