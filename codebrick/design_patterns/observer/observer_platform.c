/**
 * @file observer_platform.c
 * @brief 观察者模式平台抽象层实现
 * @version 1.0
 * @date 2024-03-23
 */

#include "observer_platform.h"
#include <stdlib.h>
#include <string.h>

/* 默认平台配置 */
static PlatformConfig g_platform_config = {
    .type = PLATFORM_UNKNOWN,
    .mutex = NULL,
    .malloc = malloc,
    .free = free,
    .mutex_create = NULL,
    .mutex_lock = NULL,
    .mutex_unlock = NULL,
    .mutex_destroy = NULL
};

/* Linux平台实现 */
#ifdef __linux__
#include <pthread.h>

static void* linux_mutex_create(void) {
    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    if (mutex) {
        pthread_mutex_init(mutex, NULL);
    }
    return mutex;
}

static void linux_mutex_lock(void* mutex) {
    if (mutex) {
        pthread_mutex_lock((pthread_mutex_t*)mutex);
    }
}

static void linux_mutex_unlock(void* mutex) {
    if (mutex) {
        pthread_mutex_unlock((pthread_mutex_t*)mutex);
    }
}

static void linux_mutex_destroy(void* mutex) {
    if (mutex) {
        pthread_mutex_destroy((pthread_mutex_t*)mutex);
        free(mutex);
    }
}

static bool init_linux_platform(PlatformConfig* config) {
    config->type = PLATFORM_LINUX;
    config->mutex_create = linux_mutex_create;
    config->mutex_lock = linux_mutex_lock;
    config->mutex_unlock = linux_mutex_unlock;
    config->mutex_destroy = linux_mutex_destroy;
    return true;
}
#endif

/* FreeRTOS平台实现 */
#ifdef FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"

static void* freertos_mutex_create(void) {
    return xSemaphoreCreateMutex();
}

static void freertos_mutex_lock(void* mutex) {
    if (mutex) {
        xSemaphoreTake(mutex, portMAX_DELAY);
    }
}

static void freertos_mutex_unlock(void* mutex) {
    if (mutex) {
        xSemaphoreGive(mutex);
    }
}

static void freertos_mutex_destroy(void* mutex) {
    if (mutex) {
        vSemaphoreDelete(mutex);
    }
}

static bool init_freertos_platform(PlatformConfig* config) {
    config->type = PLATFORM_RTOS;
    config->mutex_create = freertos_mutex_create;
    config->mutex_lock = freertos_mutex_lock;
    config->mutex_unlock = freertos_mutex_unlock;
    config->mutex_destroy = freertos_mutex_destroy;
    return true;
}
#endif

/* RT-Thread平台实现 */
#ifdef RT_USING_RTTHREAD
#include <rtthread.h>
#include <rtdevice.h>

static void* rtthread_mutex_create(void) {
    rt_mutex_t mutex = rt_mutex_create("observer_mutex", RT_IPC_FLAG_FIFO);
    return (void*)mutex;
}

static void rtthread_mutex_lock(void* mutex) {
    if (mutex) {
        rt_mutex_take((rt_mutex_t)mutex, RT_WAITING_FOREVER);
    }
}

static void rtthread_mutex_unlock(void* mutex) {
    if (mutex) {
        rt_mutex_release((rt_mutex_t)mutex);
    }
}

static void rtthread_mutex_destroy(void* mutex) {
    if (mutex) {
        rt_mutex_delete((rt_mutex_t)mutex);
    }
}

static bool init_rtthread_platform(PlatformConfig* config) {
    config->type = PLATFORM_RTOS;
    config->mutex_create = rtthread_mutex_create;
    config->mutex_lock = rtthread_mutex_lock;
    config->mutex_unlock = rtthread_mutex_unlock;
    config->mutex_destroy = rtthread_mutex_destroy;
    return true;
}
#endif

/* μC/OS平台实现 */
#ifdef UCOS
#include "os.h"

static void* ucos_mutex_create(void) {
    OS_ERR err;
    OS_MUTEX* mutex = malloc(sizeof(OS_MUTEX));
    if (mutex) {
        OSMutexCreate(mutex, "observer_mutex", &err);
        if (err != OS_ERR_NONE) {
            free(mutex);
            return NULL;
        }
    }
    return mutex;
}

