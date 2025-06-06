/**
 * @file example.c
 * @brief 观察者模式使用示例
 * @version 1.0
 * @date 2024-03-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "observer.h"
#include "observer_platform.h"

/**
 * @brief 具体的观察者实现
 */
typedef struct
{
    Observer base; /**< 基类 */
    char name[32]; /**< 观察者名称 */
} ConcreteObserver;

/**
 * @brief 具体的主题实现
 */
typedef struct
{
    Subject* base; /**< 基类 */
    int value;     /**< 主题的状态 */
} ConcreteSubject;

/**
 * @brief 观察者的更新函数
 * @param self 观察者实例指针
 * @param data 更新数据
 */
static void concrete_observer_update(Observer* self, void* data)
{
    ConcreteObserver* observer = (ConcreteObserver*)self;
    int value = *(int*)data;
    printf("Observer %s received update: value = %d\n", observer->name, value);
}

static void concrete_observer_destroy(Observer* self)
{
    ConcreteObserver* observer = (ConcreteObserver*)self;
    if (self)
    {
        if (self->mutex)
        {
            observer_platform_mutex_destroy(self->mutex);
        }
        observer_platform_free(observer);
    }
}

/**
 * @brief 创建具体的观察者
 * @param name 观察者名称
 * @return ConcreteObserver* 观察者实例指针，失败返回NULL
 */
ConcreteObserver* concrete_observer_create(const char* name)
{
    int ret;
    ConcreteObserver* self = malloc(sizeof(ConcreteObserver));
    if (!self)
        return NULL;

    ret = observer_init(&self->base, concrete_observer_update);
    if (0 != ret)
    {
        free(self);
        return NULL;
    }
    // 更新默认的销毁函数
    self->base.destroy = concrete_observer_destroy;
    // 初始化具体属性
    snprintf(self->name, sizeof(self->name), "%s", name);
    return self;
}

/**
 * @brief 创建具体的主题
 * @return ConcreteSubject* 主题实例指针，失败返回NULL
 */
ConcreteSubject* concrete_subject_create(void)
{
    ConcreteSubject* self = malloc(sizeof(ConcreteSubject));
    self->base = malloc(sizeof(Subject));

    Subject* p_subject = NULL;
    if (!self)
        return NULL;

    // 初始化基类
    p_subject = subject_create(4);

    memcpy(self->base, p_subject, sizeof(Subject));
    // 初始化具体属性
    self->value = 0;

    if (p_subject)
    {
        free(p_subject);
    }

    return self;
}

/**
 * @brief 设置主题的值并通知观察者
 * @param self 主题实例指针
 * @param value 新的值
 */
void concrete_subject_set_value(ConcreteSubject* self, int value)
{
    if (!self)
        return;
    self->value = value;
    self->base->notify(self->base, &self->value);
}

/**
 * @brief 销毁具体的主题
 * @param self 主题实例指针
 */
void concrete_subject_destroy(ConcreteSubject* self)
{
    if (!self)
        return;
    subject_destroy(self->base);
    free(self);
}

/**
 * @brief 主函数
 * @return int 程序退出状态
 */
int main(void)
{
    observer_platform_init();

    // 创建主题和观察者
    ConcreteSubject* subject = concrete_subject_create();
    if (subject)
    {
        printf("Create subject Success\n");
    }
    else
    {
        printf("Create subject Failed\n");
    }
    ConcreteObserver* observer1 = concrete_observer_create("Observer1");
    ConcreteObserver* observer2 = concrete_observer_create("Observer2");
    if (!subject || !observer1 || !observer2)
    {
        printf("Failed to create objects\n");
        return 1;
    }

    // 注册观察者
    subject->base->attach(subject->base, &observer1->base);
    subject->base->attach(subject->base, &observer2->base);

    // 改变主题状态并通知观察者
    concrete_subject_set_value(subject, 42);
    concrete_subject_set_value(subject, 100);

    // 移除一个观察者
    subject->base->detach(subject->base, &observer1->base);

    // 再次改变主题状态
    concrete_subject_set_value(subject, 200);
    // 清理资源
    concrete_subject_destroy(subject);
    observer_destroy(&observer1->base);
    observer_destroy(&observer2->base);
    return 0;
}
