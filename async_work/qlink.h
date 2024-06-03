#ifndef __qlink_H_ //shift+U转换为大写
#define __qlink_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* 计算MEMBER在TYPE内的便宜 */
#define qlink_offsetof(TYPE, MEMBER) ((size_t) & ((TYPE*)0)->MEMBER)
/* 根据ptr与type的成员member，计算结构体type的指针 */
#define qlink_container_of(ptr, type, member) ( \
    (type*)((char*)(ptr)-offsetof(type, member)))
/*链式队列结点 ---------------------------------------------------------------*/
struct qlink_node {
    struct qlink_node* next;
};

/*链式队列管理器 -------------------------------------------------------------*/
struct qlink {
    unsigned int       count;
    struct qlink_node *front, *rear;
};

/*******************************************************************************
 * @brief       初始化链式队列
 * @param[in]   q     - 队列管理器
 * @return      none
 ******************************************************************************/
static inline void qlink_init(struct qlink* q)
{
    q->front = q->rear = NULL;
    q->count           = 0;
}

/*******************************************************************************
 * @brief       入队操作
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline void qlink_put(struct qlink* q, struct qlink_node* n)
{
    if (q->count == 0)
        q->front = n;
    else
        q->rear->next = n;
    q->rear = n;
    n->next = NULL;
    q->count++;
}

/*******************************************************************************
 * @brief       预出队操作(获取队首结点)
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline struct qlink_node* qlink_peek(struct qlink* q)
{
    return q->front;
}

/*******************************************************************************
 * @brief       出队操作
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline struct qlink_node* qlink_get(struct qlink* q)
{
    struct qlink_node* n;
    if (q->count == 0)
        return NULL;
    n        = q->front;
    q->front = q->front->next;
    q->count--;
    return n;
}

/*******************************************************************************
 * @brief       队列元素个数
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline int qlink_count(struct qlink* q)
{
    return q->count;
}

#ifdef __cplusplus
}
#endif

#endif
