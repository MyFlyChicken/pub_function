/**
 * @file pp.c
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
#include "../inc/pp_crc.h"
#include "../inc/pp_list.h"
#include "../inc/pp_ringbuffer.h"
#include "../inc/pp_private.h"

/* The format definition of frame */
#define FRAME_HEAD1 (0x5A)
#define FRAME_HEAD2 (0xA5)
#define FRAME_LEN_POS (2)
#define FRAME_INDEX_POS (4)
#define FRAME_FUNC_POS (6)
#define FRAME_RET_POS (7)
#define FRAME_PAYLOAD_POS(FUNC) (IS_RESP_FUNC(FUNC) ? (8) : (7))

/* Check is timeout */
#define IS_TIMEOUT(START, NOW, LEN) (((NOW) - (START) >= (LEN)) || ((START) > (NOW)))

#if PP_TIMEOUT_DET_EN
static void check_is_timeout_occur(struct pp_handle* h);
#endif

static void parse_frame(struct pp_handle* h);

PP_WEAK uint32_t pp_get_time_tick(void)
{
    PP_LOG_E("You must redefine this function.");
    return 0;
}

static inline uint16_t read_u16_be(const uint8_t* buf)
{
    return ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
}

static inline void package_append_byte(uint8_t** frame, uint8_t data)
{
    *(*frame)++ = data;
}

static inline void package_append_u16(uint8_t** frame, uint16_t data)
{
    *(*frame)++ = data >> 8;
    *(*frame)++ = data;
}

static inline void package_append_u32(uint8_t** frame, uint32_t data)
{
    *(*frame)++ = data >> 24;
    *(*frame)++ = data >> 16;
    *(*frame)++ = data >> 8;
    *(*frame)++ = data;
}

static inline void package_subtract_byte(const uint8_t** frame, uint8_t* u8_data)
{
    *u8_data = *(*frame)++;
}

static inline void package_subtract_u16(const uint8_t** frame, uint16_t* u16_data)
{
    uint8_t buf[2];

    memcpy(buf, *frame, 2);
    *frame += 2;

    *u16_data = (uint16_t)buf[0] << 8 | (uint16_t)buf[1];
}

static inline void package_subtract_u32(const uint8_t** frame, uint32_t* u32_data)
{
    uint8_t buf[4];

    memcpy(buf, *frame, 4);
    *frame += 4;

    *u32_data = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
}

pp_err_t pp_handle_init(struct pp_handle* h, const func_and_cb_t* list, hw_send_cb send, notify_cb notify, data_stashed_cb data_stashed, data_parse_cb data_parse)
{
    PP_ASSERT(h);

    h->func_and_cb_list = list;
    h->send_cb = send;
    h->notify_cb = notify;
    h->data_stashed_cb = data_stashed;
    h->data_parse_cb = data_parse;

    h->rx_poll_step = RX_POLL_WAIT_HEAD1;
    h->fb_save_index = 0;

    ringbuffer_init(&h->rb, h->rb_pool, PP_RINGBUFFER_LEN);

#if PP_TIMEOUT_DET_EN
    list_init(&h->timeout_pkg_list.list);
#endif

    return PP_EOK;
}

