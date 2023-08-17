#ifndef __ERASE_BALANCE_H__
#define __ERASE_BALANCE_H__

#define EB_ASSERT_EN 1

#define LOG_PRINT(...) 

#if EB_ASSERT_EN
#define EB_ASSERT(x)    if (!(x))   \
                        {           \
                            LOG_PRINT("Assert failed, %s, %s", __FILE__, __LINE__); \
                            while (1);\
                        }
#else
#define EB_ASSERT(x)  
#endif

#define EB_FRAME_ADDR_START   (0)
#define EB_FRAME_ADDR_END     (4096U)
#define EB_FRAME_NEXT_DEFAULT (0xFFFFFFFF)

/* 不要随意调换顺序，带有wr前缀的成员，表明该成员是需要写入flash内部的 */
typedef struct 
{
    unsigned long wr_header;    /* 帧头 */
    unsigned long wr_len;       /* payload数据 */
    eb_frame_t * wr_next;       /* 下一个数据区，实际为数据区的地址 */
    const unsigned char * data; /* 用户赋值的数据 */
    eb_frame_t * curr;          /* 缓存数据 */
}eb_frame_t;


typedef struct 
{
    int (*flash_erase_all)(void);
    /* 接口函数写之前不进行擦除，因为均衡擦写就是避免频繁擦除的情况 */
    int (*flash_write)(unsigned long addr, unsigned long len, const unsigned char * data);
    int (*flash_read)(unsigned long addr, unsigned long len, unsigned char * data);
}eb_flash_inf_t;


int eb_init(const eb_flash_inf_t *eb_flash_inf,  eb_frame_t * eb_frame);
int eb_read_data(const eb_flash_inf_t *eb_flash_inf, eb_frame_t * eb_frame);
int eb_write_data(const eb_flash_inf_t *eb_flash_inf, eb_frame_t * prev, eb_frame_t * eb_frame);






























#endif
