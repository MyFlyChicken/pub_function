#include "test.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

void test_eq(void)
{
#define CALC_PI     (3.141592653589793)
#define CALC_SAMPLE (48000U)
#define CALC_Q      (1.41f)
#define CALC_GAIN   (0)

    signed long res;
    double      a0, a1, a2, b0, b1, b2;
    double      w0, gain_leaner, db_value, A, alpha;

    w0          = 2 * CALC_PI * 250 / CALC_SAMPLE;
    gain_leaner = pow(10, CALC_GAIN / 20);
    db_value    = -3;
    A           = pow(10, db_value / 40);
    alpha       = sin(w0) / (2 * CALC_Q);

    a0 = 1 + alpha / A;
    a1 = 2 * cos(w0);
    a2 = alpha / A - 1;
    b0 = (1 + alpha * A) * gain_leaner;
    b1 = (-2 * cos(w0)) * gain_leaner;
    b2 = (1 - alpha * A) * gain_leaner;

    a1 = a1 / a0;
    a2 = a2 / a0;
    b0 = b0 / a0;
    b1 = b1 / a0;
    b2 = b2 / a0;

    //测试发现sigmastudio修改eq时，发现db设置为0时，实际滤波系数与计算的不同。以sigmastudio的为准
    if (0 == db_value) {
        a1 = 0;
        a2 = 0;
        b0 = 1;
        b1 = 0;
        b2 = 0;
    }

    printf("%f %f %f %f %f \r\n", a1, a2, b0, b1, b2);
}

//测试C语言结构体直接赋值
void test_struct_assign(void)
{
    struct student {
        uint8_t age;
        uint8_t height;
        uint8_t weight;
    };

    struct student zhangsan = {
        .age    = 18,
        .height = 180,
        .weight = 100,
    };
    struct student lisi = {
        .age    = 28,
        .height = 160,
        .weight = 200,
    };
    TEST_FUNCTION_BEGIN();
    printf("zhangsan age = %d, height = %d weight = %d\r\n", zhangsan.age, zhangsan.height, zhangsan.weight);
    printf("lisi = %d, height = %d weight = %d\r\n", lisi.age, lisi.height, lisi.weight);
    printf("zhangsan assign lisi\r\n");
    zhangsan = lisi;
    printf("zhangsan age = %d, height = %d weight = %d\r\n", zhangsan.age, zhangsan.height, zhangsan.weight);
    printf("lisi = %d, height = %d weight = %d\r\n", lisi.age, lisi.height, lisi.weight);

    test_eq();
    TEST_FUNCTION_END();
}
