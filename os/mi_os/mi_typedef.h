#ifndef __mi_typedef_H_ //shift+U转换为大写
#define __mi_typedef_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char  mi_u8_t;
typedef unsigned short mi_u16_t;
typedef unsigned long  mi_u32_t;

typedef signed char  mi_s8_t;
typedef signed short mi_s16_t;
typedef signed long  mi_s32_t;

typedef void (*idle_hook)(void);

typedef enum {
    mi_true  = 1,
    mi_false = !mi_true
} mi_bool;

typedef enum {
    mi_ok = 0,

} mi_status_t;

#ifdef __cplusplus
}
#endif

#endif
