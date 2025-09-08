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
#define FRAME_MIN_LEN (7)
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

pp_err_t pp_handle_init(struct pp_handle* h, const func_and_cb_t* list, hw_send_cb send, notify_cb notify, data_stash_cb data_stash, data_copied_cb data_copied, ringbuffer_lock ringbuffer_lock, ringbuffer_unlock ringbuffer_unlock, data_parse_cplt_cb data_parse_cplt)
{
    PP_ASSERT(h);

    h->func_and_cb_list = list;
    h->send_cb = send;
    h->notify_cb = notify;
    h->data_stash_cb = data_stash;
    h->data_copied_cb = data_copied;
    h->ringbuffer_lock = ringbuffer_lock;
    h->ringbuffer_unlock = ringbuffer_unlock;
    h->data_parse_cplt_cb = data_parse_cplt;

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
    uint16_t rel;

    h->ringbuffer_lock ? h->ringbuffer_lock() : (void)0;
    rel = ringbuffer_put(&h->rb, data, len);
    /* When the buffer has enough data for a minimal frame header, notify the poll thread. */
    if (h->data_stash_cb && (rel > 0))
    {
        h->ringbuffer_unlock ? h->ringbuffer_unlock() : (void)0;
        h->data_stash_cb();
    }
    else
    {
        h->ringbuffer_unlock ? h->ringbuffer_unlock() : (void)0;
    }

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

    h->data_copied_cb ? h->data_copied_cb() : POLL_DELAY;
    h->ringbuffer_lock ? h->ringbuffer_lock() : (void)0;

    while (ringbuffer_data_len(&h->rb) > 0)
    {
        /* 如果已知剩余需要一次性读取的长度，沿用原来的整块读取 */
        if (h->rx_poll_step == RX_POLL_WAIT_ALL)
        {
            if (ringbuffer_data_len(&h->rb) >= (h->frame_len - h->fb_save_index))
            {
                uint32_t remaining_len = h->frame_len - h->fb_save_index;
                uint32_t actual_len = ringbuffer_get(&h->rb, &h->fb_pool[h->fb_save_index], remaining_len);
                if (actual_len == remaining_len)
                {
                    h->fb_save_index = h->frame_len;
                    parse_frame(h);
                    reset_frame_state(h);
                    continue;
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
                break;
            }
        }

        /* 等待帧头阶段的快速路径：丢弃非帧头字节（尤其是连续的0xFF dummy） */
        if (h->rx_poll_step == RX_POLL_WAIT_HEAD1)
        {
            uint8_t ch;
            int progressed = 0;
            /* 快速丢弃直到读到第一个帧头字节 FRAME_HEAD1(0x6A) 为止 */
            while (ringbuffer_getchar(&h->rb, &ch) == 1)
            {
                if (ch == FRAME_HEAD1)
                {
                    /* 命中第一个帧头，开始记录到帧缓冲 */
                    h->fb_pool[0] = ch;
                    h->fb_save_index = 1;
                    h->rx_poll_step = RX_POLL_WAIT_HEAD2;
                    progressed = 1;
                    break;
                }
                /* 否则直接丢弃该字节（大多数为0xFF的dummy） */
            }

            if (!progressed)
            {
                /* 没有更多数据可读，等待下次 */
                break;
            }
            /* 命中后继续下面的状态机处理 */
        }

        /* 下面各状态仅在需要时才写入 fb_pool，避免无效写入 */
        switch (h->rx_poll_step)
        {
            case RX_POLL_WAIT_HEAD2:
            {
                uint8_t ch;
                if (ringbuffer_getchar(&h->rb, &ch) != 1)
                    goto __exit_loop;
                if (ch == FRAME_HEAD2)
                {
                    h->fb_pool[h->fb_save_index++] = ch;
                    h->rx_poll_step = RX_POLL_WAIT_LEN;

                    /* 批量拉取 func(2) + rand(2) + len(2) = 6 字节（若可用） */
                    uint32_t need = (FRAME_LEN_POS + 2) - h->fb_save_index; /* = 9+2 - 当前(3) = 8，考虑前面逻辑，这里是再填充到 len 字段结束 */
                    uint32_t avail = ringbuffer_data_len(&h->rb);
                    if (avail >= need)
                    {
                        uint32_t got = ringbuffer_get(&h->rb, &h->fb_pool[h->fb_save_index], need);
                        if (got == need)
                        {
                            h->fb_save_index += need;
                            /* 已有 func/rand/len，直接进入 WAIT_LEN 去解析长度 */
                            h->rx_poll_step = RX_POLL_WAIT_LEN;
                        }
                    }
                }
                else
                {
                    /* 支持重叠：这个字节若是 HEAD1，保留；否则回到等 HEAD1 快速丢弃路径 */
                    if (ch == FRAME_HEAD1)
                    {
                        h->fb_pool[0] = ch;
                        h->fb_save_index = 1;
                        h->rx_poll_step = RX_POLL_WAIT_HEAD2;
                    }
                    else
                    {
                        reset_frame_state(h);
                    }
                }
                break;
            }
            case RX_POLL_WAIT_LEN:
            {
                if (h->fb_save_index < (FRAME_LEN_POS + 2))
                {
                    uint32_t need = (FRAME_LEN_POS + 2) - h->fb_save_index;
                    uint32_t avail = ringbuffer_data_len(&h->rb);
                    if (avail == 0)
                        goto __exit_loop;
                    uint32_t take = (need <= avail) ? need : avail;
                    uint32_t got = ringbuffer_get(&h->rb, &h->fb_pool[h->fb_save_index], take);
                    h->fb_save_index += got;
                    if (h->fb_save_index < (FRAME_LEN_POS + 2))
                        goto __exit_loop;
                }

                uint16_t payload_len = read_u16_be(&h->fb_pool[FRAME_LEN_POS]);
                h->frame_len = payload_len;

                if (h->frame_len > PP_FRAMEBUFFER_LEN)
                {
                    PP_LOG_E("Frame length too long: %d", h->frame_len);
                    reset_frame_state(h);
                    break;
                }

                h->rx_poll_step = RX_POLL_WAIT_ALL;
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

__exit_loop:
    h->ringbuffer_unlock ? h->ringbuffer_unlock() : (void)0;
    h->data_parse_cplt_cb ? h->data_parse_cplt_cb() : (void)0;
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
