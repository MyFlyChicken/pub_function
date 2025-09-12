/**
 * @file pp_expand1.c
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
#include "pp_expand1.h"
#include <stdint.h>
#include <sys/types.h>

/**
 * @brief 平台相关
 * 
 * @details 
 */
#define EXPAND1_DELAY_US(n) sleep_us(n) /**< 每次轮询延时 */

/**
 * @brief 帧结构
 *  send: 5A A5 LenH LenL IDH IDL FUNC [DATA0 DATA1 ... DATAN] CRC16H CRC16L
 *  resp: 5A A5 LenH LenL IDH IDL FUNC RET [DATA0 DATA1 ... DATAN] CRC16H CRC16L
 * @details 
 */

#define FRAME_HEAD1 (0x5A)  /**< 帧头1 1字节 */
#define FRAME_HEAD2 (0xA5)  /**< 帧头2 1字节 */
#define FRAME_LEN_POS (2)   /**< 长度字段位置 2字节（仅包含 IDH+IDL+FUNC+PAYLOAD_LEN+CRC16字节） LenH/LenL */
#define FRAME_INDEX_POS (4) /**< 索引字段位置 2字节，IDH/IDL */
#define FRAME_FUNC_POS (6)  /**< 功能字段位置 1字节， */
#define FRAME_RET_POS (7)   /**< 返回值字段位置 1字节， */

#define FRAME_MIN_LEN (7) /**< 最小帧长度  */

#define IS_REQ_FUNC(FUNC) (!((FUNC)&0x80))
#define IS_RESP_FUNC(FUNC) ((FUNC)&0x80)

#define FRAME_PAYLOAD_POS(FUNC) (IS_RESP_FUNC(FUNC) ? (8) : (7))
#define FRAME_GET_CRC_POS(len) (len - 2 + FRAME_INDEX_POS)

#define EXPAND1_DEFAULT_TIMEOUT_MS (3000) /**< 默认超时时间 */

#define EXPAND1_RINGBUFFER_LEN (2048) /**< 环形缓冲区长度 */
#define EXPAND1_FRAMEBUFFER_LEN (512) /**< 帧缓冲区长度 */

typedef enum
{
    EXPAND1_POLL_WAIT_HEAD1,
    EXPAND1_POLL_WAIT_HEAD2,
    EXPAND1_POLL_WAIT_LEN,
    EXPAND1_POLL_WAIT_ALL
} expand1_poll_step_e;

/**
 * @brief Callback to notify that receive data is ready for processing.
 *        This is used for thread synchronization to wake up the polling thread.
 */
typedef void (*data_stash_cb)(void);
typedef void (*data_copied_cb)(void);
typedef void (*data_parsed_cplt_cb)(void);

/* Stack will call this cb to send data in hardware. If return 0 meas ok, otherwise fail. */
typedef int (*send_cb)(const uint8_t* data, uint16_t len);

static uint8_t _rb_buffer[EXPAND1_RINGBUFFER_LEN];  /**< 环形缓冲区 */
static uint8_t _fb_buffer[EXPAND1_FRAMEBUFFER_LEN]; /**< 帧缓冲区 */

struct expand1_handle
{
    /* the index of saving data in frame buffer */
    struct pp_basic basic; /* must first*/

    uint16_t frame_len;     /* save frame len */
    uint16_t fb_save_index; /* the index of saving data in frame buffer */

    expand1_poll_step_e rx_poll_step;      /* the step of analyze rx datas, like state-machine */
    const func_and_cb_t* func_and_cb_list; /* Low level stack will call parse cb by func,
                                               you should define NULL in the end of the list.
                                               eg. list = {{XX, XXcb}, {0, NULL}}; */

    int (*send_cb)(const uint8_t* data, uint16_t len); /* Stcak will call this cb to send data in hardware */

    void (*data_stash_cb)(void);       /* notify upper receive data is stashed */
    void (*data_copied_cb)(void);      /* notify upper receive data is copied */
    void (*data_parsed_cplt_cb)(void); /* notify upper receive data is parsed complete */
};

static const uint16_t _crc16_table[] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

static void _expand1_parse_frame(struct expand1_handle* h)
{
    uint8_t illegal_func;
    /* crc */
    uint16_t crc_get = pp_basic_read_u16_le(&h->basic.fb_pool[FRAME_GET_CRC_POS(h->frame_len)]);
    uint16_t crc_cal = h->basic.vptr->crc16(0, &h->basic.fb_pool[FRAME_LEN_POS], h->frame_len - 2); /*!< -5-3 */

    if (crc_get != crc_cal)
    {
        PP_LOG_E("crc-get(0x%04x) is not equal to crc-cal(0x%04x).", crc_get, crc_cal);
        pp_expand1_resp_error(h, RET_CRC_ERROR, EXPAND1_DEFAULT_TIMEOUT_MS);
        return;
    }

    /* trans stream to frame var */
    frame_t frame;
    frame.func = h->basic.fb_pool[FRAME_FUNC_POS];
    frame.index = h->basic.fb_pool[FRAME_INDEX_POS];
    frame.payload = &h->basic.fb_pool[FRAME_PAYLOAD_POS(frame.func)];
    frame.payload_len = h->frame_len;

    /* search for func, and call it's cb function */
    if (h->func_and_cb_list)
    {
        const func_and_cb_t* pos = h->func_and_cb_list;
        illegal_func = 1;
        while (pos->cb)
        {
            if (pos->func == frame.func)
            {
                pos->cb(h, &frame);
                illegal_func = 0;
                break;
            }
            else
            {
                pos++;
            }
        }
        if (illegal_func)
        {
            pp_expand1_resp_error(h, RET_ILLEGAL_FUNCTION, EXPAND1_DEFAULT_TIMEOUT_MS);
        }
    }
}

