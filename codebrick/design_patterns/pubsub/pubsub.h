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

    typedef enum
    {
        TOPIC_TYPE_NAME = 0,
        TOPIC_TYPE_ID,
    } TOPIC_TYPE_E;

    /* 订阅者节点 */
    typedef struct SubscriberNode
    {
        union
        {
            void (*cb_name)(const char*, void*, uint32_t);
            void (*cb_id)(uint32_t, void*, uint32_t);
        } callback;
        uint8_t priority; // 优先级，值越大优先级越高

        struct SubscriberNode* next;
    } SubscriberNode;

    /* 主题节点 */
    typedef struct TopicNode
    {
        union
        {
            uint32_t id;
            const char* name;
        } topic;
        TOPIC_TYPE_E type; // 程序根据类型选择主题的内容（id or name）
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

    // 发布
    void pubsub_publish_name(PubSubManager* ps, const char* topic, void* data, uint32_t size);
    // 订阅
    int pubsub_subscribe_name(PubSubManager* ps, const char* topic, void (*callback)(const char*, void*, uint32_t), uint8_t priority);

    // 发布
    void pubsub_publish_id(PubSubManager* ps, uint32_t topic, void* data, uint32_t size);
    // 订阅
    int pubsub_subscribe_id(PubSubManager* ps, uint32_t topic, void (*callback)(uint32_t, void*, uint32_t), uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif
