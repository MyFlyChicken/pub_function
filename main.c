#include "public.h"
#include <stdio.h>

typedef struct  __BL_CTRL_VAR__
{
	uint16_t percent;        /* 出光功率占比 */
	uint32_t delay_gap_ms;   /* 出光延长间隔，单位ms */
	uint32_t laser_on_ms;    /* 激光打开时间，单位ms，因为laser_on_cnt计数会在delay_gap_cnt清0之后清0，所以需要加上delay_gap_ms */
	uint16_t  laser_status;   /* 激光器状态，仅在模式为串口连续模式时进行控制 0关闭 1打开*/
	volatile uint32_t delay_gap_cnt;
	volatile uint32_t laser_on_cnt;
} BL_CTRL_VAR;

typedef struct __STRUCT_TEST__{
 uint16_t lds_amp;
 uint16_t overTempSet;
 uint16_t ld1_amp;
 uint16_t ld2_amp;
 BL_CTRL_VAR ctr_var;
 }STRUCT_TEST;


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

signed long test_stock(signed long principal, float amplitude, uint8_t timers)
{
    signed long ret = principal;

    while (timers--)
    {
        ret = ret * amplitude + ret;
    }
    
    return ret;
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

void test_memory_operate(void)
{
    uint8_t  byte_buf[6] = {0};
    uint16_t halfword_buf[6] = {0};
    uint32_t word_buf[6] = {0};

    uint8_t  byte_buf1[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    uint16_t halfword_buf1[6] = {0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666};
    uint32_t word_buf1[6] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666};

    memory_operate_init(NULL, printf);
    memory_write_byte(byte_buf, (const uint8_t *)byte_buf1, 6);
    memory_write_halfword(halfword_buf, (const uint16_t *)halfword_buf1, 6);
    memory_write_word(word_buf, (const uint32_t *)word_buf1, 6);

    memory_read_byte(byte_buf1, byte_buf, 6);
    memory_read_halfword(halfword_buf1, halfword_buf, 6);
    memory_read_word(word_buf1, word_buf, 6);
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

void test_printf(void)
{
    uint16_t a = 99;
    printf("%#x\r\n", 666);
    printf("%#p\r\n", &a);
    printf("%05d, %05d, %5d, %5d, %-5d\r\n", 11, 222, 3333, 4444, 5);
    printf("%f, %lf, %hu\r\n", 11.22, 1.2222333333333,4294967295);
    printf("%d\r\n", test_stock(3483, 0.1, 10)); 
}

void test_time_counter(void)
{
    uint32_t i;
    TIME_COUNTER _counter;
    _counter.count = 0;
    for (i = 0; i < 640000; i++)
    {
        pub_time_counte(&_counter);
    }
    printf("year = %d, day = %d, hour = %d, minute = %d, sec = %d\r\n", _counter.mid.year, _counter.mid.day, _counter.mid.hour, _counter.mid.minute, _counter.mid.second);
}

void test_get_struct_member_offset(void)
{
    STRUCT_TEST AAA;
    printf("percent = %d, laser_on_ms = %d\r\n", 
    GET_STRUCT_MEMBER_OFFSET(STRUCT_TEST, ctr_var.percent)/2, 
    GET_STRUCT_MEMBER_OFFSET(STRUCT_TEST, ctr_var.laser_on_ms)/2);

}

void main()
{
    //test_sprintf();
    //test_log_print();
    //test_memory_operate();
    //test_bcd_conv();
    //fifo_test();
    //test_time_counter();
    //test_get_struct_member_offset();
    //test_sin_unsin();
    //getchar();
    test_printf();
}