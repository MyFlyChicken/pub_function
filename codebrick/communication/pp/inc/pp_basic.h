/**
 * @file pp_basic.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2025-09-09
 * 
 * @copyright Copyright (c) 2025 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2025-09-09 <td>v1.0     <td>Yu     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#ifndef __pp_basic_H_ //shift+U转换为大写
#define __pp_basic_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "pp_config.h"
#include "pp_ringbuffer.h"

    /* ringbuffer lock and unlock */
    typedef void (*ringbuffer_lock)(void);
    typedef void (*ringbuffer_unlock)(void);

    /* 标准错误定义 */
    typedef int pp_err_t;
#define PP_EOK (0)        /* RET_OK */
#define PP_ERROR (-1)     /* RET_HW_ERR */
#define PP_ETIMEOUT (-2)  /* RET_HW_ERR */
#define PP_EFULL (-3)     /* RET_HW_ERR */
#define PP_EEMPTY (-4)    /* RET_HW_ERR */
#define PP_ENOMEM (-5)    /* RET_HW_ERR */
#define PP_ENOSYS (-6)    /* RET_HW_ERR */
#define PP_EBUSY (-7)     /* RET_REJECT */
#define PP_EIO (-8)       /* RET_HW_ERR */
#define PP_EENV (-9)      /* RET_REJECT */
#define PP_EPARAM (-10)   /* RET_INVAL */
#define PP_EUNKNOWN (-11) /* RET_NOT_SUPPORT */

    /* RESP CODE */
    typedef enum
    {
        RET_OK = 0x00,                 ///< 无异常，操作成功完成
        RET_ILLEGAL_FUNCTION = 0x01,   ///< 非法功能码，请求的操作不支持
        RET_ILLEGAL_ADDRESS = 0x02,    ///< 非法数据地址，请求的地址超出范围
        RET_ILLEGAL_DATA_VALUE = 0x03, ///< 非法数据值，请求的值格式错误或越界
        RET_DEVICE_FAILURE = 0x04,     ///< 设备故障，内部硬件或软件错误
        RET_DEVICE_BUSY = 0x05,        ///< 设备繁忙，无法立即响应请求
        RET_ACK_TIMEOUT = 0x06,        ///< 主动发送数据时，接收超时
        RET_CRC_ERROR = 0x07,          ///< 数据帧的CRC计算错误
        RET_FRAME_TAIL_ERROR = 0x08,   ///< 数据帧尾部错误，可能是数据损坏或格式不正确
        RET_DIR_ERROR = 0x09,          ///< 方向错误，发送或接收数据时方向不匹配
        RET_OTHER = 0xFF,              ///< 其它原因
    } rsp_ret_e;

    struct ppVtbl; /* forward declaration */
    /* pp basic structure defined */
    struct pp_basic
    {
        struct ppVtbl const* vptr; /* <== pp_basic's Virtual Pointer */
        ringbuffer_t rb;           /* ringbuffer for rx */
        uint8_t* rb_pool;          /* the pool of ringbuffer */

        uint8_t* fb_pool;                    /* frame buffer for saving one frame */
        uint32_t fb_size;                    /* the size of frame buffer */
        ringbuffer_lock ringbuffer_lock;     /* lock the ringbuffer */
        ringbuffer_unlock ringbuffer_unlock; /* unlock the ringbuffer */
    };

    struct ppVtbl
    {
        void (*dtor)(struct pp_basic* const me); /* destructor */
        void (*poll)(struct pp_basic* const me); /* poll to parse data in ringbuffer */
        void (*append_str)(uint8_t** frame, const char* str, uint32_t actual_len, uint32_t demand_len);
        uint8_t (*crc8)(uint8_t init_value, const uint8_t* data, uint32_t len);
        uint16_t (*crc16)(uint16_t init_value, const uint8_t* data, uint32_t len);
        uint32_t (*crc32)(uint32_t init_value, const uint8_t* data, uint32_t len);
    };

    static inline void pp_basic_lock(struct pp_basic* const me)
    {
        me->ringbuffer_lock ? me->ringbuffer_lock() : (void)0;
    }

    static inline void pp_basic_unlock(struct pp_basic* const me)
    {
        me->ringbuffer_unlock ? me->ringbuffer_unlock() : (void)0;
    }

    static inline uint32_t pp_basic_get_rb_ch(struct pp_basic* const me, uint8_t* u8_data)
    {
        return ringbuffer_get(&me->rb, u8_data, 1);
    }

    static inline uint32_t pp_basic_get_rb_len(struct pp_basic* const me)
    {
        return ringbuffer_data_len(&me->rb);
    }

    static inline uint32_t pp_basic_get_rb_data(struct pp_basic* const me, uint8_t* data, uint32_t len)
    {
        return ringbuffer_get(&me->rb, data, len);
    }

    static inline void pp_basic_append_byte(uint8_t** frame, uint8_t data)
    {
        *(*frame)++ = data;
    }

    static inline void pp_basic_append_u16_be(uint8_t** frame, uint16_t data)
    {
        *(*frame)++ = data >> 8;
        *(*frame)++ = data;
    }

    static inline void pp_basic_append_u32_be(uint8_t** frame, uint32_t data)
    {
        *(*frame)++ = data >> 24;
        *(*frame)++ = data >> 16;
        *(*frame)++ = data >> 8;
        *(*frame)++ = data;
    }

    static inline void pp_basic_append_u16_le(uint8_t** frame, uint16_t data)
    {
        *(*frame)++ = data;
        *(*frame)++ = data >> 8;
    }

    static inline void pp_basic_append_u32_le(uint8_t** frame, uint32_t data)
    {
        *(*frame)++ = (uint8_t)(data);
        *(*frame)++ = (uint8_t)(data >> 8);
        *(*frame)++ = (uint8_t)(data >> 16);
        *(*frame)++ = (uint8_t)(data >> 24);
    }

    static inline uint16_t pp_basic_read_u16_be(const uint8_t* buf)
    {
        return ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    }

    static inline uint32_t pp_basic_read_u32_be(const uint8_t* buf)
    {
        return ((uint32_t)buf[3] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[1] << 8) | (uint32_t)buf[0];
    }

    static inline uint16_t pp_basic_read_u16_le(const uint8_t* buf)
    {
        return ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
    }

    static inline uint32_t pp_basic_read_u32_le(const uint8_t* buf)
    {
        return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    }

    void pp_basic_ctor(struct pp_basic* const me, uint8_t* rb_pool, uint32_t rb_size, uint8_t* fb_pool, uint32_t fb_size, ringbuffer_lock lock, ringbuffer_unlock unlock);
    uint32_t pp_basic_push_data(struct pp_basic* const me, const uint8_t* data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
