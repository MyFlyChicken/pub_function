/**
 * @file observer.c
 * @brief 观察者模式实现
 * @version 1.0
 * @date 2024-03-23
 */

#include "observer.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief 主题实现 - 添加观察者
 * @param self 主题实例指针
 * @param observer 观察者实例指针
 */
static void subject_attach(Subject* self, Observer* observer)
{
    if (!self || !observer)
        return;

    // 加锁保护
    observer_platform_mutex_lock(self->mutex);

    if (self->observer_count >= self->observer_capacity)
    {
        uint32_t new_capacity = self->observer_capacity * 2;
        Observer** new_observers = observer_platform_malloc(new_capacity * sizeof(Observer*));
        if (!new_observers)
        {
            observer_platform_mutex_unlock(self->mutex);
            return;
        }

        memcpy(new_observers, self->observers, self->observer_count * sizeof(Observer*));
        observer_platform_free(self->observers);

        self->observers = new_observers;
        self->observer_capacity = new_capacity;
    }

    self->observers[self->observer_count++] = observer;

    // 解锁
    observer_platform_mutex_unlock(self->mutex);
}

/**
 * @brief 主题实现 - 移除观察者
 * @param self 主题实例指针
 * @param observer 观察者实例指针
 */
static void subject_detach(Subject* self, Observer* observer)
{
    if (!self || !observer)
        return;

    // 加锁保护
    observer_platform_mutex_lock(self->mutex);

    for (uint32_t i = 0; i < self->observer_count; i++)
    {
        if (self->observers[i] == observer)
        {
            // 移动最后一个观察者到当前位置
            if (i < self->observer_count - 1)
            {
                self->observers[i] = self->observers[self->observer_count - 1];
            }
            self->observer_count--;
            break;
        }
    }

    // 解锁
    observer_platform_mutex_unlock(self->mutex);
}

/**
 * @brief 主题实现 - 通知观察者
 * @param self 主题实例指针
 * @param data 通知数据
 */
static void subject_notify(Subject* self, void* data)
{
    if (!self)
        return;

    // 加锁保护
    observer_platform_mutex_lock(self->mutex);

    for (uint32_t i = 0; i < self->observer_count; i++)
    {
        if (self->observers[i] && self->observers[i]->update)
        {
            // 解锁后再调用update，避免死锁
            observer_platform_mutex_unlock(self->mutex);
            self->observers[i]->update(self->observers[i], data);
            observer_platform_mutex_lock(self->mutex);
        }
    }

    // 解锁
    observer_platform_mutex_unlock(self->mutex);
}

/**
 * @brief 主题实现 - 销毁主题
 * @param self 主题实例指针
 */
static void subject_destroy_impl(Subject* self)
{
    if (!self)
        return;

    if (self->mutex)
    {
        observer_platform_mutex_destroy(self->mutex);
    }

    if (self->observers)
    {
        observer_platform_free(self->observers);
    }

    observer_platform_free(self);
}

Subject* subject_create(uint32_t initial_capacity)
{
    Subject* self = observer_platform_malloc(sizeof(Subject));
    if (!self)
        return NULL;
    self->observer_capacity = initial_capacity > 0 ? initial_capacity : 4;
    self->observer_count = 0;
    self->observers = observer_platform_malloc(self->observer_capacity * sizeof(Observer*));
    if (!self->observers)
    {
        observer_platform_free(self);
        return NULL;
    }

    // 创建互斥锁
    self->mutex = observer_platform_mutex_create();
    if (!self->mutex)
    {
        observer_platform_free(self->observers);
        observer_platform_free(self);
        return NULL;
    }
    // 初始化函数指针
    self->attach = subject_attach;
    self->detach = subject_detach;
    self->notify = subject_notify;
    self->destroy = subject_destroy_impl;

    return self;
}

void subject_destroy(Subject* self)
{
    if (self && self->destroy)
    {
        self->destroy(self);
    }
}

/**
 * @brief 观察者实现 - 销毁观察者
 * @param self 观察者实例指针
 */
static void observer_destroy_impl(Observer* self)
{
    if (self)
    {
        if (self->mutex)
        {
            observer_platform_mutex_destroy(self->mutex);
        }
        observer_platform_free(self);
    }
}

Observer* observer_create(void (*update)(Observer* self, void* data))
{
    Observer* self = observer_platform_malloc(sizeof(Observer));
    if (!self)
        return NULL;

    self->update = update;
    self->destroy = observer_destroy_impl;

    // 创建互斥锁
    self->mutex = observer_platform_mutex_create();
    if (!self->mutex)
    {
        observer_platform_free(self);
        return NULL;
    }

    return self;
}

int observer_init(Observer* self, void (*update)(Observer* self, void* data))
{
    if (!self)
        return -1;

    self->update = update;
    self->destroy = observer_destroy_impl;

    // 创建互斥锁
    self->mutex = observer_platform_mutex_create();
    if (!self->mutex)
    {
        return -1;
    }

    return 0;
}

void observer_destroy(Observer* self)
{
    if (self && self->destroy)
    {
        self->destroy(self);
    }
}

bool subject_is_full(const Subject* subject)
{
    if (!subject)
        return false;

    observer_platform_mutex_lock((*subject->observers)->mutex);
    bool is_full = subject->observer_count >= subject->observer_capacity;
    observer_platform_mutex_unlock((*subject->observers)->mutex);

    return is_full;
}

bool subject_is_empty(const Subject* subject)
{
    if (!subject)
        return true;

    observer_platform_mutex_lock((*subject->observers)->mutex);
    bool is_empty = subject->observer_count == 0;
    observer_platform_mutex_unlock((*subject->observers)->mutex);

    return is_empty;
}

uint32_t subject_get_observer_count(const Subject* subject)
{
    if (!subject)
        return 0;

    observer_platform_mutex_lock((*subject->observers)->mutex);
    uint32_t count = subject->observer_count;
    observer_platform_mutex_unlock((*subject->observers)->mutex);

    return count;
}
