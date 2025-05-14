/**
 * @file pubsub.c
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
#include "pubsub.h"
#include "pubsub_platform.h"
#include "pubsub.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 创建管理器 */
PubSubManager* pubsub_create(void)
{
    PubSubManager* ps = pubsub_platform_malloc(sizeof(PubSubManager));
    if (!ps)
    {
        return NULL;
    }

    ps->mutex = pubsub_platform_mutex_create();
    if (!ps->mutex)
    {
        pubsub_platform_free(ps);
        return NULL;
    }

    ps->topics = NULL;

    return ps;
}

/* 销毁管理器 */
void pubsub_destroy(PubSubManager* ps)
{
    if (!ps)
        return;

    pubsub_platform_mutex_unlock(ps->mutex);
    TopicNode* curr_topic = ps->topics;
    while (curr_topic)
    {
        SubscriberNode* curr_sub = curr_topic->subscribers;
        while (curr_sub)
        {
            SubscriberNode* next_sub = curr_sub->next;
            pubsub_platform_free(curr_sub);
            curr_sub = next_sub;
        }
        TopicNode* next_topic = curr_topic->next;
        pubsub_platform_free(curr_topic->name);
        pubsub_platform_free(curr_topic);
        curr_topic = next_topic;
    }
    pubsub_platform_mutex_unlock(ps->mutex);
    pubsub_platform_mutex_destroy(ps->mutex);
    pubsub_platform_free(ps);
}

/* 查找主题节点 */
static TopicNode* find_topic(PubSubManager* ps, const char* topic)
{
    TopicNode* curr = ps->topics;
    while (curr && strcmp(curr->name, topic) != 0)
    {
        curr = curr->next;
    }
    return curr;
}

/* 订阅实现 */
int pubsub_subscribe(PubSubManager* ps, const char* topic, void (*callback)(const char*, void*, uint32_t))
{
    pubsub_platform_mutex_lock(ps->mutex);

    TopicNode* target = find_topic(ps, topic);
    if (!target)
    {
        target = malloc(sizeof(TopicNode));
        target->name = strdup(topic);
        target->subscribers = NULL;
        target->next = ps->topics;
        ps->topics = target;
    }

    SubscriberNode* new_sub = malloc(sizeof(SubscriberNode));
    new_sub->callback = callback;
    new_sub->next = target->subscribers;
    target->subscribers = new_sub;

    pubsub_platform_mutex_unlock(ps->mutex);
    return 0;
}

/* 发布实现 */
void pubsub_publish(PubSubManager* ps, const char* topic, void* data, uint32_t size)
{
    pubsub_platform_mutex_lock(ps->mutex);

    TopicNode* target = find_topic(ps, topic);
    if (target)
    {
        SubscriberNode* curr_sub = target->subscribers;
        while (curr_sub)
        {
            curr_sub->callback(topic, data, size);
            curr_sub = curr_sub->next;
        }
    }

    pubsub_platform_mutex_unlock(ps->mutex);
}
