/**
 * @file pp_example.h
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

#ifndef inc_pp_example___h
#define inc_pp_example___h

#ifdef __cplusplus
extern "C" {
#endif

#include "../inc/pp_config.h"

#include "pp.h"

#define EXAMPLE_MASTER (0U)

/* anker function code */
#define FUNC_GET_STATUS (0x01)
#define FUNC_SET_OTA    (0x09)

/* clang-format off */
/* notify event */
#define EVT_REQ_STATUS              REQ_FUNC(FUNC_GET_STATUS)   /* cb param is `param_of_notify_req_status_t`. The same below */
#define EVT_RESP_OF_REQ_STAUTS      RESP_FUNC(FUNC_GET_STATUS)  /* cb param is `param_of_notify_resp_of_req_status_t`. The same below */

#define EVT_RCV_OTA_PKG             REQ_FUNC(FUNC_SET_OTA)
#define EVT_RESP_OF_RCV_OTA_PKG     RESP_FUNC(FUNC_SET_OTA)

/* clang-format on */
typedef struct
{
    uint16_t frame_index;
} param_of_notify_req_status_t;

typedef struct
{
    result_t result;
    uint8_t  status;
} param_of_notify_resp_of_req_status_t;

typedef struct
{
    uint16_t       frame_index;
    uint16_t       pkg_len;
    const uint8_t* pkg_data; /* identify[2Bytes]+pkg_index[2Bytes]+pkg_num[2Bytes]+ota_data[pkg_len-6] */
} param_of_notify_rcv_ota_pkg_t;

typedef struct
{
    result_t result;
    uint16_t pkg_index;
} param_of_notify_resp_of_rcv_ota_pkg_t;

struct pp_example_handle;

/**
 * @brief new one pp_example handle.
 *
 * @return struct pp_example_handle* the data make.
 */
struct pp_example_handle* pp_example_handle_new(void);

/**
 * @brief delete one pp_example handle.
 *
 * @param h handle.
 * @return pp_err_t result return.
 */
pp_err_t pp_example_handle_delete(struct pp_example_handle* h);

/**
 * @brief Init pp_example handle which is already malloc.
 *
 * @param h handle.
 * @param send hardware send data function
 * @param notify notify user.
 * @return pp_err_t return
 */
pp_err_t pp_example_handle_init(struct pp_example_handle* h, hw_send_cb send, notify_cb notify);

#if EXAMPLE_MASTER
/*  */
pp_err_t pp_example_req_status(struct pp_example_handle* h, const param_of_notify_req_status_t* param);
pp_err_t pp_example_rcv_ota_pkg(struct pp_example_handle* h, const param_of_notify_rcv_ota_pkg_t* param);
#else
/* slave resp master APIs */
pp_err_t pp_example_resp_of_req_status(struct pp_example_handle* h, const param_of_notify_resp_of_req_status_t* param);
pp_err_t pp_example_resp_of_rcv_ota_pkg(struct pp_example_handle* h, const param_of_notify_resp_of_rcv_ota_pkg_t* param);
#endif /* EXAMPLE_MASTER */

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_m___h */
