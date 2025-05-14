#include "pubsub.h"
#include "pubsub_platform.h"
#include <stdio.h>

// 订阅者回调
void log_callback(const char* topic, void* data, uint32_t size)
{
    (void)size;
    printf("[%s] Received: %s\n", topic, (char*)data);
}

void rcd_callback(const char* topic, void* data, uint32_t size)
{
    (void)size;
    printf("[%s] Received: %s\n", topic, (char*)data);
}

void evt_callback(uint32_t topic, void* data, uint32_t size)
{
    (void)size;
    printf("[%d] Received: %s\n", topic, (char*)data);
}

int main()
{
    TopicNode* curr;

    pubsub_platform_init();

    PubSubManager* ps = pubsub_create();

    // 订阅主题
    pubsub_subscribe_name(ps, "system/log", log_callback);
    pubsub_subscribe_id(ps, 66, evt_callback);
    pubsub_subscribe_name(ps, "system/rcd", rcd_callback);
    pubsub_subscribe_id(ps, 88, evt_callback);

    curr = ps->topics;
    while (curr)
    {
        printf("Topic type: %d\n", curr->type);
        curr = curr->next;
    }

    // 发布消息
    pubsub_publish_name(ps, "system/log", "Application started", 0);
    pubsub_publish_id(ps, 88, "hello 88", 0);
    pubsub_publish_id(ps, 66, "hello 66", 0);
    pubsub_publish_name(ps, "system/rcd", "hello tr1", 0);
    pubsub_publish_name(ps, "system/log", "Application started111", 0);
    pubsub_publish_id(ps, 88, "hello 88", 0);
    pubsub_publish_id(ps, 66, "hello 66", 0);
    pubsub_publish_name(ps, "system/rcd", "hello tr1", 0);
    pubsub_publish_name(ps, "system/log", "Application started333", 0);
    pubsub_publish_name(ps, "system/rcd", "hello tr1", 0);

    pubsub_publish_id(ps, 55, "hello 55", 0);

    // 清理资源
    pubsub_destroy(ps);
    return 0;
}
