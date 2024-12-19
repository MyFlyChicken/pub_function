/**
 * @file pp_example.c
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

#include "../inc/pp_config.h"

#include "../inc/pp_private.h"
#include "pp_example.h"

/* The default time for waiting request ack */
#define DEFAULT_TIMEOUT (3000)

/* pp example handle structure defined */
struct pp_example_handle {
    struct pp_handle pp;        /* parent */
    notify_cb        notify_cb; /* notify user */
};

static void example_notify_cb(struct pp_handle* h, uint8_t evt, const void* param);

#if EXAMPLE_MASTER
/* 收到从机应答消息的回调函数 */
static void resp_get_status_cb(struct pp_handle* h, frame_t* f);
static void resp_set_ota_cb(struct pp_handle* h, frame_t* f);
#else
/* 收到主机发送请求消息的回调函数 */
static void req_get_status_cb(struct pp_handle* h, frame_t* f);
static void req_set_ota_cb(struct pp_handle* h, frame_t* f);
#endif

static const func_and_cb_t g_example_func_and_cb_list[] = {
#if EXAMPLE_MASTER
    {    RESP_FUNC(FUNC_GET_STATUS), resp_get_status_cb},
    {RESP_FUNC(FUNC_GET_ERR_STATUS),    resp_set_ota_cb},
#else
    {REQ_FUNC(FUNC_GET_STATUS), req_get_status_cb},
    {REQ_FUNC(FUNC_SET_OTA), req_set_ota_cb},
#endif
    {                          0x00,               NULL}
};

struct pp_example_handle* pp_example_handle_new(void)
{
    struct pp_example_handle* handle = PP_MALLOC(sizeof(struct pp_example_handle));
    if (!handle) {
        PP_LOG_E("Fail when malloc pp_example_handle.");
    }

    return handle;
}

pp_err_t pp_example_handle_delete(struct pp_example_handle* h)
{
    PP_FREE(h);

    return PP_EOK;
}

pp_err_t pp_example_handle_init(struct pp_example_handle* h, hw_send_cb send, notify_cb notify)
{
    PP_ASSERT(h);

    pp_handle_init(&h->pp, g_example_func_and_cb_list, send, example_notify_cb);
    h->notify_cb = notify;

    return PP_EOK;
}

static void example_notify_cb(struct pp_handle* h, uint8_t evt, const void* param)
{
    PP_ASSERT(h);

    switch (evt) {
    case EVT_ACK_TIMEOUT:
        PP_LOG_D("frame index:%d func:0x%02x timeout.",
                 ((const param_of_notify_timeout_t*)param)->frame_index,
                 ((const param_of_notify_timeout_t*)param)->frame_func);

        /* And notify user */
        notify_cb cb = ((struct pp_example_handle*)h)->notify_cb;
        if (cb) {
            /* Because `param_of_notify_resp_of_XXX_t` all use result_t for the head, so there can use
                    result_t for the param of cb function. */
            result_t result;
            result.index  = ((const param_of_notify_timeout_t*)param)->frame_index;
            result.result = PP_ETIMEOUT;
            cb(h, RESP_FUNC(((const param_of_notify_timeout_t*)param)->frame_func), &result);
        }
        break;
    default:
        PP_LOG_D("unknown event.");
        break;
    }
}

#if EXAMPLE_MASTER

pp_err_t pp_example_req_status(struct pp_example_handle* h, const param_of_notify_req_status_t* param)
{
    PP_ASSERT(h);

    frame_t frame;
    frame.index       = param->frame_index;
    frame.func        = REQ_FUNC(FUNC_GET_STATUS);
    frame.payload     = NULL;
    frame.payload_len = 0;
    frame.ret         = RET_OK;

    return pp_send(&h->pp, &frame, DEFAULT_TIMEOUT);
}

pp_err_t pp_example_rcv_ota_pkg(struct pp_example_handle* h, const param_of_notify_rcv_ota_pkg_t* param)
{
    PP_ASSERT(h);

    frame_t frame;
    frame.index       = param->frame_index;
    frame.func        = REQ_FUNC(FUNC_SET_OTA);
    frame.payload     = param->pkg_data;
    frame.payload_len = param->pkg_len;
    frame.ret         = RET_OK;

    return pp_send(&h->pp, &frame, DEFAULT_TIMEOUT);
}

