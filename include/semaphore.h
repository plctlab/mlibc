#ifndef MLIBC_SEMAPHORE_H__
#define MLIBC_SEMAPHORE_H__

#include <time.h>

typedef void * __sem_t;
typedef struct
{
    unsigned int value;
    unsigned int suspended;
    __sem_t sys_sem;
} sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
// int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout); todo
int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem, int *sval);

#endif /* MLIBC_SEMAPHORE_H__ */