/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/22   0Bitbiscuits the first version
 */
#ifndef MLIBC_STDATOMIC_H__
#define MLIBC_STDATOMIC_H__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <compiler.h>
#include <sys/features.h>

/**
 * @brief judge which atomic session we choice
 */
#if __GNUC_PREREQ__(4, 7)
#define __GNUC_ATOMICS
#else
#define __HW_ATOMICS
#endif

/**
 * @brief atomic type definition
 */
#ifdef __GNUC_ATOMICS
#define mlibc_atomic(type)          _Atomic(type)
#else
#define mlibc_atomic(type)          \
struct                              \
{                                   \
    volatile type value;            \
}                    
#endif /* __HW_ATOMICS */

typedef mlibc_atomic(bool)               atomic_bool;
typedef mlibc_atomic(char)               atomic_char;
typedef mlibc_atomic(signed char)        atomic_schar;
typedef mlibc_atomic(unsigned char)      atomic_uchar;
typedef mlibc_atomic(short)              atomic_short;
typedef mlibc_atomic(unsigned short)     atomic_ushort;
typedef mlibc_atomic(int)                atomic_int;
typedef mlibc_atomic(unsigned int)       atomic_uint;
typedef mlibc_atomic(long)               atomic_long;
typedef mlibc_atomic(unsigned long)      atomic_ulong;
typedef mlibc_atomic(long long)          atomic_llong;
typedef mlibc_atomic(unsigned long long) atomic_ullong;
typedef mlibc_atomic(uintptr_t)          atomic_uintptr_t;
typedef mlibc_atomic(size_t)             atomic_size_t;
typedef mlibc_atomic(ptrdiff_t)          atomic_ptrdiff_t;


/**
 * @brief atomic operation type definition
 */
#ifndef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED        0
#endif
#ifndef __ATOMIC_CONSUME
#define __ATOMIC_CONSUME        1
#endif
#ifndef __ATOMIC_ACQUIRE
#define __ATOMIC_ACQUIRE        2
#endif
#ifndef __ATOMIC_RELEASE
#define __ATOMIC_RELEASE        3
#endif
#ifndef __ATOMIC_ACQ_REL
#define __ATOMIC_ACQ_REL        4
#endif
#ifndef __ATOMIC_SEQ_CST
#define __ATOMIC_SEQ_CST        5
#endif

typedef enum {
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
} memory_order;

/**
 * @brief Definition of explicit atomic operations
 */
#ifdef __GNUC_ATOMICS
#define atomic_compare_exchange_strong_explicit(object, expected,  \
    desired, success, failure)                                     \
    __atomic_compare_exchange_n(object, expected,                  \
        desired, 0, success, failure)

#define atomic_compare_exchange_weak_explicit(object, expected,    \
    desired, success, failure)                                     \
    __atomic_compare_exchange_n(object, expected,                  \
        desired, 1, success, failure)

#define atomic_exchange_explicit(object, desired, order)           \
    __atomic_exchange_n(object, desired, order)

#define atomic_fetch_add_explicit(object, operand, order)          \
    __atomic_fetch_add(object, operand, order)

#define atomic_fetch_and_explicit(object, operand, order)          \
    __atomic_fetch_and(object, operand, order)

#define atomic_fetch_or_explicit(object, operand, order)           \
    __atomic_fetch_or(object, operand, order)

#define atomic_fetch_sub_explicit(object, operand, order)          \
    __atomic_fetch_sub(object, operand, order)

#define atomic_fetch_xor_explicit(object, operand, order)          \
    __atomic_fetch_xor(object, operand, order)

#define atomic_load_explicit(object, order)                        \
    __atomic_load_n(object, order)

#define atomic_store_explicit(object, desired, order)              \
    __atomic_store_n(object, desired, order)
#else
#error "mlibc realization base on arch(todo)"
#endif /* EXPLICIT ATOMIC OPERATION */

/**
 * @brief Definition of hidden atomic operations
 */
#define atomic_compare_exchange_strong(object, expected, desired)  \
    atomic_compare_exchange_strong_explicit(object, expected,      \
        desired, memory_order_seq_cst, memory_order_seq_cst)

#define atomic_compare_exchange_weak(object, expected, desired)    \
    atomic_compare_exchange_weak_explicit(object, expected,        \
        desired, memory_order_seq_cst, memory_order_seq_cst)

#define atomic_exchange(object, desired)                           \
    atomic_exchange_explicit(object, desired, memory_order_seq_cst)

#define atomic_fetch_add(object, operand)                          \
    atomic_fetch_add_explicit(object, operand, memory_order_seq_cst)

#define atomic_fetch_and(object, operand)                          \
    atomic_fetch_and_explicit(object, operand, memory_order_seq_cst)

#define atomic_fetch_or(object, operand)                           \
    atomic_fetch_or_explicit(object, operand, memory_order_seq_cst)

#define atomic_fetch_sub(object, operand)                          \
    atomic_fetch_sub_explicit(object, operand, memory_order_seq_cst)

#define atomic_fetch_xor(object, operand)                          \
    atomic_fetch_xor_explicit(object, operand, memory_order_seq_cst)

#define atomic_load(object)                                        \
    atomic_load_explicit(object, memory_order_seq_cst)

#define atomic_store(object, desired)                              \
    atomic_store_explicit(object, desired, memory_order_seq_cst)

/**
 * @brief Atomic flag type and operations.
 */
typedef struct {
    atomic_bool __flag;
} atomic_flag;

mlibc_inline bool
atomic_flag_test_and_set_explicit(volatile atomic_flag *__object,
    memory_order __order)
{
    return (atomic_exchange_explicit(&__object->__flag, 1, __order));
}

mlibc_inline void
atomic_flag_clear_explicit(volatile atomic_flag *__object, memory_order __order)
{
    atomic_store_explicit(&__object->__flag, 0, __order);
}

mlibc_inline bool
atomic_flag_test_and_set(volatile atomic_flag *__object)
{
    return (atomic_flag_test_and_set_explicit(__object,
        memory_order_seq_cst));
}

mlibc_inline void
atomic_flag_clear(volatile atomic_flag *__object)
{
    atomic_flag_clear_explicit(__object, memory_order_seq_cst);
}

#endif /* MLIBC_STDATOMIC_H__ */