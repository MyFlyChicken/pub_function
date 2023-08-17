/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "osal.h"
#include "usb_errno.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"

osal_thread_t osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, thread_entry_t entry, void *args)
{
    TaskHandle_t htask = NULL;
    stack_size /= sizeof(StackType_t);
    xTaskCreate(entry, name, stack_size, args, prio, &htask);
    return (osal_thread_t)htask;
}

osal_sem_t osal_sem_create(uint32_t initial_count)
{
    return (osal_sem_t)xSemaphoreCreateCounting(1, initial_count);
}

void osal_sem_delete(osal_sem_t sem)
{
    vSemaphoreDelete((SemaphoreHandle_t)sem);
}

int osal_sem_take(osal_sem_t sem, uint32_t timeout)
{
    return (xSemaphoreTake((SemaphoreHandle_t)sem, pdMS_TO_TICKS(timeout)) == pdPASS) ? 0 : -ETIMEDOUT;
}

int osal_sem_give(osal_sem_t sem)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    if (xPortIsInsideInterrupt()) {
        ret = xSemaphoreGiveFromISR((SemaphoreHandle_t)sem, &xHigherPriorityTaskWoken);
        if (ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    } else {
        ret = xSemaphoreGive((SemaphoreHandle_t)sem);
    }

    return (ret == pdPASS) ? 0 : -ETIMEDOUT;
}

osal_mutex_t osal_mutex_create(void)
{
    return (osal_mutex_t)xSemaphoreCreateMutex();
}

void osal_mutex_delete(osal_mutex_t mutex)
{
    vSemaphoreDelete((SemaphoreHandle_t)mutex);
}

int osal_mutex_take(osal_mutex_t mutex)
{
    return (xSemaphoreTake((SemaphoreHandle_t)mutex, portMAX_DELAY) == pdPASS) ? 0 : -ETIMEDOUT;
}

int osal_mutex_give(osal_mutex_t mutex)
{
    return (xSemaphoreGive((SemaphoreHandle_t)mutex) == pdPASS) ? 0 : -ETIMEDOUT;
}

osal_mq_t osal_mq_create(uint32_t max_msgs)
{
    return (osal_mq_t)xQueueCreate(max_msgs, sizeof(uintptr_t));
}

int osal_mq_send(osal_mq_t mq, uintptr_t addr)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int ret;

    ret = xQueueSendFromISR((osal_mq_t)mq, &addr, &xHigherPriorityTaskWoken);
    if (ret == pdPASS) {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    return (ret == pdPASS) ? 0 : -ETIMEDOUT;
}

int osal_mq_recv(osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    return (xQueueReceive((osal_mq_t)mq, addr, timeout) == pdPASS) ? 0 : -ETIMEDOUT;
}

size_t osal_enter_critical_section(void)
{
    taskDISABLE_INTERRUPTS();
    return 1;
}

void osal_leave_critical_section(size_t flag)
{
    taskENABLE_INTERRUPTS();
}

void osal_msleep(uint32_t delay)
{
    vTaskDelay(pdMS_TO_TICKS(delay));
}
