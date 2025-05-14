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

int main()
{
    pubsub_platform_init();

    PubSubManager* ps = pubsub_create();

    // 订阅主题
    pubsub_subscribe(ps, "system/log", log_callback);
    pubsub_subscribe(ps, "system/rcd", rcd_callback);

    // 发布消息
    pubsub_publish(ps, "system/log", "Application started", 0);
    pubsub_publish(ps, "system/rcd", "hello tr1", 0);
    pubsub_publish(ps, "system/log", "Application started111", 0);
    pubsub_publish(ps, "system/rcd", "hello tr1", 0);
    pubsub_publish(ps, "system/log", "Application started333", 0);
    pubsub_publish(ps, "system/rcd", "hello tr1", 0);
    // 清理资源
    pubsub_destroy(ps);
    return 0;
}
