/**
 * @file pp.h
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

#ifndef inc_pp_h
#define inc_pp_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "pp_config.h"

/* 标准错误定义 */
typedef int pp_err_t;
#define PP_EOK      (0)   /* RET_OK */
#define PP_ERROR    (-1)  /* RET_HW_ERR */
#define PP_ETIMEOUT (-2)  /* RET_HW_ERR */
#define PP_EFULL    (-3)  /* RET_HW_ERR */
#define PP_EEMPTY   (-4)  /* RET_HW_ERR */
#define PP_ENOMEM   (-5)  /* RET_HW_ERR */
#define PP_ENOSYS   (-6)  /* RET_HW_ERR */
#define PP_EBUSY    (-7)  /* RET_REJECT */
#define PP_EIO      (-8)  /* RET_HW_ERR */
#define PP_EENV     (-9)  /* RET_REJECT */
#define PP_EPARAM   (-10) /* RET_INVAL */
#define PP_EUNKNOWN (-11) /* RET_NOT_SUPPORT */

struct pp_handle;

/* result of respon cmd */
typedef struct
{
    uint16_t index;
    pp_err_t result;
} result_t;

/* Force make the func to request or respons format. */
#define REQ_FUNC(FUNC)  ((FUNC) & 0x7F)
#define RESP_FUNC(FUNC) ((FUNC) | 0x80)

/* Stcak will call this cb to send data in hardware. If return 0 meas ok, otherwise fail. */
typedef int (*hw_send_cb)(const uint8_t* data, uint16_t len);

/**
 *  Low level stack notify upper level stack, or stack notify user.
 *  Where h is the caller, evt is the event, and param is the paramter
 *  carried by the event.
 */
typedef void (*notify_cb)(struct pp_handle* h, uint8_t evt, const void* param);

/**
 * @brief You should call this function when you recv data.
 *
 * @param h handle. Can be subclass of `struct pp_handle`.
 * @param data Data you recevied.
 * @param len The length of data.
 * @return uint16_t The num of data which not be saved.
 */
uint16_t pp_save_hw_recv_data(struct pp_handle* h, const uint8_t* data, uint16_t len);

/**
 * @brief You should call this function in loop.
 *
 * @param h handle. Can be subclass of `struct pp_handle`.
 */
void pp_poll(struct pp_handle* h);

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_h */
