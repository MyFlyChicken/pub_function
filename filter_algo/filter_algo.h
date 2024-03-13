#ifndef __filter_algo_H_ //shift+U转换为大写
#define __filter_algo_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/* ----------------------- Defines ------------------------------------------*/
#define MOVING_AVERAGE_FILTER_SIZE (5U)  // 设定滤波窗口大小

/* ----------------------- Type definitions ---------------------------------*/


/* ----------------------- Extern variable ---------------------------------*/



/* ----------------------- Extern function ----------------------------------*/
int16_t filter1(void);
int16_t filter2(int16_t num);
int16_t filter3(int16_t num);
int16_t filter4(void);
int16_t filter5(int16_t num);
int16_t filter6(int16_t prev_value, int16_t curr_value, int16_t ratio);
int16_t filter7(int16_t prev_value, uint8_t ratio1, uint8_t ratio2);
int16_t filter8(int16_t *weights, int16_t weights_num, int16_t sa_num);

int16_t filter_inf_register(void *(*get_currval)(unsigned char num));


/* ----------------------- End ----------------------------------------------*/



#ifdef __cplusplus
}
#endif


#endif
