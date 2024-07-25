#ifndef MLIBC_PTHREAD_MUTEX_IMPL_H__
#define MLIBC_PTHREAD_MUTEX_IMPL_H__

#include <pthread.h>

int __mutex_init(__mutex_t mutex);
int __mutex_destroy(__mutex_t mutex);
int __mutex_lock(__mutex_t mutex);
int __mutex_trylock(__mutex_t mutex);
int __mutex_unlock(__mutex_t mutex);

#endif /* MLIBC_PTHREAD_MUTEX_IMPL_H__ */