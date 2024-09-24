#ifndef __poll_port_H_ //shift+U转换为大写
#define __poll_port_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "poll_def.h"

poll_u32_t poll_get_tick(void);

void* poll_malloc(poll_u32_t size);
void  poll_free(void* memory);

#ifdef __cplusplus
}
#endif

#endif