static void ucos_mutex_lock(void* mutex) {
    if (mutex) {
        OS_ERR err;
        OSMutexPend((OS_MUTEX*)mutex, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
}

static void ucos_mutex_unlock(void* mutex) {
    if (mutex) {
        OS_ERR err;
        OSMutexPost((OS_MUTEX*)mutex, OS_OPT_POST_NONE, &err);
    }
}

static void ucos_mutex_destroy(void* mutex) {
    if (mutex) {
        OS_ERR err;
        OSMutexDel((OS_MUTEX*)mutex, OS_OPT_DEL_ALWAYS, &err);
        free(mutex);
    }
}

static bool init_ucos_platform(PlatformConfig* config) {
    config->type = PLATFORM_RTOS;
    config->mutex_create = ucos_mutex_create;
    config->mutex_lock = ucos_mutex_lock;
    config->mutex_unlock = ucos_mutex_unlock;
    config->mutex_destroy = ucos_mutex_destroy;
    return true;
}
#endif

/* Windows平台实现 */
#ifdef _WIN32
#include <windows.h>

static void* windows_mutex_create(void) {
    return CreateMutex(NULL, FALSE, NULL);
}

static void windows_mutex_lock(void* mutex) {
    if (mutex) {
        WaitForSingleObject(mutex, INFINITE);
    }
}

static void windows_mutex_unlock(void* mutex) {
    if (mutex) {
        ReleaseMutex(mutex);
    }
}

static void windows_mutex_destroy(void* mutex) {
    if (mutex) {
        CloseHandle(mutex);
    }
}

static bool init_windows_platform(PlatformConfig* config) {
    config->type = PLATFORM_WINDOWS;
    config->mutex_create = windows_mutex_create;
    config->mutex_lock = windows_mutex_lock;
    config->mutex_unlock = windows_mutex_unlock;
    config->mutex_destroy = windows_mutex_destroy;
    return true;
}
#endif

bool observer_platform_init(PlatformConfig* config) {
    if (!config) return false;
    
    // 根据平台类型初始化对应的配置
    bool success = false;
    
    #ifdef __linux__
    if (config->type == PLATFORM_LINUX) {
        success = init_linux_platform(config);
    }
    #endif
    
    #ifdef FREERTOS
    if (config->type == PLATFORM_RTOS) {
        success = init_freertos_platform(config);
    }
    #endif
    
    #ifdef RT_USING_RTTHREAD
    if (config->type == PLATFORM_RTOS) {
        success = init_rtthread_platform(config);
    }
    #endif
    
    #ifdef UCOS
    if (config->type == PLATFORM_RTOS) {
        success = init_ucos_platform(config);
    }
    #endif
    
    #ifdef _WIN32
    if (config->type == PLATFORM_WINDOWS) {
        success = init_windows_platform(config);
    }
    #endif
    
    if (!success) return false;
    
    // 复制配置
    memcpy(&g_platform_config, config, sizeof(PlatformConfig));
    
    // 创建互斥锁
    if (g_platform_config.mutex_create) {
        g_platform_config.mutex = g_platform_config.mutex_create();
    }
    
    return true;
}

PlatformConfig* observer_platform_get_config(void) {
    return &g_platform_config;
}

bool observer_platform_set_config(const PlatformConfig* config) {
    if (!config) return false;
    
    // 销毁旧的互斥锁
    if (g_platform_config.mutex && g_platform_config.mutex_destroy) {
        g_platform_config.mutex_destroy(g_platform_config.mutex);
    }
    
    // 复制新的配置
    memcpy(&g_platform_config, config, sizeof(PlatformConfig));
    
    // 创建新的互斥锁
    if (g_platform_config.mutex_create) {
        g_platform_config.mutex = g_platform_config.mutex_create();
    }
    
    return true;
}

void* observer_platform_malloc(size_t size) {
    return g_platform_config.malloc ? g_platform_config.malloc(size) : malloc(size);
}

void observer_platform_free(void* ptr) {
    if (g_platform_config.free) {
        g_platform_config.free(ptr);
    } else {
        free(ptr);
    }
}

void observer_platform_mutex_lock(void* mutex) {
    if (g_platform_config.mutex_lock) {
        g_platform_config.mutex_lock(mutex);
    }
}

void observer_platform_mutex_unlock(void* mutex) {
    if (g_platform_config.mutex_unlock) {
        g_platform_config.mutex_unlock(mutex);
    }
}

void* observer_platform_mutex_create(void) {
    return g_platform_config.mutex_create ? g_platform_config.mutex_create() : NULL;
}

void observer_platform_mutex_destroy(void* mutex) {
    if (g_platform_config.mutex_destroy) {
        g_platform_config.mutex_destroy(mutex);
    }
}

void observer_platform_deinit(void) {
    if (g_platform_config.mutex && g_platform_config.mutex_destroy) {
        g_platform_config.mutex_destroy(g_platform_config.mutex);
    }
    memset(&g_platform_config, 0, sizeof(PlatformConfig));
} 
