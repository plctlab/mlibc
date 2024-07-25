/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/25  0Bitbiscuits the first version
 */
#include "../internal/semaphore_impl.h"
#include <sys/types.h>
#include <errno.h>

/* Record the changes of the system semaphore counting device */
#define SEM_ACQUIRE sem->value > 0 ? ((sem->value)--) : ((sem->suspended)++)
#define SEM_RELEASE sem->suspended > 0 ? ((sem->suspended)--) : ((sem->value)++)

int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    (void)pshared;
    sem->value = value;
    sem->suspended = 0;
    return __sem_init(sem->sys_sem, value);
}

int sem_destroy(sem_t *sem)
{
    int ret = 0;

    if(sem == NULL)
        return -EINVAL;
    
    sem->value = 0;
    sem->suspended = 0;
    ret = sem_destroy(sem->sys_sem);
    sem->sys_sem = NULL;

    return ret;
}

int sem_wait(sem_t *sem)
{
    int ret = 0;

    if(sem == NULL)
        return -EINVAL;

    ret = __sem_wait(sem->sys_sem);
    if(ret == EOK)
    {
        SEM_ACQUIRE;
    }

    return ret;
}

int sem_trywait(sem_t *sem)
{
    int ret = 0;

    if(sem == NULL)
        return -EINVAL;

    ret = __sem_trywait(sem->sys_sem);
    if(ret == EOK)
    {
        SEM_ACQUIRE;
    }

    return ret;
}

// int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout); todo

int sem_post(sem_t *sem)
{
    int ret = 0;

    if(sem == NULL)
        return -EINVAL;

    ret = __sem_post(sem->sys_sem);
    if(ret == EOK)
    {
        SEM_RELEASE;
    }

    return ret;
}