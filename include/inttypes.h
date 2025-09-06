/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_INTTYPES_H__
#define MLIBC_INTTYPES_H__

#include <stdint.h>

typedef signed long long        intmax_t;
typedef unsigned long long      uintmax_t;

typedef signed long long        int_fast64_t;
typedef unsigned long long      uint_fast64_t;

typedef signed int          int_fast32_t;
typedef unsigned int        uint_fast32_t;

typedef signed int          int_fast16_t;
typedef unsigned int        uint_fast16_t;

typedef signed int          int_fast8_t;
typedef unsigned int        uint_fast8_t;

typedef struct
{
    intmax_t quot;
    intmax_t rem;
} imaxdiv_t;

#if UINTPTR_MAX == UINT64_MAX
#define __PRI64 "l"
#define __PRIPTR "l"
#else
#define __PRI64 "ll"
#define __PRIPTR ""
#endif

#define PRId8 "hhd"
#define PRId16 "hd"
#define PRId32 "d"
#define PRId64 "lld"

#define PRIdLEAST8 "hhd"
#define PRIdLEAST16 "hd"
#define PRIdLEAST32 "d"
#define PRIdLEAST64 __PRI64 "d"

#define PRIdFAST8 "hhd"
#define PRIdFAST16 "d"
#define PRIdFAST32 "d"
#define PRIdFAST64 __PRI64 "d"

#define PRIi8 "hhi"
#define PRIi16 "hi"
#define PRIi32 "i"
#define PRIi64 __PRI64 "i"

#define PRIiLEAST8 "hhi"
#define PRIiLEAST16 "h"
#define PRIiLEAST32 "i"
#define PRIiLEAST64 __PRI64 "i"

#define PRIiFAST8 "hhi"
#define PRIiFAST16 "i"
#define PRIiFAST32 "i"
#define PRIiFAST64 __PRI64 "i"

#define PRIo8 "hho"
#define PRIo16 "ho"
#define PRIo32 "o"
#define PRIo64 __PRI64 "o"

#define PRIoLEAST8 "hho"
#define PRIoLEAST16 "ho"
#define PRIoLEAST32 "o"
#define PRIoLEAST64 __PRI64 "o"

#define PRIoFAST8 "hho"
#define PRIoFAST16 "o"
#define PRIoFAST32 "o"
#define PRIoFAST64 __PRI64 "o"

#define PRIu8 "hhu"
#define PRIu16 "hu"
#define PRIu32 "u"
#define PRIu64 "llu"

#define PRIuLEAST8 "hhu"
#define PRIuLEAST16 "hu"
#define PRIuLEAST32 "u"
#define PRIuLEAST64 __PRI64 "u"

#define PRIuFAST8 "hhu"
#define PRIuFAST16 "u"
#define PRIuFAST32 "u"
#define PRIuFAST64 __PRI64 "u"

#define PRIx8 "hhx"
#define PRIx16 "hx"
#define PRIx32 "x"
#define PRIx64 __PRI64 "x"

#define PRIxLEAST8 "hhx"
#define PRIxLEAST16 "hx"
#define PRIxLEAST32 "x"
#define PRIxLEAST64 __PRI64 "x"

#define PRIxFAST8 "hhx"
#define PRIxFAST16 "x"
#define PRIxFAST32 "x"
#define PRIxFAST64 __PRI64 "x"

#define PRIX8 "hhX"
#define PRIX16 "hX"
#define PRIX32 "X"
#define PRIX64 __PRI64 "X"

#define PRIXLEAST8 "hhX"
#define PRIXLEAST16 "hX"
#define PRIXLEAST32 "X"
#define PRIXLEAST64 __PRI64 "X"

#define PRIXFAST8 "hhX"
#define PRIXFAST16 "X"
#define PRIXFAST32 "X"
#define PRIXFAST64 __PRI64 "X"

#define PRIdMAX __PRI64 "d"
#define PRIiMAX __PRI64 "i"
#define PRIoMAX __PRI64 "o"
#define PRIuMAX __PRI64 "u"
#define PRIxMAX __PRI64 "x"
#define PRIXMAX __PRI64 "X"

#define PRIdPTR __PRIPTR "d"
#define PRIiPTR __PRIPTR "i"
#define PRIoPTR __PRIPTR "o"
#define PRIuPTR __PRIPTR "u"
#define PRIxPTR __PRIPTR "x"
#define PRIXPTR __PRIPTR "X"

#ifndef DISABLE_UNIMPLEMENTED_LIBC_APIS
#define SCNd8 "hhd"
#define SCNd16 "hd"
#define SCNd32 "d"
#define SCNd64 __PRI64 "d"

#define SCNdLEAST8 "hhd"
#define SCNdLEAST16 "hd"
#define SCNdLEAST32 "d"
#define SCNdLEAST64 __PRI64 "d"

#define SCNdFAST8 "hhd"
#define SCNdFAST16 "d"
#define SCNdFAST32 "d"
#define SCNdFAST64 __PRI64 "d"

#define SCNi8 "hhi"
#define SCNi16 "hi"
#define SCNi32 "i"
#define SCNi64 __PRI64 "i"

#define SCNiLEAST8 "hhi"
#define SCNiLEAST16 "hi"
#define SCNiLEAST32 "i"
#define SCNiLEAST64 __PRI64 "i"

#define SCNiFAST8 "hhi"
#define SCNiFAST16 "i"
#define SCNiFAST32 "i"
#define SCNiFAST64 __PRI64 "i"

#define SCNu8 "hhu"
#define SCNu16 "hu"
#define SCNu32 "u"
#define SCNu64 __PRI64 "u"

#define SCNuLEAST8 "hhu"
#define SCNuLEAST16 "hu"
#define SCNuLEAST32 "u"
#define SCNuLEAST64 __PRI64 "u"

#define SCNuFAST8 "hhu"
#define SCNuFAST16 "u"
#define SCNuFAST32 "u"
#define SCNuFAST64 __PRI64 "u"

#define SCNo8 "hho"
#define SCNo16 "ho"
#define SCNo32 "o"
#define SCNo64 __PRI64 "o"

#define SCNoLEAST8 "hho"
#define SCNoLEAST16 "ho"
#define SCNoLEAST32 "o"
#define SCNoLEAST64 __PRI64 "o"

#define SCNoFAST8 "hho"
#define SCNoFAST16 "o"
#define SCNoFAST32 "o"
#define SCNoFAST64 __PRI64 "o"

#define SCNx8 "hhx"
#define SCNx16 "hx"
#define SCNx32 "x"
#define SCNx64 __PRI64 "x"

#define SCNxLEAST8 "hhx"
#define SCNxLEAST16 "hx"
#define SCNxLEAST32 "x"
#define SCNxLEAST64 __PRI64 "x"

#define SCNxFAST8 "hhx"
#define SCNxFAST16 "x"
#define SCNxFAST32 "x"
#define SCNxFAST64 __PRI64 "x"

#define SCNdMAX __PRI64 "d"
#define SCNiMAX __PRI64 "i"
#define SCNoMAX __PRI64 "o"
#define SCNuMAX __PRI64 "u"
#define SCNxMAX __PRI64 "x"

#define SCNdPTR __PRIPTR "d"
#define SCNiPTR __PRIPTR "i"
#define SCNoPTR __PRIPTR "o"
#define SCNuPTR __PRIPTR "u"
#define SCNxPTR __PRIPTR "x"
#endif // DISABLE_UNIMPLEMENTED_LIBC_APIS

#endif /*MLIBC_INTTYPES_H__*/
