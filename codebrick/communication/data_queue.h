#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#define ASSERT_EN 1
#define LOG_PRINT(...)

#if ASSERT_EN
#define ASSERT(x)                                               \
    if (!(x)) {                                                 \
        LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__); \
        while (1)                                               \
            ;                                                   \
    }
#else
#define ASSERT(x)
#endif

#define SEM_WAITING_FOREVER (-1)
#define SEM_TAKE_INF(sem, timeout) \
    if (sem) {                     \
        ;                          \
        LOG_PRINT("take buf sem"); \
    }

typedef struct
{
    unsigned char*         buf;
    unsigned short         curr_len;
    const unsigned short*  length;
    volatile unsigned char busy_flag;
} data_item_t;

typedef struct
{
    data_item_t*         item;
    const unsigned char* group;
    void*                sem;
} data_queue_t;

void         date_item_init(data_item_t*          p_item,
                            unsigned char*        buf,
                            const unsigned short* length);
void         date_queue_init(data_queue_t*        p_data_queue,
                             data_item_t*         p_item,
                             const unsigned char* group,
                             void*                sem);
data_item_t* date_queue_get_idle(data_queue_t* p_data_queue,
                                 signed long   timeout);
void         date_item_release(data_item_t* buffer);

#ifdef __cplusplus
}
#endif
#endif
