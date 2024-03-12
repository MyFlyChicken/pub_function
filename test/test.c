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



/* ----------------------- Platform includes --------------------------------*/



/* ----------------------- Defines ------------------------------------------*/



/* ----------------------- Type definitions ---------------------------------*/



/* ----------------------- Static variables ---------------------------------*/



/* ----------------------- Static function ----------------------------------*/


/* ----------------------- Start implementation -----------------------------*/
static void * _filter_get_value(unsigned char num)
{
    static uint16_t buffer[] = {
0x000, 0x064, 0x0C9, 0x12C, 0x18F, 0x1F1, 0x252, 0x2B2, 
0x30F, 0x36B, 0x3C5, 0x41C, 0x471, 0x4C3, 0x513, 0x55F, 
0x5A7, 0x5ED, 0x62E, 0x66C, 0x6A6, 0x6DC, 0x70D, 0x73A, 
0x763, 0x787, 0x7A7, 0x7C2, 0x7D8, 0x7E9, 0x7F5, 0x7FD, 
0x7FF, 0x7FD, 0x7F5, 0x7E9, 0x7D8, 0x7C2, 0x7A7, 0x787, 
0x763, 0x73A, 0x70D, 0x6DC, 0x6A6, 0x66C, 0x62E, 0x5ED, 
0x5A7, 0x55F, 0x513, 0x4C3, 0x471, 0x41C, 0x3C5, 0x36B, 
0x30F, 0x2B2, 0x252, 0x1F1, 0x18F, 0x12C, 0x0C9, 0x064, 
0x000, 0xF9C, 0xF37, 0xED4, 0xE71, 0xE0F, 0xDAE, 0xD4E, 
0xCF1, 0xC95, 0xC3B, 0xBE4, 0xB8F, 0xB3D, 0xAED, 0xAA1, 
0xA59, 0xA13, 0x9D2, 0x994, 0x95A, 0x924, 0x8F3, 0x8C6, 
0x89D, 0x879, 0x859, 0x83E, 0x828, 0x817, 0x80B, 0x803, 
0x801, 0x803, 0x80B, 0x817, 0x828, 0x83E, 0x859, 0x879, 
0x89D, 0x8C6, 0x8F3, 0x924, 0x95A, 0x994, 0x9D2, 0xA13, 
0xA59, 0xAA1, 0xAED, 0xB3D, 0xB8F, 0xBE4, 0xC3B, 0xC95, 
0xCF1, 0xD4E, 0xDAE, 0xE0F, 0xE71, 0xED4, 0xF37, 0xF9C, 
};

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
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT()) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

void test_func1(void)
{
    TEST_ASSERT_GREATER_THAN(100, 1000);
    TEST_ASSERT_GREATER_THAN(10, 100);
}

void test_main(void)
{
    UnityBegin("test/test.c");
    RUN_TEST(test_func1, 93);
    printf("<<<123>>>\n");
}
