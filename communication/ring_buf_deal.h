/* 该文件注意用于解决环形缓冲区内收到数据后，如何断帧，获取到一帧完整的数据 */
#ifndef RING_BUF_DEAL_H
#define RING_BUF_DEAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_PRINT(...) 

#define ASSERT(x)   if (!(x))   \
                    {           \
                        LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__); \
                    }

 /* 解析器状态机状态定义 */
typedef enum
{
    PARSE_FSM_HEAD = 0, /* 等待帧头 */
    PARSE_FSM_LEN,      /* 等待帧长度数据 */
    PARSE_FSM_END,      /* 等待完整帧数据 */
} parse_fsm_e;

typedef int (* get_frame_header)(unsigned char data);/* 0 为帧头 -1 不为帧头 */
typedef unsigned short (* get_frame_length)(unsigned char data, unsigned short *len);/*  */
typedef int (* func_fsm_end)(void *ring_buf_deal);

typedef struct 
{
    unsigned char steup;
    unsigned char *dest;
    unsigned short dest_len;
    unsigned short frame_len;/* 实际的帧长 */
    const unsigned short *max_frame_len;/* 最小的帧长度 */
    const unsigned short *min_frame_len;/* 最小的帧长度 */
    get_frame_header frame_header;/* 0继续添加header 1切换到接收长度 2数据非法 */
    get_frame_length frame_length;/* 0 继续添加 1切换到接收数据 */
    func_fsm_end     fsm_end;/* 取帧结束执行函数 */
}ring_buf_deal_t;

void ring_buf_deal_main(ring_buf_deal_t *p_ring_buf_deal, unsigned char data);

#ifdef __cplusplus
}
#endif
#endif
