/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_LIMITES_H__
#define MLIBC_LIMITES_H__

#include <stdint.h>
#include <inttypes.h>

#define CHAR_BIT    8
#define SCHAR_MIN   (-128)
#define SCHAR_MAX   127
#define UCHAR_MAX   255
#define SHRT_MIN    (-1 - 0x7fff)
#define SHRT_MAX    0x7fff
#define USHRT_MAX   0xffff
#define INT_MIN     (-1 - 0x7fffffff)
#define INT_MAX     0x7fffffff
#define UINT_MAX    0xffffffffU
#if (defined(__GNUC__) && (__SIZEOF_POINTER__ == 8))
#define __LONG_MAX INT64_MAX
#else
#define __LONG_MAX INT32_MAX
#endif /* defined(__GUNC__) */
#define LONG_MAX    __LONG_MAX
#define LONG_MIN    (-LONG_MAX - 1)
#define ULONG_MAX   (2UL * LONG_MAX + 1)
#define LLONG_MIN   (-LLONG_MAX - 1)
#define LLONG_MAX   0x7fffffffffffffffLL
#define ULLONG_MAX  (2ULL * LLONG_MAX + 1)

#endif /*MLIBC_LIMITES_H__*/
