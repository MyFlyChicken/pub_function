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

    struct ppVtbl; /* forward declaration */
    /* pp basic structure defined */
    struct pp_basic
    {
        struct ppVtbl const* vptr;    /* <== pp_basic's Virtual Pointer */
        ringbuffer_t         rb;      /* ringbuffer for rx */
        uint8_t*             rb_pool; /* the pool of ringbuffer */

        uint8_t*          fb_pool;           /* frame buffer for saving one frame */
        uint32_t          fb_size;           /* the size of frame buffer */
        ringbuffer_lock   ringbuffer_lock;   /* lock the ringbuffer */
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

    void     pp_basic_ctor(struct pp_basic* const me, uint8_t* rb_pool, uint32_t rb_size, uint8_t* fb_pool, uint32_t fb_size, ringbuffer_lock lock, ringbuffer_unlock unlock);
    uint32_t pp_basic_push_data(struct pp_basic* const me, const uint8_t* data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