/* Helper function to reset frame state */
static inline void _reset_frame_state(struct expand1_handle* h)
{
    h->rx_poll_step = EXPAND1_POLL_WAIT_HEAD1;
    h->fb_save_index = 0;
}

static void _expand1_dtor(struct pp_basic* const me)
{
    struct expand1_handle* h = (struct expand1_handle*)me;
    PP_ASSERT(h);

    PP_FREE(h);
}

static void _expand1_append_str(uint8_t** frame, const char* str, uint32_t actual_len, uint32_t demand_len)
{
    if (actual_len > demand_len)
    {
        actual_len = demand_len;
    }
    memcpy(*frame, str, actual_len);
    if (actual_len < demand_len)
    {
        memset((*frame + actual_len), 0, demand_len - actual_len);
    }

    *frame += demand_len;
}

static uint16_t _expand1_crc16(uint16_t init_value, const uint8_t* data, uint32_t len)
{
    register uint32_t i;
    register uint16_t crc16;
    register uint8_t idx;

    crc16 = 0;
    for (i = 0; i < len; i++)
    {
        idx = ((uint8_t)crc16) ^ (*data++);
        crc16 = (crc16 >> 8) ^ _crc16_table[idx];
    }

    return (crc16);
}

static void _expand1_poll(struct pp_basic* const me)
{
    struct expand1_handle* const h = (struct expand1_handle* const)me;

    PP_ASSERT(h);

#if PP_TIMEOUT_DET_EN
    /* check is timeout occur */
    check_is_timeout_occur(h);
#endif

    h->data_copied_cb ? h->data_copied_cb() : EXPAND1_DELAY_US(5000);
    pp_basic_lock(&h->basic);

    while (pp_basic_get_rb_len(&h->basic) > 0)
    {
        /* 如果已知剩余需要一次性读取的长度，沿用原来的整块读取 */
        if (h->rx_poll_step == EXPAND1_POLL_WAIT_ALL)
        {
            if (pp_basic_get_rb_len(&h->basic) >= (h->frame_len - h->fb_save_index))
            {
                uint32_t remaining_len = h->frame_len - h->fb_save_index;
                uint32_t actual_len = pp_basic_get_rb_data(&h->basic, &h->basic.fb_pool[h->fb_save_index], remaining_len);
                if (actual_len == remaining_len)
                {
                    h->fb_save_index = h->frame_len;
                    _expand1_parse_frame(h);
                    _reset_frame_state(h);
                    continue;
                }
                else
                {
                    PP_LOG_E("Failed to read complete frame data");
                    _reset_frame_state(h);
                    continue;
                }
            }
            else
            {
                break;
            }
        }

        /* 等待帧头阶段的快速路径：丢弃非帧头字节（尤其是连续的0xFF dummy） */
        if (h->rx_poll_step == EXPAND1_POLL_WAIT_HEAD1)
        {
            uint8_t ch;
            int progressed = 0;
            /* 快速丢弃直到读到第一个帧头字节 FRAME_HEAD1(0x6A) 为止 */
            while (pp_basic_get_rb_ch(&h->basic, &ch) == 1)
            {
                if (ch == FRAME_HEAD1)
                {
                    /* 命中第一个帧头，开始记录到帧缓冲 */
                    h->basic.fb_pool[0] = ch;
                    h->fb_save_index = 1;
                    h->rx_poll_step = EXPAND1_POLL_WAIT_HEAD2;
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
            case EXPAND1_POLL_WAIT_HEAD2:
            {
                uint8_t ch;
                if (pp_basic_get_rb_ch(&h->basic, &ch) != 1)
                    goto __exit_loop;
                if (ch == FRAME_HEAD2)
                {
                    h->basic.fb_pool[h->fb_save_index++] = ch;
                    h->rx_poll_step = EXPAND1_POLL_WAIT_LEN;

                    uint32_t need = (FRAME_LEN_POS + 2) - h->fb_save_index;
                    uint32_t avail = pp_basic_get_rb_len(&h->basic);
                    if (avail >= need)
                    {
                        uint32_t got = pp_basic_get_rb_data(&h->basic, &h->basic.fb_pool[h->fb_save_index], need);
                        if (got == need)
                        {
                            h->fb_save_index += need;
                            /* 已有 帧头和帧长度，直接进入 WAIT_LEN 去解析长度 */
                            h->rx_poll_step = EXPAND1_POLL_WAIT_LEN;
                        }
                    }
                }
                else
                {
                    /* 支持重叠：这个字节若是 HEAD1，保留；否则回到等 HEAD1 快速丢弃路径 */
                    if (ch == FRAME_HEAD1)
                    {
                        h->basic.fb_pool[0] = ch;
                        h->fb_save_index = 1;
                        h->rx_poll_step = EXPAND1_POLL_WAIT_HEAD2;
                    }
                    else
                    {
                        _reset_frame_state(h);
                    }
                }
                break;
            }
            case EXPAND1_POLL_WAIT_LEN:
            {
                if (h->fb_save_index < (FRAME_LEN_POS + 2))
                {
                    uint32_t need = (FRAME_LEN_POS + 2) - h->fb_save_index;
                    uint32_t avail = pp_basic_get_rb_len(&h->basic);
                    if (avail == 0)
                        goto __exit_loop;
                    uint32_t take = (need <= avail) ? need : avail;
                    uint32_t got = pp_basic_get_rb_data(&h->basic, &h->basic.fb_pool[h->fb_save_index], take);
                    h->fb_save_index += got;
                    if (h->fb_save_index < (FRAME_LEN_POS + 2))
                        goto __exit_loop;
                }

                uint16_t payload_len = pp_basic_read_u16_le(&h->basic.fb_pool[FRAME_LEN_POS]);
                h->frame_len = payload_len + FRAME_INDEX_POS; /* 帧长度 = 索引(2) + 长度(2) + payload_len */
                if (h->frame_len > EXPAND1_FRAMEBUFFER_LEN)
                {
                    PP_LOG_E("Frame length too long: %d", h->frame_len);
                    _reset_frame_state(h);
                    break;
                }

                h->rx_poll_step = EXPAND1_POLL_WAIT_ALL;
                break;
            }
            default:
            {
                PP_LOG_E("Invalid rx_poll_step: %d", h->rx_poll_step);
                _reset_frame_state(h);
                break;
            }
        }
    }

__exit_loop:
    pp_basic_lock(me);
    h->data_parsed_cplt_cb ? h->data_parsed_cplt_cb() : (void)0;
}

pp_err_t pp_expand1_resp_error(struct expand1_handle* h, uint8_t rsp_ret, uint32_t timeout)
{
    frame_t frame;

    frame.func = h->basic.fb_pool[FRAME_FUNC_POS] | 0x80; /* set resp flag */
    frame.index = pp_basic_read_u16_le(&h->basic.fb_pool[FRAME_INDEX_POS]);
    frame.payload_len = 1;
    frame.payload = NULL;
    frame.ret = rsp_ret;

    return pp_expand1_send(h, &frame, EXPAND1_DEFAULT_TIMEOUT_MS);
}

pp_err_t pp_expand1_send(struct expand1_handle* h, const frame_t* f, uint16_t timeout)
{
    uint16_t len;

    if (f->payload_len > (EXPAND1_FRAMEBUFFER_LEN - FRAME_INDEX_POS))
    {
        PP_LOG_E("payload is too long.");
        return PP_EPARAM;
    }

    uint8_t* buf = PP_MALLOC(f->payload_len + FRAME_INDEX_POS);
    if (!buf)
    {
        PP_LOG_E("malloc fail.");
        return PP_ENOMEM;
    }

    len = f->payload_len + FRAME_INDEX_POS + 2; /* +2 for CRC16 */

    uint8_t* scan = buf;
    pp_basic_append_byte(&scan, FRAME_HEAD1);
    pp_basic_append_byte(&scan, FRAME_HEAD2);
    pp_basic_append_u16_le(&scan, f->payload_len);
    pp_basic_append_u16_le(&scan, f->index);
    pp_basic_append_byte(&scan, f->func);

    if (IS_RESP_FUNC(f->func))
    {
        pp_basic_append_byte(&scan, f->ret);
    }

    if (f->payload)
    {
        h->basic.vptr->append_str(&scan, (const char*)f->payload, f->payload_len - 3, f->payload_len - 3);
    }

    uint16_t crc = h->basic.vptr->crc16(0xFFFF, &buf[FRAME_LEN_POS], len - 2); /* -2 for frame head */
    pp_basic_append_u16_le(&scan, crc);

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

struct expand1_handle* pp_expand1_init(void)
{
    static const struct ppVtbl expand1_vtable = {
        .dtor = _expand1_dtor,
        .append_str = _expand1_append_str,
        .crc8 = NULL,
        .crc16 = _expand1_crc16,
        .crc32 = NULL,
        .poll = _expand1_poll,
    };

    struct expand1_handle* h = PP_MALLOC(sizeof(struct expand1_handle));
    if (!h)
    {
        PP_LOG_E("malloc fail.");
        return NULL;
    }

    memset(h, 0, sizeof(struct expand1_handle));
    pp_basic_ctor(&h->basic, _rb_buffer, sizeof(_rb_buffer), _fb_buffer, sizeof(_fb_buffer), NULL, NULL);
    h->basic.vptr = &expand1_vtable;

    return h;
}
