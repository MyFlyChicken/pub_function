/**
 * @file eb.h
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
 * <tr><td>2024-04-16 <td>v0.0.1     <td>MyFlyChicken     <td>初版
 * </table>
 */
#ifndef __EB_H_ //shift+U转换为大写
#define __EB_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "./inc/eb_cfg.h"
#include "./inc/eb_def.h"

    /** @defgroup eb
  * @{
  */
    eb_err_t eb_init(void);
    eb_err_t eb_header_check(uint32_t* cnt);
    char*    eb_get_data(eb_frame_t* p_frame);
    /**
  *@}
  */

#ifdef __cplusplus
}
#endif

#endif
