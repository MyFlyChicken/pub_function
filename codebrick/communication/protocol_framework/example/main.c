/**
 * @file main.c
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pp_example.h"

struct pp_example_handle* h_example = NULL;

static int my_hw_send_cb(const uint8_t* data, uint16_t len);
static void my_notify_cb(struct pp_handle* h, uint8_t evt, const void* param);

int main(void)
{
    h_example = pp_example_handle_new();
    if (!h_example)
    {
        printf("create example handle fail.");
        exit(-1);
    }

    pp_example_init(h_example, my_hw_send_cb, my_notify_cb);

    while (1)
    {
        pp_poll((struct pp_handle*)h_example);
    }

    pp_example_handle_delete(h_example);

    exit(0);
}

/**< my_notify_cb函数可以不要。直接在 @b g_example_func_and_cb_list数组内的函数指针进行解析及处理。这样避免一次多余的出入栈 */
static void my_notify_cb(struct pp_handle* h, uint8_t evt, const void* param)
{
    switch (evt)
    {
        /* S l a v e */
        case EVT_REQ_STATUS:
        {
            const param_of_notify_req_status_t* evt_param = param;
            param_of_notify_resp_of_req_status_t send_param;

            send_param.result.index = evt_param->frame_index;
            send_param.result.result = PP_EOK;
            send_param.status = 0x00; /* modify 0x00 to your device status */
            pp_example_resp_of_req_status(h_example, &send_param);
        }
        break;

        /* M a s t e r */
        case EVT_RESP_OF_REQ_STAUTS:
        {
            const param_of_notify_resp_of_req_status_t* evt_param = param;

            printf("Recv resp of status. index:%d, result:%d, status:0x%02X",
                   evt_param->result.index,
                   evt_param->result.result,
                   evt_param->status);
        }
        break;

        default:
            PP_PRINTF("unkown event.\n");
            break;
    }
}

/************************************************************************************
 *                                  P O R T
 * 
 ***********************************************************************************/

/* This func must be call, when you recieve data. */
void hw_recv_data(uint8_t* data, uint16_t len)
{
    pp_save_hw_recv_data(h_example, data, len);
}

static int my_hw_send_cb(const uint8_t* data, uint16_t len)
{
    /* Use USB,UART,TCP etc. to send data */

    return 0;
}

uint32_t pp_get_time_tick(void)
{
    return 0;
}
