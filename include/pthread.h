/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/9   0Bitbiscuits  the first version
 */
#ifndef MLIBC_PTHREAD_H__
#define MLIBC_PTHREAD_H__

#include <sys/types.h>
#include <time.h>
#include <sys/syscall.h>

typedef long pthread_t;                 /* pthread id*/
typedef long pthread_condattr_t;        /* properties of sem */
typedef long pthread_mutexattr_t;       /* properties of mutex */

typedef int pthread_key_t;              /* store the identifier for thread-specific data keys */
typedef int pthread_once_t;             /* execute tag */

struct sched_param
{
    int sched_priority;                 /* thread priority */
};

struct pthread_attr
{
    void* stackaddr;                    /* stack address of thread */
    int   stacksize;                    /* stack size of thread */

    int   inheritsched;                 /* Inherit parent prio/policy */
    int   schedpolicy;                  /* scheduler policy */
    struct sched_param schedparam;      /* sched parameter */

    int   detachstate;                  /* detach state */
};
typedef struct pthread_attr pthread_attr_t;

struct pthread_mutex
{
    pthread_mutexattr_t attr;           /* Store the properties of the mutex lock. */
    mutex_t lock;                       /* system mutex */
};
typedef struct pthread_mutex pthread_mutex_t;

struct pthread_cond
{
    pthread_condattr_t attr;            /* Store the properties of the cond */
    sem_t sem;                          /* system sem */
};
typedef struct pthread_cond pthread_cond_t;

/* pthread attribute */
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/* pthread-control */
int pthread_cancel(pthread_t thread);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int pthread_detach(pthread_t thread);
int pthread_equal(pthread_t t1, pthread_t t2);
void pthread_exit(void *value_ptr);
int pthread_join(pthread_t thread, void **value_ptr);
pthread_t pthread_self(void);

/* pthread-cond */
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
/* pthread-cond-attr */
int pthread_condattr_destroy(pthread_condattr_t *attr);
int pthread_condattr_getclock(const pthread_condattr_t *attr, clockid_t *clock_id);
int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);

/* pthread-mutex */
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
/* pthread-mutex-attr */
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

/* pthread-cancel */
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
int pthread_testcancel(void);
void pthread_cleanup_pop(int execute);
void pthread_cleanup_push(void (*routine)(void *), void *arg);

/* pthread specific data */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
int pthread_key_delete(pthread_key_t key);
void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void *value);

/* pthread current cond level */
int pthread_getconcurrency(int *level);
int pthread_setconcurrency(int level);

/* pthread sched param */
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);
int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
int pthread_setschedprio(pthread_t thread, int prio);

/* others */
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
int pthread_getcpuclockid(pthread_t thread, clockid_t *clock_id);


#endif /* MLIBC_PTHREAD_H__ */
