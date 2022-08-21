/**
 * @file pub_memory.c
 * @author YYF (NONE)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "pub_memory.h"


int pub_memcmp(const void *s1, const void *s2, uint16_t n) 
{
    const unsigned char *ss1 = s1, *ss2 = s2;
    while (n--) {
        int c = *ss1++ - *ss2++;
        if (c) {
            return c;
        }
    }
    return 0;
}

void *pub_memcpy(void *dest, const void *src, uint16_t n) 
{
    return pub_memmove(dest, src, n);
}

void *pub_memmove(void *dest, const void *src, uint16_t n) 
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (s < d && d < s + n) {
        // Need to copy backwards.
        d += n - 1;
        s += n - 1;
        while (n--) {
            *d-- = *s--;
        }
    } else {
        // Can copy forwards.
        while (n--) {
            *d++ = *s++;
        }
    }
    return dest;
}

void *pub_memset(void *s, int c, uint16_t n) 
{
    unsigned char *ss = s;
    while (n--) {
        *ss++ = c;
    }
    return s;
}

char *pub_strchr(const char *s, int c)
{
    while (*s) {
        if (*s == c) {
            return (char *)s;
        }
        ++s;
    }
    return NULL;
}

int pub_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        int c = *s1++ - *s2++;
        if (c) {
            return c;
        }
    }
    return *s1 - *s2;
}

uint16_t pub_strlen(const char *s) 
{
    const char *ss = s;
    while (*ss) {
        ++ss;
    }
    return ss - s;
}