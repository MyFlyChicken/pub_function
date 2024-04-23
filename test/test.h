/**
 * @file test.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-03-05
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-03-05 <td>v1.0     <td>chen     <td>内容
 * </table>
 */
#ifndef __test_H_ //shift+U转换为大写
#define __test_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define UNITY_INCLUDE_CONFIG_H
#include "../UnityTesting/src/unity.h"

    /* ----------------------- Defines ------------------------------------------*/

    /* ----------------------- Type definitions ---------------------------------*/

    /* ----------------------- Extern variable ---------------------------------*/
    void test_main(void);

    void test_filter_main(void);
    void test_eb_main(void);
    void test_led_main(void);
    /* ----------------------- Extern function ----------------------------------*/

    /* ----------------------- End ----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
