#ifndef MLIBC_SPINLOCK_H__
#define MLIBC_SPINLOCK_H__

#include <stdatomic.h>

typedef struct _spinlock
{
    atomic_int lock;
} spinlock_t;

#endif /* MLIBC_SPINLOCK_H__ */