pp_err_t pp_send(struct pp_handle* h, const frame_t* f, uint16_t timeout)
{
    PP_ASSERT(h && f);

    if (f->payload_len > PP_PAYLOAD_MAX_LEN)
    {
        PP_LOG_D("payload is too long.");
        return PP_EPARAM;
    }

    uint8_t* buf = PP_MALLOC(f->payload_len + 10);
    if (!buf)
    {
        PP_LOG_E("malloc fail.");
        return PP_ENOMEM;
    }

    uint16_t len;
    if (IS_REQ_FUNC(f->func))
    {
        len = f->payload_len + 9;
    }
    else
    {
        len = f->payload_len + 10;
    }

    uint8_t* scan = buf;
    package_append_byte(&scan, FRAME_HEAD1);
    package_append_byte(&scan, FRAME_HEAD2);
    package_append_u16(&scan, len);
    package_append_u16(&scan, f->index);
    package_append_byte(&scan, f->func);

    if (IS_RESP_FUNC(f->func))
    {
        package_append_byte(&scan, f->ret);
    }
    else
    {
#if PP_TIMEOUT_DET_EN
        timeout_pkg_t* pkg = PP_MALLOC(sizeof(timeout_pkg_t));
        if (!pkg)
        {
            PP_LOG_E("timeout malloc fail.");
            PP_FREE(buf);
            return PP_ENOMEM;
        }
        else
        {
            pkg->start_tick = pp_get_time_tick();
            pkg->time = timeout;
            pkg->frame_index = f->index;
            pkg->frame_func = RESP_FUNC(f->func);

            list_add(&pkg->list, &h->timeout_pkg_list.list);
        }
#endif
    }

    package_append_str(&scan, f->payload, f->payload_len);

    uint16_t crc = pp_crc_calculate(buf, len - 2);
    package_append_u16(&scan, crc);

    int ret = 0;
    if (h->send_cb)
    {
        ret = h->send_cb(buf, len);
    }

    PP_FREE(buf);

    if (ret != 0)
    {
        return PP_EIO;
    }
    else
    {
        return PP_EOK;
    }
}

uint16_t pp_save_hw_recv_data(struct pp_handle* h, const uint8_t* data, uint16_t len)
{
    PP_ASSERT(h && data && (len > 0));

    uint16_t rel = ringbuffer_put(&h->rb, data, len);

    return (len - rel);
}

// void pp_poll(struct pp_handle* h)
// {
//     PP_ASSERT(h);

// #if PP_TIMEOUT_DET_EN
//     /* check is timeout occur */
//     check_is_timeout_occur(h);
// #endif

//     for (;;)
//     {
//         uint32_t tick = pp_get_time_tick();

//         uint8_t ch;
//         if (ringbuffer_getchar(&h->rb, &ch) != 1)
//         {
//             /* check is no data long time */
//             if (IS_TIMEOUT(h->frame_timeout, tick, FRAME_TIMEOUT))
//             {
//                 // PP_LOG_D("long time no data, end frame.");
//                 h->rx_poll_step = RX_POLL_WAIT_HEAD1;
//                 h->fb_save_index = 0;
//                 h->frame_timeout = tick;
//             }

//             POLL_DELAY;

//             break;
//         }
//         else
//         {
//             h->fb_pool[h->fb_save_index++] = ch;
//             h->frame_timeout = tick;
//         }

//         /* state-machine */
//         switch (h->rx_poll_step)
//         {
//             case RX_POLL_WAIT_HEAD1:
//                 if (ch == FRAME_HEAD1)
//                 {
//                     h->rx_poll_step = RX_POLL_WAIT_HEAD2;
//                 }
//                 break;
//             case RX_POLL_WAIT_HEAD2:
//                 if (ch == FRAME_HEAD2)
//                 {
//                     h->rx_poll_step = RX_POLL_WAIT_LEN;
//                 }
//                 else
//                 {
//                     h->rx_poll_step = RX_POLL_WAIT_HEAD1;
//                     h->fb_save_index = 0;
//                 }
//                 break;
//             case RX_POLL_WAIT_LEN:
//                 if (h->fb_save_index >= (FRAME_LEN_POS + 2))
//                 {
//                     h->frame_len = (uint16_t)h->fb_pool[FRAME_LEN_POS] << 8 | (uint16_t)h->fb_pool[FRAME_LEN_POS + 1];

//                     if (h->frame_len > PP_FRAMEBUFFER_LEN)
//                     {
//                         /* len is too long */
//                         h->rx_poll_step = RX_POLL_WAIT_HEAD1;
//                         h->fb_save_index = 0;
//                         break;
//                     }

//                     /* prepare to get all frame */
//                     h->rx_poll_step = RX_POLL_WAIT_ALL;
//                 }
//                 break;
//             case RX_POLL_WAIT_ALL:
//                 if (h->fb_save_index >= h->frame_len)
//                 {
//                     parse_frame(h);

