/**
 * @file main.c
 * @author yuyf ()
 * @brief
 * @version 0.1
 * @date 2023-04-05
 *
 * @copyright Copyright (c) 2023 xxxxxxx公司
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-04-05 <td>v1.0     <td>chen     <td>内容
 * </table>
 */

#include "public.h"
#include <stdio.h>

//

typedef enum _TEST_ENUM1_
{
    AAA = 0X444444,
} TEST_ENUM1;

typedef enum _TEST_ENUM2_
{
    BBB = 0X12,
} TEST_ENUM2;

typedef enum __BL_TRIG_MODE__
{
    BL_MODE_EXTERN_CONTINUE = 0,
    BL_MODE_EXTERN_PULSE,
    BL_MODE_SERIAL_PORT,
    BL_MODE_FAIL,
    BL_MODE_INVALID
} BL_TRIG_MODE;
typedef struct __BL_CTRL_VAR__
{
    uint8_t percent;      /* 出光功率占比 */
    uint8_t laser_status; /* 激光器状态，仅在模式为串口连续模式时进行控制 0关闭 1打开*/
    // uint8_t rsvd1;
    // uint8_t rsvd2;
    uint32_t delay_gap_ms; /* 出光延长间隔，单位ms */
    uint32_t laser_on_ms;  /* 激光打开时间，单位ms，因为laser_on_cnt计数会在delay_gap_cnt清0之后清0，所以需要加上delay_gap_ms */
    volatile uint32_t delay_gap_cnt;
    volatile uint32_t laser_on_cnt;
} BL_CTRL_VAR __attribute__((packed));
typedef struct __LD_LOGIC_VAR__
{
    uint8_t boot_flag;
    uint8_t fan_work_temp; /* 风扇启动，默认为25 */
    uint8_t ls_over_temp;  /* 保护温度，默认为70 */
    uint8_t print_status;  /* 打印状态 */
    BL_TRIG_MODE bl_run_mode;
    BL_CTRL_VAR bl_var;
} LD_LOGIC_VAR __attribute__((packed)); /*aligned*/

LD_LOGIC_VAR g_ld_logic_var;
void fifo_test()
{
#define FIFO_BUFFER_SIZE 50
    uint16_t i, j, temp;
    FIFO _fifo;
    fifo_buffer_type pop_buffer[FIFO_BUFFER_SIZE] = {0};
    const fifo_buffer_type mystring[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
#if 1
    fifo_init(&_fifo, pop_buffer, FIFO_BUFFER_SIZE);
    pub_memset(_fifo.pbuffer, 0, FIFO_BUFFER_SIZE * sizeof(fifo_buffer_type));
    for (j = 0; j < FIFO_BUFFER_SIZE; j++)
    {
        /* code */
        printf("%3d", *(_fifo.pbuffer + j));
    }
    printf("\nInit end\r\n");
    for (i = 0; i < 10; i++)
    {
        temp = fifo_push_bytes(&_fifo, mystring, sizeof(mystring) / sizeof(fifo_buffer_type));
        // printf("%d---head = %d, tail = %d, temp = %d\r\n", i, _fifo.head, _fifo.tail, temp);
        for (j = 0; j < FIFO_BUFFER_SIZE; j++)
        {
            /* code */
            printf("%3d", *(_fifo.pbuffer + j));
        }
        printf("\r\n");
    }
    printf("pop1---head = %d, tail = %d, %d\r\n", _fifo.head, _fifo.tail, fifo_pop_bytes(&_fifo, pop_buffer, FIFO_BUFFER_SIZE));
    for (j = 0; j < FIFO_BUFFER_SIZE; j++)
    {
        if (*(_fifo.pbuffer + ((j + _fifo.tail) % FIFO_BUFFER_SIZE)) != pop_buffer[j])
        {
            printf("1read fail! %d,\r\n", j);
            break;
        }
    }
    pub_memset(pop_buffer, 0, FIFO_BUFFER_SIZE * sizeof(fifo_buffer_type));
    printf("pop2---head = %d, tail = %d, %d\r\n", _fifo.head, _fifo.tail, fifo_pop_bytes_clear(&_fifo, pop_buffer, FIFO_BUFFER_SIZE));
    for (j = 0; j < FIFO_BUFFER_SIZE; j++)
    {
        if (*(_fifo.pbuffer + ((j + _fifo.tail) % FIFO_BUFFER_SIZE)) != pop_buffer[j])
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
        printf("%d", (val >> i) & 0x01);
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
    uint8_t byte_buf[6] = {0};
    uint16_t halfword_buf[6] = {0};
    uint32_t word_buf[6] = {0};

    uint8_t byte_buf1[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
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
    uint8_t a = 0x55;
    uint16_t b = 0x255;
    uint32_t c = 0x77;
    uint8_t aa = 55;
    uint16_t bb = 166;
    uint32_t cc = 77;
    printf("%d, %d, %d\r\n", pub_bcd2dec(a, sizeof(a)), pub_bcd2dec(b, sizeof(b)), pub_bcd2dec(c, sizeof(c)));
    printf("0x%x, 0x%x, 0x%x\r\n", pub_dec2bcd(aa), pub_dec2bcd(bb), pub_dec2bcd(cc));
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
    printf("%f, %lf, %hu\r\n", 11.22, 1.2222333333333, 4294967295);
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
    uint8_t buffer[sizeof(LD_LOGIC_VAR)];
    LD_LOGIC_VAR *p_ld_logic = (LD_LOGIC_VAR *)buffer;

    buffer[0] = 10;
    buffer[1] = 20;
    buffer[2] = 30;
    buffer[3] = 40;
    buffer[4] = 0x01;
    buffer[5] = 0x01;
    buffer[6] = 0x01;
    buffer[7] = 0x01;
    buffer[4] = 0x01;
    buffer[5] = 0x01;
    buffer[6] = 0x01;
    buffer[7] = 0x01;
    buffer[8] = 50;
    buffer[9] = 60;
    buffer[10] = 70;
    buffer[11] = 80;
    buffer[12] = 0x02;
    buffer[13] = 0x02;
    buffer[14] = 0x02;
    buffer[15] = 0x02;
    buffer[16] = 0x04;
    buffer[17] = 0x04;
    buffer[18] = 0x04;
    buffer[19] = 0x04;
    buffer[20] = 0x08;
    buffer[21] = 0x08;
    buffer[22] = 0x08;
    buffer[23] = 0x08;
    printf("sizeof = %d, boot_flag = %d, fan_work_temp = %d, ls_over_temp = %d, print_status = %d,                                      \
				percent = %d, delay_gap_ms = %d, laser_on_ms = %d, run_mode = %d\r\n",
           sizeof(LD_LOGIC_VAR), p_ld_logic->boot_flag,
           p_ld_logic->fan_work_temp, p_ld_logic->ls_over_temp, p_ld_logic->print_status, p_ld_logic->bl_var.percent,
           p_ld_logic->bl_var.delay_gap_ms, p_ld_logic->bl_var.laser_on_ms, p_ld_logic->bl_run_mode);
}

void main()
{
    // test_sprintf();
    // test_log_print();
    // test_memory_operate();
    // test_bcd_conv();
    fifo_test();
    // test_time_counter();
    // test_get_struct_member_offset();
    // test_sin_unsin();
    // getchar();
    // test_printf();
}