static void resp_get_status_cb(struct pp_handle* h, frame_t* f)
{
    PP_ASSERT(h && f);

    notify_cb cb = ((struct pp_example_handle*)h)->notify_cb;

    if (cb) {
        param_of_notify_resp_of_req_status_t param;

        if (f->payload_len != 1) {
            PP_LOG_E("payload len is invalid.");
            param.result.index  = f->index;
            param.result.result = PP_EPARAM;
        }
        else {
            param.result.index  = f->index;
            param.result.result = pp_trans_ret_to_err(f->ret);
            param.status        = f->payload[0];
        }

        cb(h, EVT_RESP_OF_REQ_STAUTS, &param);
    }
}

static void resp_set_ota_cb(struct pp_handle* h, frame_t* f)
{
    PP_ASSERT(h && f);

    notify_cb cb = ((struct pp_example_handle*)h)->notify_cb;

    if (cb) {
        param_of_notify_resp_of_rcv_ota_pkg_t param;

        if (f->payload_len != 2) {
            PP_LOG_E("payload len is invalid.");
            param.result.index  = f->index;
            param.result.result = PP_EPARAM;
        }
        else {
            param.result.index  = f->index;
            param.result.result = pp_trans_ret_to_err(f->ret);
            param.pkg_index     = (uint16_t)f->payload[0] << 8 | (uint16_t)f->payload[1];
        }

        cb(h, EVT_RESP_OF_RCV_OTA_PKG, &param);
    }
}
#else

pp_err_t pp_example_resp_of_req_status(struct pp_example_handle* h, const param_of_notify_resp_of_req_status_t* param)
{
    PP_ASSERT(h);

    frame_t frame;
    frame.index       = param->result.index;
    frame.func        = RESP_FUNC(FUNC_GET_STATUS);
    frame.payload     = &param->status;
    frame.payload_len = 1;
    frame.ret         = pp_trans_err_to_ret(param->result.result);

    return pp_send(&h->pp, &frame, DEFAULT_TIMEOUT);
}

pp_err_t pp_example_resp_of_rcv_ota_pkg(struct pp_example_handle* h, const param_of_notify_resp_of_rcv_ota_pkg_t* param)
{
    PP_ASSERT(h);

    uint8_t  buf[2];
    uint8_t* p = buf;
    package_append_u16(&p, param->pkg_index);

    frame_t frame;
    frame.index       = param->result.index;
    frame.func        = RESP_FUNC(FUNC_SET_OTA);
    frame.payload     = buf;
    frame.payload_len = 2;
    frame.ret         = pp_trans_err_to_ret(param->result.result);

    return pp_send(&h->pp, &frame, DEFAULT_TIMEOUT);
}

static void req_get_status_cb(struct pp_handle* h, frame_t* f)
{
    PP_ASSERT(h && f);

    notify_cb cb = ((struct pp_example_handle*)h)->notify_cb;

    if (cb) {
        param_of_notify_req_status_t param;
        param.frame_index = f->index;
        cb(h, EVT_REQ_STATUS, &param);
    }
}

static void req_set_ota_cb(struct pp_handle* h, frame_t* f)
{
    PP_ASSERT(h && f);

    notify_cb cb = ((struct pp_example_handle*)h)->notify_cb;

    if (cb) {
        if (f->payload_len < 6) {
            param_of_notify_resp_of_rcv_ota_pkg_t resp_param;
            resp_param.result.index  = f->index;
            resp_param.result.result = PP_EPARAM;
            PP_LOG_E("payload len is invalid.");
            pp_example_resp_of_rcv_ota_pkg((struct pp_example_handle*)h, &resp_param);
        }
        else {
            param_of_notify_rcv_ota_pkg_t req_param;
            req_param.frame_index = f->index;
            req_param.pkg_data    = f->payload;
            req_param.pkg_len     = f->payload_len;
            cb(h, EVT_RCV_OTA_PKG, &req_param);
        }
    }
}

#endif /* EXAMPLE_MASTER */
