#include "test.h"
#include "../codebrick/algorithm/search/algo_search.h"

uint32_t data[20]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
uint8_t  datau8[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

static void _setUp(void)
{
    printf("%s, %s, %d\r\n", __func__, __FILE__, __LINE__);
}

static void _test_binary_search_lrco(void)
{
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3, algo_binarysearch_lcro(data, 20, 4, _ALGO_SEARCH_U32), "AAA");
    TEST_ASSERT_EQUAL_UINT8(3, algo_binarysearch_lcro(datau8, 20, 4, _ALGO_SEARCH_U8));
}

static void _test_binary_search_lrcb(void)
{
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(16, algo_binarysearch_lcrc(data, 20, 17, _ALGO_SEARCH_U32), "BBB");
    TEST_ASSERT_EQUAL_UINT8(3, algo_binarysearch_lcrc(datau8, 20, 4, _ALGO_SEARCH_U8));
}

void test_binary_search(void)
{
    TEST_FUNCTION_BEGIN();
    UnityBegin("test/test_binary_search.c");
    RUN_TEST(_test_binary_search_lrco, 7);
    RUN_TEST(_test_binary_search_lrcb, 13);
    TEST_FUNCTION_END();
}
