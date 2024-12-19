/**
 * @file eb.c
 * @author MyFlyChicken ()
 * @brief 该模块实现均衡擦写，个人能力有限，暂不支持坏块管理
 * @version 0.0.1
 * @date 2024-04-16
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">版本</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-04-16 <td>v0.0.1    <td>MyFlyChicken     <td>初版
 * </table>
 * @section 
 * @code 
 * @endcode
 */

#include "eb.h"
#include "inc/eb_cfg.h"
#include "inc/eb_def.h"
#include <stdint.h>

eb_port_t   _eb_port;
eb_frame_t  _eb_frame;
eb_header_t _eb_header;

/**
 * @brief 上电加载数据
 * @return eb_err_t 
 * 
 * @details 需要在eb_init之前调用，返回失败调用eb_init
 */
static eb_err_t _eb_loading(void)
{
    int         ret;
    eb_header_t header;

    ret = _eb_port.read(0, (uint8_t*)&header, sizeof(eb_header_t));
    if (ret) {
        EB_DEBUG("read flash header failed %d", ret);
        return EB_READ_ERR;
    }

    if (strcmp(header.name, EB_NAME)) {
        EB_DEBUG("header name not match %d", ret);
        return EB_NAME_NOT_FOUND;
    }

    do {
        ret = _eb_port.read(_eb_frame.offset, (uint8_t*)&_eb_frame, sizeof(eb_frame_t));
        if (ret) {
            EB_DEBUG("read flash frame failed %d", ret);
            return EB_READ_ERR;
        }
    } while (_eb_frame.offset != EB_FRAME_OFFSET_DEFAULT);

    if (EB_FRAME_BYTES_DEFAULT == _eb_frame.bytes) {
        return EB_INVALID_FRAME;
    }

    return EB_NO_ERR;
}

/**
 * @brief 管理区域写满后会进行复位，调用该函数
 * @return eb_err_t 
 * 
 * @details 
 */
static eb_err_t _eb_reset(void)
{
    int ret;

    _eb_frame.offset = sizeof(eb_header_t);
    _eb_frame.bytes  = EB_FRAME_BYTES_DEFAULT;

    ret = _eb_port.erase(0, _eb_header.bytes);
    if (ret) {
        EB_DEBUG("erase flash failed %d", ret);
        return EB_ERASE_ERR;
    }

    ret = _eb_port.write(0, (const uint8_t*)&_eb_header, sizeof(eb_header_t));
    if (ret) {
        EB_DEBUG("write flash failed %d", ret);
        return EB_WRITE_ERR;
    }

    return EB_NO_ERR;
}

/**
 * @brief 获取frame信息
 * @return eb_frame_t* 
 * 
 * @details 
 */
eb_frame_t* eb_get_frame(void)
{
    return &_eb_frame;
}

/**
 * @brief 初始化eb管理flash区域
 * @param [in] offset 偏移
 * @param [in] bytes 管理区域字节
 * @return eb_err_t 
 * 
 * @details 第一次写入会写入一个空的frame
 */
eb_err_t eb_init(uint32_t offset, uint32_t bytes)
{
    int ret;

    EB_ASSERT(((EB_NAME_MAX_BYTES > sizeof(EB_NAME)) && sizeof(EB_NAME)));
    EB_ASSERT(bytes > sizeof(eb_header_t))

    ret = _eb_loading();
    if (ret) {
        EB_DEBUG("eb loading failed %d", ret);
    }

    EB_MEMSET(_eb_header.name, 0, EB_NAME_MAX_BYTES);
    EB_MEMCOPY(_eb_header.name, EB_NAME, sizeof(EB_NAME));

    _eb_header.bytes = bytes;

    _eb_frame.offset = sizeof(eb_header_t);
    _eb_frame.bytes  = EB_FRAME_BYTES_DEFAULT;
    _eb_frame.crc16  = 0;

    ret = _eb_port.erase(offset, bytes);
    if (ret) {
        EB_DEBUG("erase flash failed %d", ret);
        return EB_ERASE_ERR;
    }

    ret = _eb_port.write(offset, (const uint8_t*)&_eb_header, sizeof(eb_header_t));
    if (ret) {
        EB_DEBUG("write flash failed %d", ret);
        return EB_WRITE_ERR;
    }

    ret = _eb_port.write(_eb_frame.offset, (const uint8_t*)&_eb_frame, sizeof(eb_frame_t));
    if (ret) {
        EB_DEBUG("write frame failed %d", ret);
        return EB_WRITE_ERR;
    }

    return EB_NO_ERR;
}

