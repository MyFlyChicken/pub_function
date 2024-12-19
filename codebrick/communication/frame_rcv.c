#include "frame_rcv.h"

void frame_rcv_process(frame_rcv_t* p_frame_rcv, unsigned char data)
{
    ASSERT(p_frame_rcv);
    ASSERT(p_frame_rcv->dest);
    ASSERT(p_frame_rcv->frame_length);
    ASSERT(p_frame_rcv->frame_header);
    ASSERT(p_frame_rcv->min_frame_len);
    ASSERT(p_frame_rcv->max_frame_len);
    ASSERT(p_frame_rcv->fsm_end);

    int            ret;
    unsigned char  val       = 0;
    unsigned short frame_len = 0; /* 由数据得到的帧长度 */

    switch (p_frame_rcv->steup) {
    case PARSE_FSM_HEAD:
        ret = p_frame_rcv->frame_header(data);
        if (0 == ret) {
            p_frame_rcv->dest[p_frame_rcv->dest_len++] = data;
        }
        else if (1 == ret) {
            p_frame_rcv->dest[p_frame_rcv->dest_len++] = data;
            p_frame_rcv->steup                         = PARSE_FSM_LEN;
        }
        else {
            LOG_PRINT("Data is invalid of PARSE_FSM_HEAD");
            p_frame_rcv->dest_len = 0;
        }
        break;
    case PARSE_FSM_LEN:
        ret = p_frame_rcv->frame_length(data, &frame_len);
        if (0 == ret) {
            p_frame_rcv->dest[p_frame_rcv->dest_len++] = data;
        }
        else if (1 == ret) {
            p_frame_rcv->dest[p_frame_rcv->dest_len++] = data;
            if ((frame_len > (*p_frame_rcv->max_frame_len))
                || (frame_len < (*p_frame_rcv->min_frame_len))) {
                LOG_PRINT("Frame len is invalid of PARSE_FSM_LEN");
                p_frame_rcv->dest_len = 0;
                p_frame_rcv->steup    = PARSE_FSM_HEAD;
            }
            else {
                p_frame_rcv->steup     = PARSE_FSM_END;
                p_frame_rcv->frame_len = frame_len;
            }
        }
        else {
            LOG_PRINT("Data is invalid of PARSE_FSM_LEN");
            p_frame_rcv->dest_len = 0;
            p_frame_rcv->steup    = PARSE_FSM_HEAD;
        }
        break;
    case PARSE_FSM_END:
        p_frame_rcv->dest[p_frame_rcv->dest_len++] = data;
        if (p_frame_rcv->dest_len >= p_frame_rcv->frame_len) {
            p_frame_rcv->fsm_end(p_frame_rcv->dest, p_frame_rcv->dest_len);
        }
        break;
    default:
        LOG_PRINT("Steup is err %d", p_frame_rcv->steup);
        p_frame_rcv->dest_len = 0;
        p_frame_rcv->steup    = PARSE_FSM_HEAD;
        break;
    }
}
