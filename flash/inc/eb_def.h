/**
 * @file eb_def.h
 * @author MyFlyChicken ()
 * @brief flash磨损函数对外声明头文件
 * @version 0.0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024 
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$版本</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-04-15 <td>0.0.1     <td>MyFlyChicken     <td>初版
 * </table>
 */
#ifndef __EB_DEF_H_ //shift+U转换为大写
#define __EB_DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "eb_cfg.h"
    /** @ingroup eb
  * @defgroup erase_balance_def
  * @{
  */

    /* log function. default EB_PRINT macro is printf() */
#ifndef EB_PRINT
#define EB_PRINT(...) printf(__VA_ARGS__)
#endif
#define EB_LOG_PREFIX1() EB_PRINT("[EB]" EB_LOG_TAG)
#define EB_LOG_PREFIX2() EB_PRINT(" ")
#define EB_LOG_PREFIX() \
    EB_LOG_PREFIX1();   \
    EB_LOG_PREFIX2()
#ifdef EB_DEBUG_ENABLE
#define EB_DEBUG(...)                         \
    EB_LOG_PREFIX();                          \
    EB_PRINT("(%s:%d) ", __FILE__, __LINE__); \
    EB_PRINT(__VA_ARGS__)
#else
#define EB_DEBUG(...)
#endif
/* routine print function. Must be implement by user. */
#define EB_INFO(...) \
    EB_LOG_PREFIX(); \
    EB_PRINT(__VA_ARGS__)
/* assert for developer. */
#ifdef EB_ASSERT_EN
#define EB_ASSERT(EXPR)                                              \
    if (!(EXPR))                                                     \
    {                                                                \
        EB_INFO("(%s) has assert failed at %s.\n", #EXPR, __func__); \
        while (1)                                                    \
            ;                                                        \
    }
#endif /* EB_ASSERT_EN */

    typedef enum
    {
        EB_NO_ERR = 0,
        EB_ERASE_ERR,
        EB_READ_ERR,
        EB_WRITE_ERR,
        EB_NAME_NOT_FOUND,
        EB_CRC_ERR,
        EB_INIT_FAILED,
    } eb_err_t;

/**
     * @brief flash管理帧头
     * 
     * @details 必须四字节对齐
     */
#pragma pack(4)
    typedef struct
    {
        char     name[EB_NAME_MAX_BYTES]; /*!< eb用户设置的名字 */
        uint32_t bytes;                   /*!< flash管理字节数，不包含eb_header_t的长度 */
    } eb_header_t;
#pragma pack()

    /**
     * @brief  eb操作flash接口
     * 
     * @details init接口可以不需要
     */
    typedef struct
    {
        int (*init)(void);
        int (*read)(uint32_t offset, uint8_t* buf, uint32_t size);
        int (*write)(uint32_t offset, const uint8_t* buf, uint32_t size);
        int (*erase)(uint32_t offset, uint32_t size);
        uint16_t (*crc16)(uint16_t init_val, uint8_t* pdata, uint32_t len);
    } eb_port_t;

    /**
     * @brief 
     * 
     * @details 
     */
#pragma pack(4)
    typedef struct
    {
        eb_frame_t* next;  /* 下一个数据区，实际为数据区的地址 */
        uint32_t    bytes; /* 用户赋值的数据 */
        uint16_t    crc16; /* 缓存数据 */
    } eb_frame_t;
#pragma pack()
    /**
  *@}
  */

#ifdef __cplusplus
}
#endif

#endif
