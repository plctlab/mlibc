#ifndef MLIBC_MEM_H__
#define MLBIC_MEM_H__

#include <sys/types.h>
typedef struct mlibc_mem_t
{
    ubase_t address;
	size_t used;
	size_t max;
	size_t total;
} mem_t;

mem_t *mlibc_mem_init(void* addr, size_t size);
void *mlibc_malloc(mem_t *m, size_t size);
void mlibc_mem_free(void *rmem);

#endif
