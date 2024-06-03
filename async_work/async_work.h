#ifndef __async_work_H_ //shift+U转换为大写
#define __async_work_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "qlink.h"
#include <stdbool.h>

/*异步作业管理器 -------------------------------------------------------------*/
typedef struct {
    struct qlink idle;  /*空闲队列*/
    struct qlink ready; /*就绪队列*/
} async_work_t;

typedef void (*async_work_func_t)(async_work_t* w, void* object, void* params);

/*异步作业结点 ---------------------------------------------------------------*/
typedef struct {
    void*             object; /*作业对象*/
    void*             params; /*作业参数*/
    async_work_func_t work;   /*作业入口*/
    struct qlink_node node;   /*链表节点*/
} work_node_t;

void async_work_init(async_work_t* w, work_node_t* nodes, int max_nodes);

bool async_work_add(async_work_t* w, void* object, void* params, async_work_func_t work);

void async_work_process(async_work_t* w);

#ifdef __cplusplus
}
#endif

#endif
