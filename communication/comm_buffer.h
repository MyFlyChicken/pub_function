#ifndef COMM_BUFFER_H
#define COMM_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_PRINT(...) 

#define ASSERT(x)   if (!(x))   \
                    {           \
                        LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__); \
                        while (1);  \                        
                    }

#define SEM_WAITING_FOREVER (-1)
#define SEM_TAKE_INF(sem, timeout) if (sem) \
                                    {\
                                        ;\
                                        LOG_PRINT("take buf sem");\
                                    }


typedef struct
{
    unsigned char *buf;
    unsigned short curr_len;
    const unsigned short* max_len;
    volatile unsigned char busy_flag;
} comm_buffer_t;

typedef struct 
{
    comm_buffer_t *com_buf;
    const unsigned char* max_group;
    void *sem;
} comm_buf_manage_t;

void comm_buffer_init(comm_buffer_t *comm_buffer, unsigned char *buf, const unsigned char* max_len);
void comm_buf_manage_init(comm_buf_manage_t *comm_buf_manage, comm_buffer_t *comm_buffer, const unsigned char* max_group, void *sem);
comm_buffer_t* comm_buffer_get_idle(comm_buf_manage_t *comm_buf_manage);
void comm_buffer_release(comm_buffer_t* buffer);






#ifdef __cplusplus
}
#endif
#endif
