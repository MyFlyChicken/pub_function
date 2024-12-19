/**
 * @file frame_rcv.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-03-22
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-03-22 <td>v1.0     <td>chen     <td>内容
 * </table>
 */
#ifndef FRAME_RCV_H
#define FRAME_RCV_H

#ifdef __cplusplus
extern "C" {
#endif

#define ASSERT_EN (0U)

#define LOG_PRINT(...)

#if ASSERT_EN
#define ASSERT(x)                                               \
    if (!(x)) {                                                 \
        LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__); \
    }
#else
#define ASSERT(x)
#endif

/**
 * @brief 解析器状态机状态定义
 * 
 * @details 协议需要满足 “帧头+长度+数据” 这种格式
 */
typedef enum {
    PARSE_FSM_HEAD = 0, /* 等待帧头 */
    PARSE_FSM_LEN,      /* 等待帧长度数据 */
    PARSE_FSM_END,      /* 等待完整帧数据 */
} parse_fsm_e;

/**
 * @brief 对收到的数据进行处理
 * 
 * @details data为收到的数据，该函数判断收到的数据手是否为帧头
 *          返回值：0继续添加header 1切换到接收长度 2数据非法
 */
typedef int (*get_frame_header)(unsigned char data); /* 0 为帧头 -1 不为帧头 */
/**
 * @brief 对收到的数据进行处理
 * 
 * @details data为收到的数据，该函数判断收到的数据手是否为帧长度
 *          返回值：0 继续添加 1切换到接收数据
 */
typedef unsigned short (*get_frame_length)(unsigned char data, unsigned short* len); /*  */
/**
 * @brief 对收到的数据进行处理
 * 
 * @details 
 */
typedef int (*frame_fsm_end)(unsigned char* data, unsigned short size);

typedef struct
{
    unsigned char         steup;
    unsigned char*        dest; /* 缓存数据 */
    unsigned short        dest_len;
    unsigned short        frame_len;     /* 实际的帧长 */
    const unsigned short* max_frame_len; /* 最小的帧长度 */
    const unsigned short* min_frame_len; /* 最小的帧长度 */
    get_frame_header      frame_header;
    get_frame_length      frame_length;
    frame_fsm_end         fsm_end; /* 取帧结束执行函数 */
} frame_rcv_t;

void frame_rcv_process(frame_rcv_t* p_frame_rcv, unsigned char data);

#ifdef __cplusplus
}
#endif
#endif
