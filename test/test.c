#include "test.h"

int test_main(void)
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
    test_oop_c();
    test_c11_generic();
    test_binary_search();
    test_pubsub();

    return UNITY_END();
}
