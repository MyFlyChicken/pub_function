#ifndef __mi_os_H_ //shift+U转换为大写
#define __mi_os_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "mi_def.h"
#include "mi_port.h"
#include "mi_typedef.h"

struct mi_thread {
    const char* name;
    void*       sp;         /* stack pointer */
    char*       stack;      /* stack  */
    mi_u32_t    stack_size; /* stack size */
    mi_u32_t    timeout;    /* timeout delay down-counter */
    mi_u8_t     prio;       /* thread priority */

    void (*entry)(void* parameter);
};
typedef struct mi_thread* mi_thread_t;

mi_bool mi_thread_creat(mi_thread_t thread,
                        const char* name,
                        void (*entry)(void* parameter),
                        void*    parameter,
                        char*    stack,
                        mi_u32_t stack_size,
                        mi_u8_t  priority);

void    mi_idle_hook_regist(idle_hook hook);
mi_bool mi_thread_start(mi_thread_t thread);

void mi_delay(mi_u32_t ticks);

//定时器中断周期调用
void mi_os_tick(void);

#ifdef __cplusplus
}
#endif

#endif
