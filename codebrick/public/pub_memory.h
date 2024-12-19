/**
 * @file pub_memory.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __PUB_MEMORY_H__
#define  __PUB_MEMORY_H__

#include <stdint.h>




int pub_memcmp(const void *s1, const void *s2, uint16_t n);
void *pub_memcpy(void *dest, const void *src, uint16_t n) ;
void *pub_memmove(void *dest, const void *src, uint16_t n) ;
void *pub_memset(void *s, int c, uint16_t n);
char *pub_strchr(const char *s, int c);
int pub_strcmp(const char *s1, const char *s2);
uint16_t pub_strlen(const char *s);







#endif /* __PUB_MEMORY_H__ */