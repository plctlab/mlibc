/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/22    0Bitbiscuits the first version
 */
/* Not tested and not supported on AArch64 */

// #include <spinlock.h>
// #include <sys/errno.h>

// int spin_init(spinlock_t *lock, int pshared)
// {
//     if (!lock)
//         return EINVAL;

//     atomic_store(&lock->lock, false);

//     return 0;
// }

// int spin_destroy(spinlock_t *lock)
// {
//     if (!lock)
//         return EINVAL;

//     return 0;
// }

// int spin_lock(spinlock_t *lock)
// {
//     bool expected = false;
//     while (!atomic_compare_exchange_weak_explicit(&lock->lock, &expected, true,
//             memory_order_acquire, memory_order_relaxed)) {
//         expected = false;
//     }
//     return 0;
// }

// int spin_trylock(spinlock_t *lock)
// {
//     bool expected = false;
//     if (!atomic_compare_exchange_weak_explicit(&lock->lock, &expected, true,
//         memory_order_acquire, memory_order_relaxed))
//     {
//         return 0;
//     }
//     return EBUSY;
// }

// int spin_unlock(spinlock_t *lock)
// {
//     atomic_store_explicit(&lock->lock, false, memory_order_release);

//     return 0;
// }