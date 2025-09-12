/**
 * @file pp_expand1.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2025-09-11 <td>v1.0     <td>Yu     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#ifndef __pp_expand1_H_ //shift+U转换为大写
#define __pp_expand1_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "../inc/pp_basic.h"
#include "../inc/pp_config.h"

    struct expand1_handle; /**< 前向声明 */

    typedef uint8_t func_t;

    /**< 应答结果 */
    typedef enum
    {
        EXPAND1_RET_OK,
        EXPAND1_RET_NOT_SUPPORT,
        EXPAND1_RET_HW_ERR,
        EXPAND1_RET_REJECT,
        EXPAND1_RET_INVAL
    } expand1_ret_e;

    /**< 帧结构 */
    typedef struct
    {
        uint16_t index;         /* The index of this frame */
        func_t func;            /* The function code of this frame */
        expand1_ret_e ret;      /* The status returned in the ack-frame */
        uint16_t payload_len;   /* The len(bytes) of payload */
        const uint8_t* payload; /* The payload getted from this frame */
    } frame_t;

    /**<功能码及回调匹配结构体 */
    typedef struct
    {
        func_t func;
        void (*cb)(struct expand1_handle* h, frame_t* f);
    } func_and_cb_t;

    pp_err_t pp_expand1_resp_error(struct expand1_handle* h, uint8_t rsp_ret, uint32_t timeout);
    pp_err_t pp_expand1_send(struct expand1_handle* h, const frame_t* f, uint16_t timeout);
    struct expand1_handle* pp_expand1_init(void);

#ifdef __cplusplus
}
#endif

#endif
