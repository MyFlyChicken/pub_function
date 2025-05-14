/**
 * @file pubsub.h
 * @author yuyf ()
 * @brief 发布-订阅模式的头文件
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
#ifndef __pubsub_H_ //shift+U转换为大写
#define __pubsub_H_

#include <stdint.h>
#include <stdbool.h>
#include "pubsub_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <pthread.h>

    /* 订阅者节点 */
    typedef struct SubscriberNode
    {
        void (*callback)(const char*, void*, uint32_t);
        struct SubscriberNode* next;
    } SubscriberNode;

    /* 主题节点 */
    typedef struct TopicNode
    {
        char* name;
        SubscriberNode* subscribers;
        struct TopicNode* next;
    } TopicNode;

    /* 发布订阅管理器 */
    typedef struct
    {
        TopicNode* topics;
        void* mutex;
    } PubSubManager;

    // 初始化/销毁接口
    PubSubManager* pubsub_create(void);
    void pubsub_destroy(PubSubManager* ps);

    // 订阅
    int pubsub_subscribe(PubSubManager* ps, const char* topic, void (*callback)(const char*, void*, uint32_t));
    // 发布
    void pubsub_publish(PubSubManager* ps, const char* topic, void* data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
