#ifndef __mi_port_H_ //shift+U转换为大写
#define __mi_port_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_ARM) || defined(__GNUC__) /* ARM,GCC*/
#define LOG2(x) (32U - __builtin_clz(x))
__attribute__((always_inline)) static inline void __enable_irq(void)
{
    __asm volatile("cpsie i" : : : "memory");
}

__attribute__((always_inline)) static inline void __disable_irq(void)
{
    __asm volatile("cpsid i" : : : "memory");
}
#elif defined(__ICCARM__) /*IAR */
#define LOG2(x) (32U - CLZ(x))

#endif

#ifdef __cplusplus
}
#endif

#endif