//                     h->rx_poll_step = RX_POLL_WAIT_HEAD1;
//                     h->fb_save_index = 0;
//                 }
//                 break;
//             default:
//                 PP_LOG_E("handle step may not be init.");
//                 h->rx_poll_step = RX_POLL_WAIT_HEAD1;
//                 h->fb_save_index = 0;
//                 break;
//         }
//     }
// }
/* Helper function to reset frame state */
static inline void reset_frame_state(struct pp_handle* h)
{
    h->rx_poll_step = RX_POLL_WAIT_HEAD1;
    h->fb_save_index = 0;
}

void pp_poll(struct pp_handle* h)
{
    PP_ASSERT(h);

#if PP_TIMEOUT_DET_EN
    /* check is timeout occur */
    check_is_timeout_occur(h);
#endif

    if (h->data_parse_cb)
    {
        h->data_parse_cb(); /* Notify that data can be parsed */
    }
    else
    {
        POLL_DELAY;
    }

    /* Process data using state machine until we have a complete frame */
    while (ringbuffer_data_len(&h->rb) > 0)
    {
        uint8_t ch;

        /* If we're waiting for a complete frame, check if we have enough data */
        if (h->rx_poll_step == RX_POLL_WAIT_ALL)
        {
            if (ringbuffer_data_len(&h->rb) >= (h->frame_len - h->fb_save_index))
            {
                /* Read the remaining frame data */
                uint32_t remaining_len = h->frame_len - h->fb_save_index;
                uint32_t actual_len = ringbuffer_get(&h->rb, &h->fb_pool[h->fb_save_index], remaining_len);
                if (actual_len == remaining_len)
                {
                    h->fb_save_index = h->frame_len;
                    parse_frame(h);
                    reset_frame_state(h);
                    continue; /* Process next frame if available */
                }
                else
                {
                    PP_LOG_E("Failed to read complete frame data");
                    reset_frame_state(h);
                    continue;
                }
            }
            else
            {
                break; /* Not enough data for complete frame, wait for more */
            }
        }

        /* Read one byte for state machine processing */
        if (ringbuffer_getchar(&h->rb, &ch) != 1)
        {
            break; /* No more data available */
        }

        /* Check buffer overflow before writing */
        if (h->fb_save_index >= PP_FRAMEBUFFER_LEN)
        {
            PP_LOG_E("Frame buffer overflow, reset state.");
            reset_frame_state(h);
            continue;
        }

        h->fb_pool[h->fb_save_index++] = ch;

        /* State machine processing */
        switch (h->rx_poll_step)
        {
            case RX_POLL_WAIT_HEAD1:
            {
                if (ch == FRAME_HEAD1)
                {
                    h->rx_poll_step = RX_POLL_WAIT_HEAD2;
                }
                else
                {
                    reset_frame_state(h);
                }
                break;
            }
            case RX_POLL_WAIT_HEAD2:
            {
                if (ch == FRAME_HEAD2)
                {
                    h->rx_poll_step = RX_POLL_WAIT_LEN;
                }
                else
                {
                    reset_frame_state(h);
                    /* Check if current byte could be new frame start */
                    if (ch == FRAME_HEAD1)
                    {
                        h->fb_pool[h->fb_save_index++] = ch;
                        h->rx_poll_step = RX_POLL_WAIT_HEAD2;
                    }
                }
                break;
            }
            case RX_POLL_WAIT_LEN:
            {
                if (h->fb_save_index >= (FRAME_LEN_POS + 2))
                {
                    h->frame_len = read_u16_be(&h->fb_pool[FRAME_LEN_POS]);

                    /* Validate frame length */
                    if (h->frame_len > PP_FRAMEBUFFER_LEN)
                    {
                        PP_LOG_E("Frame length too long: %d", h->frame_len);
                        reset_frame_state(h);
                        break;
                    }
                    /* prepare to get remaining all frame */
                    h->rx_poll_step = RX_POLL_WAIT_ALL;
                }
                break;
            }
            default:
            {
                PP_LOG_E("Invalid rx_poll_step: %d", h->rx_poll_step);
                reset_frame_state(h);
                break;
            }
        }
    }
}

/**
 * @brief trans `pp_err_t` to `ret_t`
 *
 * @param result `pp_err_t`
 * @return ret_t `ret_t`
 */
