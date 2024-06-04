#ifndef MLIBC_MEM_IMPL_H__
#define MLIBC_MEM_IMPL_H__

#include "tlsf.h"
#include <stdlib.h>
#include <sys/types.h>

extern tlsf_t tlsf;

void __mlibc_sys_heap_init(void);
void *sbrk(int incr);

#endif /* MLIBC_MEM_IMPL_H__*/
