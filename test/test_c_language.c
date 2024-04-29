#include "test.h"
#include <stdint.h>
#include <stdio.h>

//测试C语言结构体直接赋值
void test_struct_assign(void)
{
    struct student
    {
        uint8_t age;
        uint8_t height;
        uint8_t weight;
    };

    struct student zhangsan = {
        .age    = 18,
        .height = 180,
        .weight = 100};
    struct student lisi = {
        .age    = 28,
        .height = 160,
        .weight = 200};
    printf("zhangsan age = %d, height = %d weight = %d\r\n", zhangsan.age, zhangsan.height, zhangsan.weight);
    printf("lisi = %d, height = %d weight = %d\r\n", lisi.age, lisi.height, lisi.weight);
    printf("zhangsan assign lisi\r\n");
    zhangsan = lisi;
    printf("zhangsan age = %d, height = %d weight = %d\r\n", zhangsan.age, zhangsan.height, zhangsan.weight);
    printf("lisi = %d, height = %d weight = %d\r\n", lisi.age, lisi.height, lisi.weight);
}
