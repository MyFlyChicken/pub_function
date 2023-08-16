#include "comm_buffer.h"

#ifndef NULL
#define NULL 0
#endif

/**
 * @brief 初始化comm_buffer
 * @param comm_buffer 
 * @param buf 
 * @param max_len 
 */
void comm_buffer_init(comm_buffer_t *comm_buffer, unsigned char *buf, const unsigned char *max_len)
{
    ASSERT(comm_buffer);
    ASSERT(buf);
    ASSERT(max_len);

    comm_buffer->buf = buf;
    comm_buffer->max_len = max_len;
    comm_buffer->curr_len = 0;
    comm_buffer->busy_flag = 0;
}

/**
 * @brief 初始化comm_buf_manage
 * @param comm_buf_manage 
 * @param comm_buffer 
 * @param max_group 
 * @param sem 
 */
void comm_buf_manage_init(comm_buf_manage_t *comm_buf_manage, comm_buffer_t *comm_buffer, const unsigned char *max_group, void *sem)
{
    ASSERT(comm_buf_manage);
    ASSERT(comm_buffer);
    ASSERT(max_group);

    comm_buf_manage->com_buf = comm_buffer;
    comm_buf_manage->max_group = max_group;
    comm_buf_manage->sem = sem;
}

/**
 * @brief 获取空闲的通讯buffer
 *
 * @return comm_buffer_t*
 */
comm_buffer_t* comm_buffer_get_idle(comm_buf_manage_t *comm_buf_manage)
{
    ASSERT(comm_buf_manage);
    unsigned char i;

    for (i = 0; i < *comm_buf_manage->max_group; i++)
    {
        if (0 == comm_buf_manage->com_buf[i].busy_flag)
        {
            SEM_TAKE_INF(comm_buf_manage->sem, 10);
            comm_buf_manage->com_buf[i].busy_flag = 1;
            return ((comm_buffer_t*)&comm_buf_manage->com_buf[i]);
        }
    }

    LOG_PRINT("The comm_buf_manage has not idle buffer!");
    SEM_TAKE_INF(comm_buf_manage->sem, SEM_WAITING_FOREVER);
    LOG_PRINT("The comm_buf_manage taked idle buffer!");
    for (i = 0; i < *comm_buf_manage->max_group; i++)
    {
        if (0 == comm_buf_manage->com_buf[i].busy_flag)
        {
            SEM_TAKE_INF(comm_buf_manage->sem, 10);
            comm_buf_manage->com_buf[i].busy_flag = 1;
            return ((comm_buffer_t*)&comm_buf_manage->com_buf[i]);
        }
    }
    LOG_PRINT("The comm_buf_manage has not idle buffer");
    return NULL;
}

void comm_buffer_release(comm_buffer_t* buffer)
{
    RT_ASSERT(buffer);

    buffer->curr_len = 0;
    buffer->busy_flag = 0;
    LOG_PRINT("release buffer");
}
