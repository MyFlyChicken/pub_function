#ifndef __qlink_H_ //shift+U转换为大写
#define __qlink_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* 计算MEMBER在TYPE内的偏移 */
#define qlink_offsetof(TYPE, MEMBER) ((size_t) & ((TYPE*)0)->MEMBER)
/* 根据ptr与type的成员member，计算结构体type的指针 */
#define qlink_container_of(ptr, type, member) ( \
    (type*)((char*)(ptr) - offsetof(type, member)))
/*链式队列结点 ---------------------------------------------------------------*/
struct qlink_node {
    struct qlink_node* next;
};

/*链式队列管理器 -------------------------------------------------------------*/
struct qlink {
    unsigned int       count;
    struct qlink_node *front, *rear; //front队首  rear队尾
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
 * @details     这个链表是一个单向链表，front与rear只是用于管理链表的第一个数据和最后一个数据
 ******************************************************************************/
static inline void qlink_put(struct qlink* q, struct qlink_node* n)
{
    if (q->count == 0)
        q->front = n; //count为0时，同时将front rear赋值为n
    else
        q->rear->next = n;
    q->rear = n;    //rear永远是最后的指针
    n->next = NULL; //rear永远没有数据
    q->count++;     //链表计数
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
    n        = q->front;       //根据qlink_put逻辑，front永远为最先压入的链表
    q->front = q->front->next; //更新front
    q->count--;                //计数链表
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
