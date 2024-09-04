/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
 */
#ifndef MLIBC_COMPILER_H__
#define MLIBC_COMPILER_H__

#if defined(__ARMCC_VERSION)        /* ARM Compiler */
#define mlibc_section(x)               __attribute__((section(x)))
#define mlibc_used                     __attribute__((used))
#define mlibc_align(n)                 __attribute__((aligned(n)))
#define mlibc_weak                     __attribute__((weak))
#define mlibc_neck
#define mlibc_typeof                   typeof
#define mlibc_noreturn
#define mlibc_inline                   static __inline
#define mlibc_always_inline            mlibc_inline
#elif defined (__IAR_SYSTEMS_ICC__) /* for IAR Compiler */
#define mlibc_section(x)               @ x
#define mlibc_used                     __root
#define PRAGMA(x)                      _Pragma(#x)
#define mlibc_align(n)                 PRAGMA(data_alignment=n)
#define mlibc_weak                     __weak
#define mlibc_neck
#define mlibc_typeof                   __typeof
#define mlibc_noreturn
#define mlibc_inline                   static inline
#define mlibc_always_inline            mlibc_inline
#elif defined (__GNUC__)            /* GNU GCC Compiler */
#define mlibc_section(x)               __attribute__((section(x)))
#define mlibc_used                     __attribute__((used))
#define mlibc_align(n)                 __attribute__((aligned(n)))
#define mlibc_weak                     __attribute__((weak))
#define mlibc_neck                     __attribute__((naked))
#define mlibc_typeof                   __typeof__
#define mlibc_noreturn                 __attribute__ ((noreturn))
#define mlibc_inline                   static __inline
#define mlibc_always_inline            static inline __attribute__((always_inline))
#else                              /* Unkown Compiler */
    #error not supported tool chain
#endif /* __ARMCC_VERSION */

#endif /* MLIBC_COMPILER_H__ */