ret_t pp_trans_err_to_ret(pp_err_t err)
{
    switch (err)
    {
        case PP_EOK:
            return RET_OK;
        case PP_EPARAM:
            return RET_INVAL;
        case PP_EBUSY:
        case PP_EENV:
            return RET_REJECT;
        case PP_EUNKNOWN:
            return RET_NOT_SUPPORT;
        default:
            return RET_HW_ERR;
    }
}

/**
 * @brief trans `ret_t` to `pp_err_t`
 *
 * @param ret `ret_t`
 * @return pp_err_t `pp_err_t`
 */
pp_err_t pp_trans_ret_to_err(ret_t ret)
{
    switch (ret)
    {
        case RET_OK:
            return PP_EOK;
        case RET_NOT_SUPPORT:
            return PP_EUNKNOWN;
        case RET_HW_ERR:
            return PP_ERROR;
        case RET_REJECT:
            return PP_EENV;
        default:
            return PP_ERROR;
    }
}

void package_append_str(uint8_t** frame, const uint8_t* pdata, uint16_t len)
{
    memcpy(*frame, pdata, len);
    *frame += len;
}

void pp_log_hex(const uint8_t* data, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        PP_PRINTF("%02x ", data[i]);

        if ((i + 1) % 16 == 0)
        {
            PP_PRINTF("\n");
        }
        else if ((i + 1) % 8 == 0)
        {
            PP_PRINTF("\t");
        }
    }
    PP_PRINTF("\n");
}

#if PP_TIMEOUT_DET_EN
static void check_is_timeout_occur(struct pp_handle* h)
{
    uint32_t tick = pp_get_time_tick();

    list_for_each(pos, &h->timeout_pkg_list.list)
    {
        if (IS_TIMEOUT(((timeout_pkg_t*)pos)->start_tick, tick, ((timeout_pkg_t*)pos)->time))
        {
            list_del(pos);

            if (h->notify_cb)
            {
                param_of_notify_timeout_t param;
                param.frame_func = ((timeout_pkg_t*)pos)->frame_func;
                param.frame_index = ((timeout_pkg_t*)pos)->frame_index;
                h->notify_cb(h, EVT_ACK_TIMEOUT, &param);
            }

            PP_FREE(pos);

            break;
        }
    }
}
#endif

static void parse_frame(struct pp_handle* h)
{
    /* crc */
    uint16_t crc_get = (uint16_t)h->fb_pool[h->frame_len - 2] << 8 | (uint16_t)h->fb_pool[h->frame_len - 1];
    uint16_t crc_cal = pp_crc_calculate(h->fb_pool, h->frame_len - 2);

    if (crc_get != crc_cal)
    {
        PP_LOG_E("crc-get(0x%04x) is not equal to crc-cal(0x%04x).", crc_get, crc_cal);
        return;
    }

    /* trans stream to frame var */
    frame_t frame;
    frame.func = h->fb_pool[FRAME_FUNC_POS];
    frame.index = (uint16_t)h->fb_pool[FRAME_INDEX_POS] << 8 | (uint16_t)h->fb_pool[FRAME_INDEX_POS + 1];
    frame.payload = &h->fb_pool[FRAME_PAYLOAD_POS(frame.func)];
    frame.payload_len = IS_RESP_FUNC(frame.func) ? (h->frame_len - 10) : (h->frame_len - 9);
    frame.ret = IS_RESP_FUNC(frame.func) ? (h->fb_pool[FRAME_RET_POS]) : 0x00;

    /* search for func, and call it's cb function */
    if (h->func_and_cb_list)
    {
        const func_and_cb_t* pos = h->func_and_cb_list;

        while (pos->cb)
        {
            if (pos->func == frame.func)
            {
#if PP_TIMEOUT_DET_EN
                list_for_each(pos, &h->timeout_pkg_list.list)
                {
                    if (((timeout_pkg_t*)pos)->frame_func == frame.func)
                    {
                        list_del(pos);
                        PP_FREE(pos);
                        break;
                    }
                }
#endif

                pos->cb(h, &frame);
                break;
            }
            else
            {
                pos++;
            }
        }
    }
}
