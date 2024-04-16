/**
 * @file eb.c
 * @author MyFlyChicken ()
 * @brief 
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
#include <cstdint>
#include <cstring>

eb_port_t   _eb_port;
eb_frame_t  _eb_frame;
eb_header_t _eb_header;

/**
 * @brief 上电加载数据
 * @param [in] cnt 
 * @return eb_err_t 
 * 
 * @details 需要在eb_init之前调用，返回失败调用eb_init
 */
static eb_err_t _eb_loading(void)
{
    int         ret;
    eb_header_t header;

    ret = _eb_port.read(0, (uint8_t*)&header, sizeof(eb_header_t));
    if (ret)
    {
        EB_DEBUG("read flash failed %d", ret);
        return EB_WRITE_ERR;
    }

    if (strcmp(header.name, EB_NAME))
    {
        EB_DEBUG("header name not match %d", ret);
        return EB_NAME_NOT_FOUND;
    }

    return EB_NO_ERR;
}

/**
 * @brief 初始化eb管理flash区域
 * @param [in] bytes 
 * @return eb_err_t 
 * 
 * @details 
 */
eb_err_t eb_init(uint32_t offset, uint32_t bytes)
{
    int ret;

    EB_ASSERT(((EB_NAME_MAX_BYTES > sizeof(EB_NAME)) && sizeof(EB_NAME)));
    EB_ASSERT(bytes > sizeof(eb_header_t))

    ret = _eb_loading();
    if (ret)
    {
        EB_DEBUG("eb loading failed %d", ret);
        return ret;
    }
    else
    {
        return EB_NO_ERR;
    }

    EB_MEMSET(_eb_header.name, 0, EB_NAME_MAX_BYTES);
    EB_MEMCOPY(_eb_header.name, EB_NAME, sizeof(EB_NAME));

    _eb_header.bytes = bytes;

    _eb_frame.next = NULL;

    ret = _eb_port.erase(offset, bytes);
    if (ret)
    {
        EB_DEBUG("erase flash failed %d", ret);
        return EB_ERASE_ERR;
    }

    ret = _eb_port.write(offset, (const uint8_t*)&_eb_header, sizeof(eb_header_t));
    if (ret)
    {
        EB_DEBUG("write flash failed %d", ret);
        return EB_WRITE_ERR;
    }

    return EB_NO_ERR;
}

char* eb_read_data(eb_frame_t* p_frame, char* dst, uint32_t length)
{
}

char* eb_write_data(eb_frame_t* p_frame, const char* src, uint32_t length)
{
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
