/**
 * @file test.c
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
/* ----------------------- System includes ----------------------------------*/
#include "test.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../algorithm/filter/filter_algo.h"
/* ----------------------- Platform includes --------------------------------*/

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
static uint16_t bufindex = 0;
/* ----------------------- Static function ----------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
static void* _filter_get_value(unsigned char num)
{
    static int16_t buffer[] = {//1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
                               10,
                               9,
                               8,
                               7,
                               6,
                               5,
                               4,
                               3,
                               2,
                               1,
                               0};

    bufindex = (bufindex + 1) % (sizeof(buffer) / sizeof(int16_t));
    return &buffer[bufindex];
}

static void _setUp(void)
{
    bufindex = 0;
    filter_inf_register(_filter_get_value);
    printf("%s, %s, %d\r\n", __func__, __FILE__, __LINE__);
}

void tearDown(void)
{
    return;
}

void test_filter1(void)
{
    int16_t tmp;

    tmp = filter1();
    TEST_ASSERT_EQUAL_UINT16(1, tmp);
}

void test_filter2(void)
{
    int16_t tmp;
    tmp = filter2(10);
    TEST_ASSERT_EQUAL_UINT16(5, tmp);
}

void test_filter3(void)
{
    int16_t tmp;
    tmp = filter3(10);
    TEST_ASSERT_UINT16_WITHIN(1, 5, tmp);
}

void test_filter4(void)
{
    int16_t tmp;
    for (int16_t i = 0; i < 10; i++) {
        tmp = filter4();
        printf("raw dat:%d\r\n", tmp);
    }

    TEST_ASSERT_UINT16_WITHIN(1, 5, tmp);
}

void test_filter5(void)
{
    int16_t tmp;
    tmp = filter5(10);
    TEST_ASSERT_UINT16_WITHIN(1, 5, tmp);
}

void test_filter_main(void)
{
    TEST_FUNCTION_BEGIN();
    UnityBegin("test/test.c");
    RUN_TEST(test_filter1, 80);
    RUN_TEST(test_filter2, 88);
    RUN_TEST(test_filter3, 95);
    RUN_TEST(test_filter4, 90);
    RUN_TEST(test_filter5, 97);
    TEST_FUNCTION_END();
}
