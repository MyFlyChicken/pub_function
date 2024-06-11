#ifndef __polling_H_ //shift+U转换为大写
#define __polling_H_

#include "poll_def.h"
#include "poll_config.h"
#include "poll_port.h"
#include "poll_module.h"

#ifdef __cplusplus
extern "C" {
#endif

void poll_module_init(void);
void poll_process(void);

#ifdef __cplusplus
}
#endif

#endif
