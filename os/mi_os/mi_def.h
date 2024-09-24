#ifndef __mi_def_H_ //shift+U转换为大写
#define __mi_def_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "mi_typedef.h"

/* 最大的线程数 */
#define THREAD_MAXS (32)
/* 空闲线程回调函数 */
#define THREAD_IDLE_CBK (1)

#define THREAD_CALLBACKS     (3)
#define IDLE_HOOK_STACK_SIZE (512)

#ifdef __cplusplus
}
#endif

#endif
