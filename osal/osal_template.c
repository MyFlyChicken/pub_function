/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "osal.h"
#include "usb_errno.h"
#include <aos/kernel.h>
#include <csi_core.h>

osal_thread_t osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, thread_entry_t entry, void *args)
{
    aos_task_t task_handle = NULL;

    aos_task_new_ext(&task_handle, name, entry, args, stack_size, prio + AOS_DEFAULT_APP_PRI - 4);

    return task_handle;
}

osal_sem_t osal_sem_create(uint32_t initial_count)
{
    aos_sem_t sem = NULL;

    aos_sem_new(&sem, initial_count);

    return sem;
}

void osal_sem_delete(osal_sem_t sem)
{
    aos_sem_free((aos_sem_t *)&sem);
}

int osal_sem_take(osal_sem_t sem, uint32_t timeout)
{
    return aos_sem_wait((aos_sem_t *)&sem, timeout);
}

int osal_sem_give(osal_sem_t sem)
{
    aos_sem_signal((aos_sem_t *)&sem);
    return 0;
}

osal_mutex_t osal_mutex_create(void)
{
    aos_mutex_t mutex = NULL;

    aos_mutex_new(&mutex);

    return (osal_mutex_t)mutex;
}

void osal_mutex_delete(osal_mutex_t mutex)
{
    aos_mutex_free(((aos_mutex_t *)&mutex));
}

int osal_mutex_take(osal_mutex_t mutex)
{
    return aos_mutex_lock(((aos_mutex_t *)&mutex), AOS_WAIT_FOREVER);
}

int osal_mutex_give(osal_mutex_t mutex)
{
    return aos_mutex_unlock(((aos_mutex_t *)&mutex));
}

osal_mq_t osal_mq_create(uint32_t max_msgs)
{
    aos_queue_t queue = NULL;

    aos_queue_create(&queue, sizeof(uintptr_t), max_msgs, 0);

    return (osal_mq_t)queue;
}

int osal_mq_send(osal_mq_t mq, uintptr_t addr)
{
    return aos_queue_send((aos_queue_t *)&mq, &addr, sizeof(uintptr_t));
}

int osal_mq_recv(osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    size_t recv_size;
    return aos_queue_recv((aos_queue_t *)&mq, timeout, addr, &recv_size);
}

size_t osal_enter_critical_section(void)
{
    return csi_irq_save();
}

void osal_leave_critical_section(size_t flag)
{
    csi_irq_restore(flag);
}

void osal_msleep(uint32_t delay)
{
    aos_msleep(delay);
}
