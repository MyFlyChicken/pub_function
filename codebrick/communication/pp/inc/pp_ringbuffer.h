/**
 * @file pp_ringbuffer.h
 * @author 
 * @brief 
 * @version 0.1
 * @date 2025-06-27
 * 
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2025-06-27 <td>v1.0     <td>     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */

#ifndef inc_pp_ringbuffer_h
#define inc_pp_ringbuffer_h

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    typedef struct
    {
        uint8_t* buffer_ptr;

        /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     *
     * The tradeoff is we could only use 64KiB of buffer for 16 bit of index.
     * But it should be enough for most of the cases.
     */
        uint32_t read_mirror  : 1;
        uint32_t read_index   : 31;
        uint32_t write_mirror : 1;
        uint32_t write_index  : 31;

        /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
        int64_t buffer_size;
    } ringbuffer_t;

    typedef enum
    {
        RINGBUFFER_EMPTY,
        RINGBUFFER_FULL,

        /* half full is neither full nor empty */
        RINGBUFFER_HALFFULL,
    } ringbuffer_state_e;

    void               ringbuffer_init(ringbuffer_t* rb, uint8_t* pool, int64_t size);
    void               ringbuffer_reset(ringbuffer_t* rb);
    uint32_t           ringbuffer_put(ringbuffer_t* rb, const uint8_t* ptr, uint32_t length);
    uint32_t           ringbuffer_put_force(ringbuffer_t* rb, const uint8_t* ptr, uint32_t length);
    uint32_t           ringbuffer_putchar(ringbuffer_t* rb, const uint8_t ch);
    uint32_t           ringbuffer_putchar_force(ringbuffer_t* rb, const uint8_t ch);
    uint32_t           ringbuffer_get(ringbuffer_t* rb, uint8_t* ptr, uint32_t length);
    uint32_t           ringbuffer_peek(ringbuffer_t* rb, uint8_t* ptr, uint32_t length);
    uint32_t           ringbuffer_getchar(ringbuffer_t* rb, uint8_t* ch);
    uint32_t           ringbuffer_data_len(ringbuffer_t* rb);
    uint32_t           ringbuffer_get_size(ringbuffer_t* rb);
    ringbuffer_state_e ringbuffer_status(ringbuffer_t* rb);

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_ringbuffer_h */
