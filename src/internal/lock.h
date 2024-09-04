/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/16  0Bitbiscuits the first version
 */
#ifndef MLIBC_LOCK_H__
#define MLIBC_LOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

struct __lock;
typedef struct __lock * _LOCK_T;
#define _LOCK_RECURSIVE_T _LOCK_T

/**
 * @brief Reentrant Lock Initialization Interface
 * 
 * @param lock Reentrant Lock pointer
 * @return int 
 * Return the operation status. ONLY When the return value is EOK, the operation is successful.
 * If the return value is any other values, it means that the mutex take failed.
 */
extern int __retarget_lock_init(_LOCK_T *lock);
#define __lock_init(lock) __retarget_lock_init(&lock)
extern int __retarget_lock_init_recursive(_LOCK_T *lock);
#define __lock_init_recursive(lock) __retarget_lock_init_recursive(&lock)

/**
 * @brief Reentrant Lock Destroy Interface
 * 
 * @param lock Reentrant Lock pointer
 * @return int 
 * Return the operation status. ONLY When the return value is EOK, the operation is successful.
 * If the return value is any other values, it means that the mutex take failed.
 */
extern int __retarget_lock_deinit(_LOCK_T lock);
#define __lock_deinit(lock) __retarget_lock_deinit(lock)
extern int __retarget_lock_deinit_recursive(_LOCK_T lock);
#define __lock_deinit_recursive(lock) __retarget_lock_deinit_recursive(lock)

/**
 * @brief Reentrant Lock Take Interface
 * 
 * @param lock Reentrant Lock pointer
 * @return int 
 * Return the operation status. ONLY When the return value is EOK, the operation is successful.
 * If the return value is any other values, it means that the mutex take failed.
 */
extern int __retarget_lock_take(_LOCK_T lock);
#define __lock_take(lock) __retarget_lock_take(lock)
extern int __retarget_lock_take_recursive(_LOCK_T lock);
#define __lock_take_recursive(lock) __retarget_lock_take_recursive(lock)

/**
 * @brief Reentrant Lock Release Interface
 * 
 * @param lock Reentrant Lock pointer
 * @return int 
 * Return the operation status. ONLY When the return value is EOK, the operation is successful.
 * If the return value is any other values, it means that the mutex take failed.
 */
extern int __retarget_lock_release(_LOCK_T lock);
#define __lock_release(lock) __retarget_lock_release(lock)
extern int __retarget_lock_release_recursive(_LOCK_T lock);
#define __lock_release_recursive(lock) __retarget_lock_release_recursive(lock)

#ifdef __cplusplus
}
#endif

#endif /* MLIBC_LOCK_H__ */