#include <libc_config.h>

#ifdef MLIBC_RUNNING_BARE

#include <sys/sys_mem.h>
#include <stdint.h>
#include <assert.h>

#define HEAP_DEFAULT_SIZE   1024 * 8
#define MLIBC_HEAP_START    (&MLIBC_HEAP[0])
#define MLIBC_HEAP_END      (&MLIBC_HEAP[HEAP_DEFAULT_SIZE - 1])

static char MLIBC_HEAP[HEAP_DEFAULT_SIZE];
static char *cur_point = MLIBC_HEAP_START;
static char *prev_point = NULL;

void *__mlibc_sbrk(ptrdiff_t size)
{
    if(cur_point == NULL)
    {
        cur_point = MLIBC_HEAP_START;
    }

    if((size < 0) || (cur_point + size > MLIBC_HEAP_END) || (cur_point + size < MLIBC_HEAP_START))
    {
        return NULL;
    }

    prev_point = cur_point;
    cur_point += size;

    rt_kprintf("malloc from system\n");

    return (void *)prev_point;
}

void __mlibc_free(void *mem)
{
    assert(0);
}

#endif /* MLIBC_RUNNING_BARE */