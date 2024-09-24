/**
 * @file mi_os.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-07-05
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-07-05 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "mi_os.h"
#include "mi_port.h"
#include "mi_typedef.h"

#include "../mf_log.h"

static mi_u32_t _thread_ready;   //bit置1表示对应位的线程就绪，可以进行执行
static mi_u32_t _thread_delayed; //bit置1表示对应位的线程进行了延时

static idle_hook _idle_hook[THREAD_CALLBACKS] = {0};

static mi_thread_t _thread[THREAD_MAXS];

mi_thread_t _thread_curr;
mi_thread_t _thread_next;

//线程创建函数
mi_bool mi_thread_creat(mi_thread_t thread,
                        const char* name,
                        void (*entry)(void* parameter),
                        void*       parameter,
                        char* const stack,
                        mi_u32_t    stack_size,
                        mi_u8_t     priority)
{
    MF_ASSERT(priority < THREAD_MAXS);

    _thread[priority] = thread;

    _thread[priority]->name       = name;
    _thread[priority]->prio       = priority;
    _thread[priority]->stack      = stack;
    _thread[priority]->entry      = entry;
    _thread[priority]->stack_size = stack_size;

    _thread[priority]->timeout = 0;

    return mi_true;
}

//线程启动
mi_bool mi_thread_start(mi_thread_t thread)
{
    mi_u32_t* sp;
    mi_u32_t* stk_limit;

    /* round down the stack top to the 8-byte boundary
    * NOTE: ARM Cortex-M stack grows down from hi -> low memory
    */
    thread->sp = (mi_u32_t*)((((mi_u32_t)thread->stack + thread->stack_size) / 8) * 8);
    sp         = (mi_u32_t*)thread->sp;
    /* priority must be in ragne
    * and the priority level must be unused
    */
    MF_ASSERT(thread->prio < THREAD_MAXS);

    *(--sp) = (1U << 24);              /* xPSR */
    *(--sp) = (mi_u32_t)thread->entry; /* PC */
    *(--sp) = 0x0000000EU;             /* LR  */
    *(--sp) = 0x0000000CU;             /* R12 */
    *(--sp) = 0x00000003U;             /* R3  */
    *(--sp) = 0x00000002U;             /* R2  */
    *(--sp) = 0x00000001U;             /* R1  */
    *(--sp) = 0x00000000U;             /* R0  */
    /* additionally, fake registers R4-R11 */
    *(--sp) = 0x0000000BU; /* R11 */
    *(--sp) = 0x0000000AU; /* R10 */
    *(--sp) = 0x00000009U; /* R9 */
    *(--sp) = 0x00000008U; /* R8 */
    *(--sp) = 0x00000007U; /* R7 */
    *(--sp) = 0x00000006U; /* R6 */
    *(--sp) = 0x00000005U; /* R5 */
    *(--sp) = 0x00000004U; /* R4 */

    /* save the top of the stack in the thread's attibute */
    thread->sp = sp;

    /* round up the bottom of the stack to the 8-byte boundary */
    stk_limit = (mi_u32_t*)(((((mi_u32_t)thread->stack - 1U) / 8) + 1U) * 8);

    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }

    /* make the thread ready to run */
    if (thread->prio > 0U) {
        _thread_ready |= (1U << (thread->prio - 1U));
    }
}

//开启调度
void mi_thread_sched(void)
{
    /* choose the next thread to execute... */
    mi_thread_t next;

    if (_thread_ready == 0U) { /* idle condition? */
        next = _thread[0];     /* the idle thread */
    }
    else {
        next = _thread[LOG2(_thread_ready)];
        MF_ASSERT(next != (mi_thread_t)0);
    }

    /* trigger PendSV, if needed */
    if (next != _thread_curr) {
        _thread_next                    = next;
        *(mi_u32_t volatile*)0xE000ED04 = (1U << 28);
    }
}

//空闲线程
void mi_thread_idle(void* parameter)
{
    mi_u8_t i = 0;

    for (i = 0; i < THREAD_CALLBACKS; i++) {
        if ((NULL != _idle_hook[i])) {
            _idle_hook[i]();
        }
    }
}

//空闲线程回调函数
void mi_idle_hook_regist(idle_hook hook)
{
    mi_u8_t i = 0;

    for (i = 0; i < THREAD_CALLBACKS; i++) {
        if ((NULL != _idle_hook[i])) {
            _idle_hook[i] = hook;
        }
    }
}

