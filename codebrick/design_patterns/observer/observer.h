/**
 * @file observer.h
 * @brief 观察者模式接口定义
 * @version 1.0
 * @date 2024-03-23
 */

#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdint.h>
#include <stdbool.h>
#include "observer_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 观察者接口结构体
 */
typedef struct Observer {
    void (*update)(struct Observer* self, void* data);  /**< 更新函数指针 */
    void (*destroy)(struct Observer* self);             /**< 销毁函数指针 */
    void* mutex;                                        /**< 互斥锁句柄 */
} Observer;

/**
 * @brief 主题接口结构体
 */
typedef struct Subject {
    Observer** observers;           /**< 观察者数组 */
    uint32_t observer_count;       /**< 当前观察者数量 */
    uint32_t observer_capacity;    /**< 观察者数组容量 */
    
    void (*attach)(struct Subject* self, Observer* observer);    /**< 添加观察者函数 */
    void (*detach)(struct Subject* self, Observer* observer);    /**< 移除观察者函数 */
    void (*notify)(struct Subject* self, void* data);            /**< 通知观察者函数 */
    void (*destroy)(struct Subject* self);                       /**< 销毁主题函数 */
} Subject;

/**
 * @brief 创建主题实例
 * @param initial_capacity 初始观察者容量
 * @return Subject* 主题实例指针，失败返回NULL
 */
Subject* subject_create(uint32_t initial_capacity);

/**
 * @brief 销毁主题实例
 * @param subject 主题实例指针
 */
void subject_destroy(Subject* subject);

/**
 * @brief 创建观察者实例
 * @param update 更新函数指针
 * @return Observer* 观察者实例指针，失败返回NULL
 */
Observer* observer_create(void (*update)(Observer* self, void* data));

/**
 * @brief 销毁观察者实例
 * @param observer 观察者实例指针
 */
void observer_destroy(Observer* observer);

/**
 * @brief 检查主题是否已满
 * @param subject 主题实例指针
 * @return bool 是否已满
 */
bool subject_is_full(const Subject* subject);

/**
 * @brief 检查主题是否为空
 * @param subject 主题实例指针
 * @return bool 是否为空
 */
bool subject_is_empty(const Subject* subject);

/**
 * @brief 获取主题当前观察者数量
 * @param subject 主题实例指针
 * @return uint32_t 观察者数量
 */
uint32_t subject_get_observer_count(const Subject* subject);

#ifdef __cplusplus
}
#endif

#endif /* OBSERVER_H */ 
