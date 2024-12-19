#ifndef __poll_def_H_ //shift+U转换为大写
#define __poll_def_H_

#ifdef __cplusplus
extern "C" {
#endif

/* basic data type ********************************************************* */
typedef unsigned long  poll_u32_t;
typedef unsigned short poll_u16_t;
typedef unsigned char  poll_u8_t;
typedef signed long    poll_s32_t;
typedef signed short   poll_s16_t;
typedef signed char    poll_s8_t;

typedef enum {
    poll_false = 0,
    poll_true  = !poll_false,
} poll_bool_e;

#define POLL_NULL ((void*)0)

#define POLL_U32_MAX (0xFFFFFFFF)
#define POLL_U32_MIN (0)
#define POLL_U16_MAX (0xFFFF)
#define POLL_U16_MIN (0)

#define POLL_PI (3.1415926535)

/* anony define ************************************************************ */
#define ANONY_CONN(type, var, line)   type var##line
#define ANONY_DEF(type, prefix, line) ANONY_CONN(type, prefix, line)
#define ANONY_TYPE(type, prefix)      ANONY_DEF(type, prefix, __LINE__)

/* tool chain config ******************************************************* */
#if defined(__CC_ARM) || defined(__GNUC__) /* ARM,GCC*/
#define SECTION(x) __attribute__((section(x)))
#define UNUSED     __attribute__((unused))
#define USED       __attribute__((used))
#define ALIGN(n)   __attribute__((aligned(n)))
#define WEAK       __attribute__((weak))
#elif defined(__ICCARM__) /*IAR */
#define SECTION(x) @x
#define UNUSED
#define USED __root
#define WEAK __weak
#else
#error "Current tool chain haven't supported yet!"
#endif

#ifdef __cplusplus
}
#endif

#endif
