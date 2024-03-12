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
#include <stdint.h>
#include <stdio.h>

/* ----------------------- Platform includes --------------------------------*/

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Static function ----------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
static void* _filter_get_value(unsigned char num)
{
    static int16_t buffer[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    return &buffer[0];
}

void setUp(void)
{
    filter_inf_register(_filter_get_value);
}

void tearDown(void)
{
    return;
}

/*=======Test Runner Used To Run Each Test Below=====*/
#define RUN_TEST(TestFunc, TestLineNum)            \
    {                                              \
        Unity.CurrentTestName       = #TestFunc;   \
        Unity.CurrentTestLineNumber = TestLineNum; \
        Unity.NumberOfTests++;                     \
        if (TEST_PROTECT())                        \
        {                                          \
            setUp();                               \
            TestFunc();                            \
        }                                          \
        if (TEST_PROTECT())                        \
        {                                          \
            tearDown();                            \
        }                                          \
        UnityConcludeTest();                       \
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

void test_filter5(void)
{
    int16_t tmp;
    tmp = filter5(10);
    TEST_ASSERT_UINT16_WITHIN(1, 5, tmp);
}

void test_main(void)
{
    UnityBegin("test/test.c");
    RUN_TEST(test_filter1, 80);
    RUN_TEST(test_filter2, 88);
    RUN_TEST(test_filter3, 95);
    RUN_TEST(test_filter5, 90);
}
