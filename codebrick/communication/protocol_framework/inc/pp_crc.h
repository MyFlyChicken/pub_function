/**
 * @file pp_crc.h
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

#ifndef inc_pp_crc_h
#define inc_pp_crc_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint16_t pp_crc_calculate(const uint8_t* buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_crc_h */
