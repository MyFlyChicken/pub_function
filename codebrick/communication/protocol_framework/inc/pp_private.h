/**
 * @file pp_private.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-12-11 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */

#ifndef inc_pp_private_h
#define inc_pp_private_h

#ifdef __cplusplus
extern "C"
{
#endif

#include "../inc/pp.h"
#include "../inc/pp_list.h"
#include "../inc/pp_ringbuffer.h"

/* print hex */
#define PP_LOG_HEX(DATA, LEN) pp_log_hex((DATA), (LEN))

/* Check that this function code is used for a request or a response. */
#define IS_REQ_FUNC(FUNC) (!((FUNC)&0x80))
#define IS_RESP_FUNC(FUNC) ((FUNC)&0x80)

    /* protocol function code type */
    typedef uint8_t func_t;

    /* protocol return state type */
    typedef uint8_t ret_t;
#define RET_OK (0x00)
#define RET_NOT_SUPPORT (0x01)
#define RET_HW_ERR (0x02)
#define RET_REJECT (0x03)
#define RET_INVAL (0x04)

    /* the info of a frame */
    typedef struct
    {
        uint16_t index;         /* The index of this frame */
        func_t func;            /* The function code of this frame */
        ret_t ret;              /* The status returned in the ack-frame */
        const uint8_t* payload; /* The payload getted from this frame */
        uint16_t payload_len;   /* The len(bytes) of payload */
    } frame_t;

    /* payload parse callback function */
    typedef void (*parse_cb)(struct pp_handle* h, frame_t* f);
    typedef void (*data_stashed_cb)(void);
    typedef void (*data_parse_cb)(void);

    /* function code and it's parse fucntion */
    typedef struct
    {
        func_t func;
        parse_cb cb;
    } func_and_cb_t;

/* notify */
#define EVT_ACK_TIMEOUT (0x00)

    /* param when notify ack timeout */
    typedef struct
    {
        uint16_t frame_index;
        func_t frame_func;
    } param_of_notify_timeout_t;

    /* state of recv frame */
    typedef enum
    {
        RX_POLL_WAIT_HEAD1,
        RX_POLL_WAIT_HEAD2,
        RX_POLL_WAIT_LEN,
        RX_POLL_WAIT_ALL
    } rx_poll_step_e;

    /* timeout detect */
    typedef struct
    {
        struct list_s list;
        uint16_t frame_index;
        func_t frame_func;
        uint32_t start_tick;
        uint32_t time;
    } timeout_pkg_t;

    /* pp handle structure defined */
    struct pp_handle
    {
        ringbuffer_t rb;                    /* ringbuffer for rx */
        uint8_t rb_pool[PP_RINGBUFFER_LEN]; /* the pool of ringbuffer */

        uint8_t fb_pool[PP_FRAMEBUFFER_LEN]; /* frame buffer for saving one frame */
        uint16_t fb_save_index;              /* the index of saving data in frame buffer */

        uint16_t frame_len;     /* save frame len */
        uint32_t frame_timeout; /* if no data long time, we will end a frame */

#if PP_TIMEOUT_DET_EN
        timeout_pkg_t timeout_pkg_list; /* The head of a list, which saves pkg to detect
                                            whether ack is timeout. */
#endif

        const func_and_cb_t* func_and_cb_list; /* Low level stack will call parse cb by func,
                                               you should define NULL in the end of the list.
                                               eg. list = {{XX, XXcb}, {0, NULL}}; */

        rx_poll_step_e rx_poll_step; /* the step of analyze rx datas, like state-machine */

        hw_send_cb send_cb;              /* Stcak will call this cb to send data in hardware */
        notify_cb notify_cb;             /* notify upper level some thing */
        data_stashed_cb data_stashed_cb; /* notify receive data is stashed */
        data_parse_cb data_parse_cb;     /* notify upper data can parse */
    };

    void pp_log_hex(const uint8_t* data, uint16_t len);
    ret_t pp_trans_err_to_ret(pp_err_t err);
    pp_err_t pp_trans_ret_to_err(ret_t ret);
    pp_err_t pp_handle_init(struct pp_handle* h, const func_and_cb_t* list, hw_send_cb send, notify_cb notify, data_stashed_cb data_stashed, data_parse_cb data_parse);
    pp_err_t pp_send(struct pp_handle* h, const frame_t* f, uint16_t timeout);

    void package_append_str(uint8_t** frame, const uint8_t* pdata, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_private_h */
