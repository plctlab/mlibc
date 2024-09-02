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

#ifndef MLIBC_RETARGETABLE_LOCKING
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;

#define __LOCK_INIT(class,lock)             static int lock = 0;
#define __LOCK_INIT_RECURSIVE(class,lock)   static int lock = 0;
#define __lock_init(lock)                   ((void) 0)
#define __lock_init_recursive(lock)         ((void) 0)
#define __lock_deinit(lock)                  ((void) 0)
#define __lock_deinit_recursive(lock)        ((void) 0)
#define __lock_take(lock)                ((void) 0)
#define __lock_take_recursive(lock)      ((void) 0)
#define __lock_try_take(lock)            ((void) 0)
#define __lock_try_take_recursive(lock)  ((void) 0)
#define __lock_release(lock)                ((void) 0)
#define __lock_release_recursive(lock)      ((void) 0)

#else

#ifdef __cplusplus
extern "C" {
#endif

struct __lock;
typedef struct __lock * _LOCK_T;
#define _LOCK_RECURSIVE_T _LOCK_T

#define __LOCK_INIT(class,lock) extern struct __lock __lock_ ## lock; \
    class _LOCK_T lock = &__lock_ ## lock
#define __LOCK_INIT_RECURSIVE(class,lock) __LOCK_INIT(class,lock)

/* Lock init */
extern int __retarget_lock_init(_LOCK_T *lock);
#define __lock_init(lock) __retarget_lock_init(&lock)
extern int __retarget_lock_init_recursive(_LOCK_T *lock);
#define __lock_init_recursive(lock) __retarget_lock_init_recursive(&lock)

/* Lock deinit */
extern int __retarget_lock_deinit(_LOCK_T lock);
#define __lock_deinit(lock) __retarget_lock_deinit(lock)
extern int __retarget_lock_deinit_recursive(_LOCK_T lock);
#define __lock_deinit_recursive(lock) __retarget_lock_deinit_recursive(lock)

/* Lock take */
extern int __retarget_lock_take(_LOCK_T lock, int timeout);
#define __lock_take(lock) __retarget_lock_take(lock, 0)
extern int __retarget_lock_take_recursive(_LOCK_T lock);
#define __lock_take_recursive(lock) __retarget_lock_take_recursive(lock)

/* Lock try take */
extern int __retarget_lock_try_take(_LOCK_T lock);
#define __lock_try_take(lock) __retarget_lock_try_take(lock)
extern int __retarget_lock_try_take_recursive(_LOCK_T lock);
#define __lock_try_take_recursive(lock) \
  __retarget_lock_try_take_recursive(lock)

/* Lock release */
extern int __retarget_lock_release(_LOCK_T lock);
#define __lock_release(lock) __retarget_lock_release(lock)
extern int __retarget_lock_release_recursive(_LOCK_T lock);
#define __lock_release_recursive(lock) __retarget_lock_release_recursive(lock)

#ifdef __cplusplus
}
#endif

#endif /* MLIBC_RETARGETABLE_LOCKING */

#endif /* MLIBC_LOCK_H__ */