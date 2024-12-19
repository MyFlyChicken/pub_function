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
    eb_err_t eb_init(uint32_t offset, uint32_t bytes);
    eb_err_t eb_port_init(const eb_port_t* p_port);

    eb_err_t eb_read_data(eb_frame_t* p_frame, char* dst, uint32_t len, uint32_t* actual_len);
    eb_err_t eb_write_data(eb_frame_t* p_frame, const char* src, uint32_t length);

    eb_frame_t* eb_get_frame(void);
    /**
  *@}
  */

#ifdef __cplusplus
}
#endif

#endif
