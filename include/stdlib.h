/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021/02/17     Bernard      first version
 */

#ifndef MLIBC_STDLIB_H__
#define MLIBC_STDLIB_H__

#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#ifndef a_ctz_32
#define a_ctz_32 a_ctz_32
static inline int a_ctz_32(uint32_t x)
{
#ifdef a_clz_32
    return 31-a_clz_32(x&-x);
#else
    static const char debruijn32[32] = {
        0, 1, 23, 2, 29, 24, 19, 3, 30, 27, 25, 11, 20, 8, 4, 13,
        31, 22, 28, 18, 26, 10, 7, 12, 21, 17, 9, 6, 16, 5, 15, 14
    };
    return debruijn32[(x&-x)*0x076be629 >> 27];
#endif
}
#endif

#ifndef a_ctz_64
#define a_ctz_64 a_ctz_64
static inline int a_ctz_64(uint64_t x)
{
    static const char debruijn64[64] = {
        0, 1, 2, 53, 3, 7, 54, 27, 4, 38, 41, 8, 34, 55, 48, 28,
        62, 5, 39, 46, 44, 42, 22, 9, 24, 35, 59, 56, 49, 18, 29, 11,
        63, 52, 6, 26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
    };
    if (sizeof(long) < 8) {
        uint32_t y = x;
        if (!y) {
            y = x>>32;
            return 32 + a_ctz_32(y);
        }
        return a_ctz_32(y);
    }
    return debruijn64[(x&-x)*0x022fdd63cc95386dull >> 58];
}
#endif

static inline int a_ctz_l(unsigned long x)
{
    return (sizeof(long) < 8) ? a_ctz_32(x) : a_ctz_64(x);
}

#define _IMPLEMENT_ABS(type, x) \
{ \
    type mask = x >> (sizeof(type) * CHAR_BIT - 1); \
    return (x + mask) ^ mask; \
}

#define EXIT_SUCCESS    (0)
#define EXIT_FAILURE    (1)

typedef unsigned long long ullong_type;
typedef long long llong_type;

typedef struct
{
    int quot;
    int rem;
} div_t;

typedef struct
{
    long quot;
    long rem;
} ldiv_t;

typedef struct
{
    long long quot;
    long long rem;
} lldiv_t;

int abs(int x);
double atof(const char *str);
int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);
char *itoa(int num, char *str,int radix);
div_t div(int num, int den);
long labs(long a);
ldiv_t ldiv(long num, long den);
long long llabs(long long a);
lldiv_t lldiv(long long num, long long den);
int rand_r(unsigned int* seed);
void srand(unsigned int seed);
int rand(void);
void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t size);
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));
void *bsearch (const void *key, const void *base, size_t nelem, size_t elem_size, int (*cmp) (const void *, const void *));
double strtod(char *str, char **ptr);
long strtol(const char *nptr, char **endptr, int base);
llong_type strtoll(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
ullong_type strtoull(const char *nptr, char **endptr, int base);
#endif /*MLIBC_STDLIB_H__*/
