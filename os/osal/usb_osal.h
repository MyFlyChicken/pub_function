/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __OSAL_H__
#define __OSAL_H__

#include <stdint.h>
#include <string.h>

typedef void *osal_thread_t;
typedef void *osal_sem_t;
typedef void *osal_mutex_t;
typedef void *osal_mq_t;
typedef void (*thread_entry_t)(void *argument);

osal_thread_t osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, thread_entry_t entry, void *args);

osal_sem_t osal_sem_create(uint32_t initial_count);
void osal_sem_delete(osal_sem_t sem);
int osal_sem_take(osal_sem_t sem, uint32_t timeout);
int osal_sem_give(osal_sem_t sem);

osal_mutex_t osal_mutex_create(void);
void osal_mutex_delete(osal_mutex_t mutex);
int osal_mutex_take(osal_mutex_t mutex);
int osal_mutex_give(osal_mutex_t mutex);

osal_mq_t osal_mq_create(uint32_t max_msgs);
int osal_mq_send(osal_mq_t mq, uintptr_t addr);
int osal_mq_recv(osal_mq_t mq, uintptr_t *addr, uint32_t timeout);

size_t osal_enter_critical_section(void);
void osal_leave_critical_section(size_t flag);

void osal_msleep(uint32_t delay);

#endif /* osal_H */
