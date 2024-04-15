/**
 * @file erase_balance_cfg.h
 * @author MyFlyChicken ()
 * @brief flash磨损均衡配置文件
 * @version 0.0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024 
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">版本</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-04-15 <td>0.0.1     <td>MyFlyChicken     <td>初版
 * </table>
 */
#ifndef __ERASE_BALANCE_CFG_H_ //shift+U转换为大写
#define __ERASE_BALANCE_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup erase_balance_cfg
  * @{
  */
#define EB_ASSERT_EN 1

#define LOG_PRINT(...)

#if EB_ASSERT_EN
#define EB_ASSERT(x)                                                           \
    if (!(x))                                                                  \
    {                                                                          \
        LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__);                \
        while (1)                                                              \
            ;                                                                  \
    }
#else
#define EB_ASSERT(x)
#endif

#define EB_NAME           ("EB_BALANCE") /*!< 匹配名字 */
#define EB_NAME_MAX_BYTES (16)           /*!< EB_NAME最大字节 */

    /**
  *@}
  */

    /* ----------------------- Defines ------------------------------------------*/

    /* ----------------------- Type definitions ---------------------------------*/

    /* ----------------------- Extern variable ---------------------------------*/

    /* ----------------------- Extern function ----------------------------------*/

    /* ----------------------- End ----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
