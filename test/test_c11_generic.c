#include "test.h"

typedef int* i2c_master_bus_handle_t;

#define GET_TYPENAME(var) _Generic((var),               \
    i2c_master_bus_handle_t: "i2c_master_bus_handle_t", \
    char: "char",                                       \
    int: "int",                                         \
    default: "other type")

void test_c11_generic(void)
{
    i2c_master_bus_handle_t k;
    int                     i;
    char                    j;

    TEST_FUNCTION_BEGIN();

    printf("i type is %s, j type is%s, k type is %s\r\n", GET_TYPENAME(i), GET_TYPENAME(j), GET_TYPENAME(k));

    TEST_FUNCTION_END();
}
