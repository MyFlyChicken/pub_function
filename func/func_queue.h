#ifndef __func_queue_H_ //shift+U转换为大写
#define __func_queue_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

typedef struct {
    void*          base;         /*队列容器基地址*/
    unsigned short element_size; /*队列元素大小，即每次put元素是几个字节*/
    unsigned short size;         /*队列容器可容纳元素个数，实际缓存的元素个数为(size-1)*/
    unsigned short front;        /*队首,每次取数据时，都是在队首取出。其实是数据基于base的偏移*/
    unsigned short rear;         /*队尾,每次压入数据时，都是在队尾进行压入。其实是数据基于base的偏移*/
} func_queue_t;

bool func_queue_create(func_queue_t* q, void* container, int size, int element_size);
bool func_queue_peek(func_queue_t* q, void** element);
bool func_queue_put(func_queue_t* q, void* element);
bool func_queue_get(func_queue_t* q, void* element);
bool func_queue_del(func_queue_t* q);
void func_queue_clr(func_queue_t* q);
int  func_queue_size(func_queue_t* q);

#ifdef __cplusplus
}
#endif

#endif
