/**
 * @file pp_list.h
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-12-11
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-12-11 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */

#ifndef inc_pp_list_h
#define inc_pp_list_h

#ifdef __cplusplus
extern "C" {
#endif

struct list_s {
    struct list_s *next, *prev;
};

#define list_for_each(pos, head) for (struct list_s* pos = (head)->next; pos != (head); pos = pos->next)

void list_init(struct list_s* list);
void list_add(struct list_s* lnew, struct list_s* lhead);
void list_del(struct list_s* pos);

#ifdef __cplusplus
}
#endif

#endif /* inc_pp_list_h */
