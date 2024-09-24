#include "data_queue.h"

#ifndef NULL
#define NULL 0
#endif

/**
 * @brief 初始化data_item_t
 * @param [in] p_item 
 * @param [in] buf 
 * @param [in] length 
 * 
 * @details 
 */
void date_item_init(data_item_t*          p_item,
                    unsigned char*        buf,
                    const unsigned short* length)
{
    ASSERT(p_item);
    ASSERT(buf);
    ASSERT(length);

    p_item->buf       = buf;
    p_item->length    = length;
    p_item->curr_len  = 0;
    p_item->busy_flag = 0;
}

/**
 * @brief 
 * @param [in] p_date_queue 
 * @param [in] p_data_item 
 * @param [in] group 
 * @param [in] sem 
 * 
 * @details 
 */void date_queue_init(data_queue_t*        p_date_queue,
                     data_item_t*         p_data_item,
                     const unsigned char* group,
                     void*                sem)
{
    ASSERT(p_date_queue);
    ASSERT(p_data_item);
    ASSERT(group);

    p_date_queue->item  = p_data_item;
    p_date_queue->group = group;
    p_date_queue->sem   = sem;
}

/**
 * @brief 获取空闲的item
 * @param [in] p_data_queue 
 * @return data_item_t* 
 * 
 * @details 
 */

data_item_t* date_queue_get_idle(data_queue_t* p_data_queue,
                                 signed long   timeout)
{
    ASSERT(p_data_queue);
    unsigned char i;

    for (i = 0; i < *p_data_queue->group; i++) {
        if (0 == p_data_queue->item[i].busy_flag) {
            p_data_queue->item[i].busy_flag = 1;
            return ((data_item_t*)&p_data_queue->item[i]);
        }
    }

    SEM_TAKE_INF(p_data_queue->sem, timeout);

    for (i = 0; i < *p_data_queue->group; i++) {
        if (0 == p_data_queue->item[i].busy_flag) {
            p_data_queue->item[i].busy_flag = 1;
            return ((data_item_t*)&p_data_queue->item[i]);
        }
    }
    return NULL;
}

void date_item_release(data_item_t* buffer)
{
    ASSERT(buffer);

    buffer->curr_len  = 0;
    buffer->busy_flag = 0;
    LOG_PRINT("release buffer");
}
