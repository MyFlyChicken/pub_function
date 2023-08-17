
#include "erase_balance.h"

/**
 * @brief 均衡擦写开始前必须调用该函数，用于擦除flash内的全部数据
 * @param eb_flash_inf 
 * @param eb_frame 
 * @return 
 */
int eb_init(const eb_flash_inf_t *eb_flash_inf,  eb_frame_t * eb_frame)
{
    EB_ASSERT(eb_flash_inf);
    EB_ASSERT(eb_flash_inf->flash_erase_all);

    return eb_flash_inf->flash_erase_all();
}

/**
 * @brief 读取上次写入的帧
 * @param eb_flash_inf 
 * @param eb_frame 
 * @return 
 */
int eb_read_data(const eb_flash_inf_t *eb_flash_inf, eb_frame_t * eb_frame)
{
    unsigned char flag = 0;
    unsigned char buf[16];
    unsigned long addr;
    eb_frame_t * p_eb_frame;

    EB_ASSERT(eb_flash_inf);
    EB_ASSERT(eb_frame);
    EB_ASSERT(eb_flash_inf->flash_read);

    addr = EB_FRAME_ADDR_START;
    /* 查找数据 */
    while (!flag)
    {
        eb_flash_inf->flash_read(addr, 16, buf);
        p_eb_frame = (eb_frame_t *)buf;
        /* 最后一帧 */
        if (p_eb_frame->wr_next == EB_FRAME_NEXT_DEFAULT)
        {
            eb_frame->wr_header = p_eb_frame->wr_header;
            eb_frame->wr_len  = p_eb_frame->wr_len;
            eb_frame->wr_next = p_eb_frame->wr_next;
            eb_frame->data = addr + 12;
            eb_frame->curr = addr;
            return 0;
        }
        addr = p_eb_frame->wr_next;
        if (addr > (EB_FRAME_ADDR_END - 12))
        {
            return -1;
        }
    }
}

/**
 * @brief 写数据前，必须把eb_frame_t的next成员赋值为EB_FRAME_NEXT_DEFAULT
 * @param eb_flash_inf 
 * @param prev 上一个帧的地址
 * @param eb_frame 
 * @return 
 */
int eb_write_data(const eb_flash_inf_t *eb_flash_inf, eb_frame_t * prev, eb_frame_t * eb_frame)
{
    unsigned long addr;
    EB_ASSERT(eb_flash_inf);
    EB_ASSERT(eb_frame);
    EB_ASSERT(eb_flash_inf->flash_write);
    EB_ASSERT(eb_flash_inf->flash_erase_all);

    /* 写入next成员值 */
    addr = prev->curr;

    /* 擦除重写数据 */
    if ((addr + prev->wr_len + 12) <= EB_FRAME_ADDR_END)
    {
        eb_flash_inf->flash_write(addr + 8, sizeof(prev->curr), addr + prev->wr_len + 12);
        /* 写入帧头 长度 next，next为EB_FRAME_NEXT_DEFAULT */
        addr = addr + prev->wr_len + 12;
        eb_flash_inf->flash_write(addr, 12, (const unsigned char *)eb_frame);        
    }
    else
    {
        eb_flash_inf->flash_erase_all();
        addr = 0;
        eb_flash_inf->flash_write(addr, 12, (const unsigned char *)eb_frame);  
    }
    /* 写入数据data */
    addr = addr + 12;
    eb_flash_inf->flash_write(addr, eb_frame->wr_len, (const unsigned char *)eb_frame->data);

    return 0;
}