//系统开始运行
void os_run(void)
{
    static char             idle_stack[IDLE_HOOK_STACK_SIZE];
    static struct mi_thread thread;

    __disable_irq();
    mi_thread_creat(&thread, "idle", mi_thread_idle, 0, idle_stack, sizeof(idle_stack), 0);
    mi_thread_start(&thread);
    __enable_irq();
}

//系统时基
void mi_os_tick(void)
{
    mi_u32_t workingSet = _thread_delayed;
    while (workingSet != 0U) {
        mi_u32_t    bit;
        mi_thread_t t = _thread[LOG2(workingSet)];

        MF_ASSERT((t != (mi_thread_t)0) && (t->timeout != 0U));

        bit = (1U << (t->prio - 1U));
        --t->timeout;
        if (t->timeout == 0U) {
            _thread_ready   |= bit;  /* insert to set */
            _thread_delayed &= ~bit; /* remove from set */
        }
        workingSet &= ~bit; /* remove from working set */
    }
}

//延时函数，让出CPU使用权
void mi_delay(mi_u32_t ticks)
{
    mi_u32_t bit;

    __disable_irq();

    /* never call OS_delay from the idleThread */
    MF_ASSERT(_thread_curr != _thread[0]);

    _thread_curr->timeout  = ticks;
    bit                    = (1U << (_thread_curr->prio - 1U));
    _thread_ready         &= ~bit;
    _thread_delayed       |= bit;

    mi_thread_sched();
    __enable_irq();
}

/* inline assembly syntax for IAR ARM */
void PendSV_Handler(void)
{
    __asm volatile(
        /* __disable_irq(); */
        "  CPSID         I                 \n"

        /* if (OS_curr != (OSThread *)0) { */
        "  LDR           r1,=OS_curr       \n"
        "  LDR           r1,[r1,#0x00]     \n"
        "  CMP           r1,#0             \n"
        "  BEQ           PendSV_restore    \n"

    /*     push registers r4-r11 on the stack */
#if (__ARM_ARCH == 6)                         // if ARMv6-M...
        "  SUB           sp,sp,#(8*4)     \n" // make room for 8 registers r4-r11
        "  MOV           r0,sp            \n" // r0 := temporary stack pointer
        "  STMIA         r0!,{r4-r7}      \n" // save the low registers
        "  MOV           r4,r8            \n" // move the high registers to low registers...
        "  MOV           r5,r9            \n"
        "  MOV           r6,r10           \n"
        "  MOV           r7,r11           \n"
        "  STMIA         r0!,{r4-r7}      \n" // save the high registers
#else                                         // ARMv7-M or higher
        "  PUSH          {r4-r11}          \n"
#endif                                        // ARMv7-M or higher

        /*     OS_curr->sp = sp; */
        "  LDR           r1,=OS_curr       \n"
        "  LDR           r1,[r1,#0x00]     \n"
        "  MOV           r0,sp             \n"
        "  STR           r0,[r1,#0x00]     \n"
        /* } */

        "PendSV_restore:                   \n"
        /* sp = OS_next->sp; */
        "  LDR           r1,=OS_next       \n"
        "  LDR           r1,[r1,#0x00]     \n"
        "  LDR           r0,[r1,#0x00]     \n"
        "  MOV           sp,r0             \n"

        /* OS_curr = OS_next; */
        "  LDR           r1,=OS_next       \n"
        "  LDR           r1,[r1,#0x00]     \n"
        "  LDR           r2,=OS_curr       \n"
        "  STR           r1,[r2,#0x00]     \n"

    /* pop registers r4-r11 */
#if (__ARM_ARCH == 6)                          // if ARMv6-M...
        "  MOV           r0,sp             \n" // r0 := top of stack
        "  MOV           r2,r0             \n"
        "  ADDS          r2,r2,#(4*4)      \n" // point r2 to the 4 high registers r7-r11
        "  LDMIA         r2!,{r4-r7}       \n" // pop the 4 high registers into low registers
        "  MOV           r8,r4             \n" // move low registers into high registers
        "  MOV           r9,r5             \n"
        "  MOV           r10,r6            \n"
        "  MOV           r11,r7            \n"
        "  LDMIA         r0!,{r4-r7}       \n" // pop the low registers
        "  ADD           sp,sp,#(8*4)      \n" // remove 8 registers from the stack
#else                                          // ARMv7-M or higher
        "  POP           {r4-r11}          \n"
#endif                                         // ARMv7-M or higher

        /* __enable_irq(); */
        "  CPSIE         I                 \n"

        /* return to the next thread */
        "  BX            lr                \n");
}
