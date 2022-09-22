#include "memory_operate.h"

const uint32_t c_memory_wr_groups[MEMORY_WR_ADDR_GROUPS][2] = 
{
    {0x00000000, 0xFFFFFFFF}
    //{0x20000000, 0x20005000}
};

const uint32_t c_memory_ro_groups[MEMORY_RO_ADDR_GROUPS][2] = 
{
    {0x00000000, 0xFFFFFFFF}
    //{0x08000000, 0x08020000}
};

static memory_operate_callback __memory_callback = NULL;
static memory_operate_print __memory_print = NULL;

/**
 * @brief 校验地址是否符合规范
 * 
 * @param check_addr 被检验的地址
 * @param buffer 地址有效范围
 * @param number 有效地址的组数
 * @return uint8_t 0 失败 1 成功
 */
static uint8_t __memory_operation_addr_check(uint32_t check_addr, const uint32_t buffer[][2], uint8_t number)
{
    uint8_t i;

    for ( i = 0; i < number; i++)
    {
        if (__memory_print)
        {
            __memory_print("0x%4x, 0x%4x, 0x%4x\r\n", check_addr, buffer[i][0], buffer[i][1]);
        }
        //if ((check_addr < ((buffer + i)[0])) || (check_addr > ((buffer + i)[0])))
        if ((check_addr < buffer[i][0]) || (check_addr > buffer[i][1]))
        {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief 初始化相关函数指针
 * 
 * @param __callback 读写完数据后的回调函数
 * @param __print 内部打印函数，在读取数据时，将数据以一定格式打印出来
 */
void memory_operate_init(memory_operate_callback __callback, memory_operate_print __print)
{
    __memory_callback = __callback;
    __memory_print    = __print;
}

/**
 * @brief 写RAM函数，会强制更新dest所指向的数据，须判断dest地址是否为可写范围
 * 
 * @param dest 强制更新数据地址
 * @param src 数据源
 * @param length 更新长度，单位：字节
 * @return uint8_t 0 失败 1 成功
 */
uint8_t memory_write_byte(uint8_t *dest, const uint8_t *src, uint16_t length)
{
    uint16_t i;

    if ((0 == __memory_operation_addr_check((uint32_t)dest, c_memory_wr_groups, MEMORY_WR_ADDR_GROUPS)) || (NULL == src))
    {
        if (__memory_print)
        {
            __memory_print("Addr Fail\r\n");
        }
        return 0;/* 地址不符合规范 */
    }
    
    for (i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }

    if (__memory_print)
    {
        __memory_print("memory_write_byte Ok\r\n");
    }
    return 1;
}

/**
 * @brief 写RAM函数，会强制更新dest所指向的数据，须判断dest地址是否为可写范围
 * 
 * @param dest 强制更新数据地址
 * @param src 数据源
 * @param length 更新长度，单位：半字
 * @return uint8_t 0 失败 1 成功
 */
uint8_t memory_write_halfword(uint16_t *dest, const uint16_t *src, uint16_t length)
{
    uint16_t i;

    if ((0 == __memory_operation_addr_check((uint32_t)dest, c_memory_wr_groups, MEMORY_WR_ADDR_GROUPS)) || (NULL == src))
    {
        if (__memory_print)
        {
            __memory_print("Addr Fail\r\n");
        }
        return 0;/* 地址不符合规范 */
    }
    
    for (i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }

    if (__memory_print)
    {
        __memory_print("memory_write_halfword Ok\r\n");
    }
    return 1;
}

/**
 * @brief 写RAM函数，会强制更新dest所指向的数据，须判断dest地址是否为可写范围
 * 
 * @param dest 强制更新数据地址
 * @param src 数据源
 * @param length 更新长度，单位：字
 * @return uint8_t 0 失败 1 成功
 */
uint8_t memory_write_word(uint32_t *dest, const uint32_t *src, uint16_t length)
{
    uint16_t i;

    if ((0 == __memory_operation_addr_check((uint32_t)dest, c_memory_wr_groups, MEMORY_WR_ADDR_GROUPS)) || (NULL == src))
    {
        if (__memory_print)
        {
            __memory_print("Addr Fail\r\n");
        }
        return 0;/* 地址不符合规范 */
    }
    
    for (i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }

    if (__memory_print)
    {
        __memory_print("memory_write_word Ok\r\n");
    }
    return 1;
}

/**
 * @brief 读取RAM/FLASH指定地址的数据，缓存至目标地址内，如过读取回调函数不为空，则会打印出这些数据
 * 
 * @param dest 数据缓存目标地址
 * @param src 数据源，即读取数据的指定地址
 * @param length 读取长度，单位：字节
 * @return uint8_t 0 失败 1 成功
 */
uint8_t memory_read_byte(uint8_t *dest, const uint8_t *src, uint16_t length)
{
    uint16_t i, j;

    if ((NULL == dest) || (0 == __memory_operation_addr_check((uint32_t)dest, c_memory_wr_groups, MEMORY_WR_ADDR_GROUPS)) ||
                          (0 == __memory_operation_addr_check((uint32_t)dest, c_memory_ro_groups, MEMORY_RO_ADDR_GROUPS)))
    {
        if (__memory_print)
        {
            __memory_print("Addr Fail\r\n");
        }
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }
    
    /* 建议仅调试时打开，此处耗时严重 */
    if (__memory_print)
    {
        __memory_print("\r\nRead Data:\r\n");
        j = length / 5;
        for (i = 0; i < j; i++)
        {
            __memory_print("0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x\r\n", dest[i*5 + 0], dest[i*5 + 1], dest[i*5 + 2], 
                                                                dest[i*5 + 3], dest[i*5 + 4]);
        }
        j = length % 5;
        length /= 5;
        for (i = 0; i < j; i++)
        {
            __memory_print("0x%02x", dest[length*5 + i]);
        }
        __memory_print("\r\n");
    }
    return 1;
}

/**
 * @brief 读取RAM/FLASH指定地址的数据，缓存至目标地址内，如过读取回调函数不为空，则会打印出这些数据
 * 
 * @param dest 数据缓存目标地址
 * @param src 数据源，即读取数据的指定地址
 * @param length 读取长度，单位：字节
 * @return uint8_t 
 */
uint8_t memory_read_halfword(uint16_t *dest, const uint16_t *src, uint16_t length)
{
    uint16_t i, j;

    if ((NULL == dest) || (0 == __memory_operation_addr_check((uint32_t)dest, c_memory_wr_groups, MEMORY_WR_ADDR_GROUPS)) ||
                          (0 == __memory_operation_addr_check((uint32_t)dest, c_memory_ro_groups, MEMORY_RO_ADDR_GROUPS)))
    {
        if (__memory_print)
        {
            __memory_print("Addr Fail\r\n");
        }
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }
    
    /* 建议仅调试时打开，此处耗时严重 */
    if (__memory_print)
    {
        __memory_print("\r\nRead Data:\r\n");
        j = length / 5;
        for (i = 0; i < j; i++)
        {
            __memory_print("0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x\r\n", dest[i*5 + 0], dest[i*5 + 1], dest[i*5 + 2], 
                                                                dest[i*5 + 3], dest[i*5 + 4]);
        }
        j = length % 5;
        length /= 5;
        for (i = 0; i < j; i++)
        {
            __memory_print("0x%04x", dest[length*5 + i]);
        }
        __memory_print("\r\n");
    }
    return 1;
}

/**
 * @brief 读取RAM/FLASH指定地址的数据，缓存至目标地址内，如过读取回调函数不为空，则会打印出这些数据
 * 
 * @param dest 数据缓存目标地址
 * @param src 数据源，即读取数据的指定地址
 * @param length 读取长度，单位：字节
 * @return uint8_t 
 */
uint8_t memory_read_word(uint32_t *dest, const uint32_t *src, uint16_t length)
{
    uint16_t i, j;

    if ((NULL == dest) || (0 == __memory_operation_addr_check((uint32_t)dest, c_memory_wr_groups, MEMORY_WR_ADDR_GROUPS)) ||
                          (0 == __memory_operation_addr_check((uint32_t)dest, c_memory_ro_groups, MEMORY_RO_ADDR_GROUPS)))
    {
        if (__memory_print)
        {
            __memory_print("Addr Fail\r\n");
        }
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }
    
    /* 建议仅调试时打开，此处耗时严重 */
    if (__memory_print)
    {
        __memory_print("\r\nRead Data:\r\n");
        j = length / 5;
        for (i = 0; i < j; i++)
        {
            __memory_print("0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", dest[i*5 + 0], dest[i*5 + 1], dest[i*5 + 2], 
                                                                dest[i*5 + 3], dest[i*5 + 4]);
        }
        j = length % 5;
        length /= 5;
        for (i = 0; i < j; i++)
        {
            __memory_print("0x%08x", dest[length*5 + i]);
        }
        __memory_print("\r\n");
    }
    return 1;
}