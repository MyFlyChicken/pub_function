/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "osal.h"
#include "usb_errno.h"
#include <rtthread.h>
#include <rthw.h>

osal_thread_t osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, thread_entry_t entry, void *args)
{
    rt_thread_t htask;
    htask = rt_thread_create(name, entry, args, stack_size, prio, 10);
    rt_thread_startup(htask);
    return (osal_thread_t)htask;
}

osal_sem_t osal_sem_create(uint32_t initial_count)
{
    return (osal_sem_t)rt_sem_create("usbh_sem", initial_count, RT_IPC_FLAG_FIFO);
}

void osal_sem_delete(osal_sem_t sem)
{
    rt_sem_delete((rt_sem_t)sem);
}

int osal_sem_take(osal_sem_t sem, uint32_t timeout)
{
    int ret = 0;
    rt_err_t result = RT_EOK;

    result = rt_sem_take((rt_sem_t)sem, rt_tick_from_millisecond(timeout));
    if (result == -RT_ETIMEOUT) {
        ret = -ETIMEDOUT;
    } else if (result == -RT_ERROR) {
        ret = -EINVAL;
    } else {
        ret = 0;
    }

    return (int)ret;
}

int osal_sem_give(osal_sem_t sem)
{
    return (int)rt_sem_release((rt_sem_t)sem);
}

osal_mutex_t osal_mutex_create(void)
{
    return (osal_mutex_t)rt_mutex_create("usbh_mutex", RT_IPC_FLAG_FIFO);
}

void osal_mutex_delete(osal_mutex_t mutex)
{
    rt_mutex_delete((rt_mutex_t)mutex);
}

int osal_mutex_take(osal_mutex_t mutex)
{
    return (int)rt_mutex_take((rt_mutex_t)mutex, RT_WAITING_FOREVER);
}

int osal_mutex_give(osal_mutex_t mutex)
{
    return (int)rt_mutex_release((rt_mutex_t)mutex);
}

osal_mq_t osal_mq_create(uint32_t max_msgs)
{
    return (osal_mq_t)rt_mq_create("usbh_mq", sizeof(uintptr_t), max_msgs, RT_IPC_FLAG_FIFO);
}

int osal_mq_send(osal_mq_t mq, uintptr_t addr)
{
    return rt_mq_send((rt_mq_t)mq, &addr, sizeof(uintptr_t));
}

int osal_mq_recv(osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    int ret = 0;
    rt_err_t result = RT_EOK;

    result = rt_mq_recv((rt_mq_t)mq, addr, sizeof(uintptr_t), rt_tick_from_millisecond(timeout));
    if (result == -RT_ETIMEOUT) {
        ret = -ETIMEDOUT;
    } else if (result == -RT_ERROR) {
        ret = -EINVAL;
    } else {
        ret = 0;
    }

    return (int)ret;
}

size_t osal_enter_critical_section(void)
{
    return rt_hw_interrupt_disable();
}

void osal_leave_critical_section(size_t flag)
{
    rt_hw_interrupt_enable(flag);
}

void osal_msleep(uint32_t delay)
{
    rt_thread_mdelay(delay);
}
