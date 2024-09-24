#include "osal.h"
#include "usb_errno.h"

#include <cstdlib>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <pthread.h>
#include <semaphore.h>


osal_thread_t osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, thread_entry_t entry, void *args)
{    
    //TODO 

    // pthread_t thread;
    // pthread_attr_t attr;

    // // 初始化线程属性
    // pthread_attr_init(&attr);

    // // 设置栈大小
    // pthread_attr_setstacksize(&attr, stack_size);

    // // 创建线程
    // pthread_create(&thread, &attr, entry, NULL);
    // return task_handle;
}

osal_sem_t osal_sem_create(uint32_t initial_count)
{
    sem_t * sem = malloc(sizeof(sem_t));
    sem_init(sem, 0, 1);    
    return (osal_sem_t)sem;
}

void osal_sem_delete(osal_sem_t sem)
{
    sem_destroy((sem_t *)sem); 
    free(sem);   
}

int osal_sem_take(osal_sem_t sem, uint32_t timeout)
{
    (void)timeout;
    return sem_wait((sem_t *)sem);
}

int osal_sem_give(osal_sem_t sem)
{
    return sem_post((sem_t *)sem);
}

osal_mutex_t osal_mutex_create(void)
{
    pthread_mutex_t * mutex = malloc(sizeof(pthread_mutex_t));

    pthread_mutex_init(mutex, NULL); 

    return (osal_mutex_t)mutex;
}

void osal_mutex_delete(osal_mutex_t mutex)
{
    pthread_mutex_destroy(((pthread_mutex_t *)mutex));
}

int osal_mutex_take(osal_mutex_t mutex)
{
    return pthread_mutex_lock(((pthread_mutex_t *)mutex));
}

int osal_mutex_give(osal_mutex_t mutex)
{
    return pthread_mutex_unlock(((pthread_mutex_t *)mutex));
}

osal_mq_t osal_mq_create(uint32_t max_msgs)
{
    // TODO
}

int osal_mq_send(osal_mq_t mq, uintptr_t addr)
{
    // TODO
}

int osal_mq_recv(osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    // TODO
}

size_t osal_enter_critical_section(void)
{
    // TODO
}

void osal_leave_critical_section(size_t flag)
{
   // TODO
}

void osal_msleep(uint32_t delay)
{
    uint32_t us;

    if (delay < 4294967) {
        us = delay*1000;
    }
    else {
        usleep(4294967000);
        us = (delay - 4294967)*1000;        
    }  
    usleep(us);     
}
