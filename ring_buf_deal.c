#include "ring_buf_deal.h"


void ring_buf_deal_main(ring_buf_deal_t *p_ring_buf_deal, unsigned char data)
{
    ASSERT(p_ring_buf_deal);
    ASSERT(p_ring_buf_deal->dest);
    ASSERT(p_ring_buf_deal->frame_length);
    ASSERT(p_ring_buf_deal->frame_header);
    ASSERT(p_ring_buf_deal->min_frame_len);
    ASSERT(p_ring_buf_deal->max_frame_len);
    ASSERT(p_ring_buf_deal->fsm_end);

    unsigned char  val       = 0;
    unsigned short frame_len = 0;/* 由数据得到的帧长度 */

    switch (p_ring_buf_deal->steup)
    {
    case PARSE_FSM_HEAD:
        if (0 == p_ring_buf_deal->frame_header(data))
        {
            p_ring_buf_deal->dest[p_ring_buf_deal->dest_len++] = data;
        }
        else if (1 == p_ring_buf_deal->frame_header(data))
        {
            p_ring_buf_deal->dest[p_ring_buf_deal->dest_len++] = data;
            p_ring_buf_deal->steup = PARSE_FSM_LEN;
        }
        else
        {
            LOG_PRINT("Data is invalid of PARSE_FSM_HEAD");
            p_ring_buf_deal->dest_len = 0;
        }
        break;
    case PARSE_FSM_LEN:
        if (0 == p_ring_buf_deal->frame_length(data, &frame_len))
        {
            p_ring_buf_deal->dest[p_ring_buf_deal->dest_len++] = data;
        }
        else if (1 == p_ring_buf_deal->frame_length(data, &frame_len))
        {
            p_ring_buf_deal->dest[p_ring_buf_deal->dest_len++] = data;
            if ((frame_len > (*p_ring_buf_deal->max_frame_len)) || (frame_len < (*p_ring_buf_deal->min_frame_len)))
            {
                LOG_PRINT("Frame len is invalid of PARSE_FSM_LEN");
                p_ring_buf_deal->dest_len = 0;
                p_ring_buf_deal->steup    = PARSE_FSM_HEAD;
            }
            else
            {
                p_ring_buf_deal->steup     = PARSE_FSM_END;
                p_ring_buf_deal->frame_len = frame_len;                
            }            
        }
        else
        {
            LOG_PRINT("Data is invalid of PARSE_FSM_LEN");
            p_ring_buf_deal->dest_len = 0;
            p_ring_buf_deal->steup    = PARSE_FSM_HEAD;
        }
        break;
    case PARSE_FSM_END:
        p_ring_buf_deal->dest[p_ring_buf_deal->dest_len++] = data;
        if (p_ring_buf_deal->dest_len >= p_ring_buf_deal->frame_len)
        {
            p_ring_buf_deal->fsm_end(p_ring_buf_deal);
        }
        break;
    default:
        LOG_PRINT("Steup is err %d", p_ring_buf_deal->steup);
        p_ring_buf_deal->dest_len = 0;
        p_ring_buf_deal->steup    = PARSE_FSM_HEAD;
        break;
    }
}

