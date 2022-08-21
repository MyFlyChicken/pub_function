#include "public.h"
#include <stdio.h>

void fifo_test()
{
    uint16_t i, j, temp;
    FIFO _fifo;
    fifo_buffer_type pop_buffer[FIFO_BUFFER_SIZE] = {0};
    const fifo_buffer_type mystring[]= {123,999,888,777,666,555,444,333,222,111, 789};
    #if 1
    fifo_init(&_fifo);
    pub_memset(_fifo.buffer, 0, FIFO_BUFFER_SIZE);
    for ( i = 0; i < 10; i++)
    {
        temp = fifo_push_bytes(&_fifo, mystring, sizeof(mystring));
        //printf("%d---head = %d, tail = %d, temp = %d\r\n", i, _fifo.head, _fifo.tail, temp);
        for ( j = 0; j < FIFO_BUFFER_SIZE; j++)
        {
            /* code */
            printf("%d  ", _fifo.buffer[j]);
        }
        printf("\r\n");
    }
    printf("pop1---head = %d, tail = %d, %d\r\n",  _fifo.head, _fifo.tail, fifo_pop_bytes(&_fifo, pop_buffer, FIFO_BUFFER_SIZE));
    for ( j = 0; j < FIFO_BUFFER_SIZE; j++)
    {
        if (_fifo.buffer[(j + _fifo.tail)%FIFO_BUFFER_SIZE] != pop_buffer[j])
        {
            printf("1read fail! %d,\r\n", j);
            break;
        }
    }
    pub_memset(pop_buffer, 0, FIFO_BUFFER_SIZE);
    printf("pop2---head = %d, tail = %d, %d\r\n",  _fifo.head, _fifo.tail, fifo_pop_bytes_clear(&_fifo, pop_buffer, FIFO_BUFFER_SIZE));
    for ( j = 0; j < FIFO_BUFFER_SIZE; j++)
    {
        if (_fifo.buffer[(j + _fifo.tail)%FIFO_BUFFER_SIZE] != pop_buffer[j])
        {
            printf("2read fail! %d,\r\n", j);
            break;
        }
    }
    #endif
}

void test_sin_unsin(void)
{
    int32_t val = -2;
    
    printf("sin = %d, unsigned = %u\r\n", val, (uint32_t)val);
    for (size_t i = 0; i < 32; i++)
    {
        /* code */
        printf("%d", (val>>i)&0x01);
    }
}

void test_log_print(void)
{
    log_print_init(printf, NULL);
    LOG_PRINTF_INFO("0x%8x\r\n", 0x12345678);
    LOG_PRINTF_WARN("Warning\r\n");
    LOG_PRINTF_ERROR("Error\r\n");
    LOG_PRINTF_USER("Hello word!\r\n");

}


void test_bcd_conv()
{
    uint8_t  a = 0x55;
    uint16_t b = 0x1299;
    uint32_t c = 0x12345678;
    uint8_t  aa = 55;
    uint16_t bb = 1299;
    uint32_t cc = 12345678;
    uint32_t dd = 123456789;
    //printf("%d, %d, %d\r\n", pub_hex2bcd(a, sizeof(a)), pub_hex2bcd(b, sizeof(b)),pub_hex2bcd(c, sizeof(c)));
    printf("%d, %d, %d, %d\r\n", pub_bcd2hex(aa), pub_bcd2hex(bb), pub_bcd2hex(cc), pub_bcd2hex(dd));
    
}

void test_sprintf(void)
{
    printf("%s\r\n", pub_sprintf("0x%x", 0x123456));
}

void main()
{
    //test_sprintf();
    test_log_print();
    //test_bcd_conv();
    //fifo_test();
    //test_sin_unsin();
    //getchar();
}