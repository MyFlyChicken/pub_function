#include "test.h"

void test_main(void)
{
    test_filter_main();
    test_eb_main();
    test_blink_main();
    test_mrmem_main();
    test_struct_assign();
    test_ee_device();
    test_async();
    test_polling();
    test_func_queue();
    test_key();
}