/**
 * @brief 读取数据
 * @param [in] p_frame 考虑删除？
 * @param [in] dst 目标缓存区指针
 * @param [in] len 想要读取的长度
 * @param [in] actual_len 实际读取的长度
 * @return eb_err_t 
 * 
 * @details 
 */
eb_err_t eb_read_data(eb_frame_t* p_frame, char* dst, uint32_t len, uint32_t* actual_len)
{
    int ret;

    if (EB_FRAME_BYTES_DEFAULT == p_frame->bytes) {
        return EB_INVALID_FRAME;
    }

    if (len <= p_frame->bytes) {
        *actual_len = len;
    }
    else {
        *actual_len = p_frame->bytes;
    }

    ret = _eb_port.read(p_frame->offset + sizeof(eb_frame_t), (uint8_t*)dst, *actual_len);
    if (ret) {
        EB_DEBUG("read flash frame failed %d", ret);
        return EB_READ_ERR;
    }

    if (*actual_len) {
        /* 计算CRC */
        if (p_frame->crc16 != _eb_port.crc16(EB_CRC_INITVAL, (uint8_t*)dst, *actual_len)) {
            return EB_CRC_ERR;
        }
    }

    return EB_NO_ERR;
}

/**
 * @brief 写入数据
 * @param [in] p_frame 考虑删除？
 * @param [in] src 数据源
 * @param [in] length 数据源长度
 * @return eb_err_t 
 * 
 * @details 
 */
eb_err_t eb_write_data(eb_frame_t* p_frame, const char* src, uint32_t length)
{
    int        ret;
    eb_frame_t tmp;

    if ((p_frame->offset + p_frame->bytes + sizeof(eb_frame_t) + length) > _eb_header.bytes) {
        //重新初始化eb管理区域
        ret = _eb_reset();
        if (ret) {
            EB_DEBUG("_eb_reset failed %d", ret);
            return ret;
        }
    }

    tmp.offset = p_frame->offset + p_frame->bytes + sizeof(eb_frame_t);
    tmp.bytes  = length;
    tmp.crc16  = _eb_port.crc16(EB_CRC_INITVAL, (uint8_t*)src, length);

    ret = _eb_port.write(tmp.offset, (const uint8_t*)&tmp, sizeof(eb_frame_t));
    if (ret) {
        EB_DEBUG("write flash frame failed %d", ret);
        return EB_WRITE_ERR;
    }

    ret = _eb_port.write(tmp.offset + sizeof(eb_frame_t), (const uint8_t*)src, length);
    if (ret) {
        EB_DEBUG("write flash data failed %d", ret);
        return EB_WRITE_ERR;
    }

    _eb_frame.offset = tmp.offset;
    _eb_frame.bytes  = tmp.bytes;
    _eb_frame.crc16  = tmp.crc16;

    return EB_NO_ERR;
}

/**
 * @brief 赋值接口函数
 * @param [in] p_port 
 * @return eb_err_t 
 *          - EB_NO_ERR 只会返回这个
 * @details 
 */
eb_err_t eb_port_init(const eb_port_t* p_port)
{
    EB_ASSERT(p_port);
    EB_ASSERT(p_port->erase);
    EB_ASSERT(p_port->read);
    EB_ASSERT(p_port->write);
    EB_ASSERT(p_port->crc16);

    _eb_port.init  = p_port->init;
    _eb_port.erase = p_port->erase;
    _eb_port.read  = p_port->read;
    _eb_port.write = p_port->write;
    _eb_port.crc16 = p_port->crc16;

    return EB_NO_ERR;
}
