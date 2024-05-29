/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/29   0Bitbiscuits  realize sys_close
 */
#include <libc_config.h>
#include <sys/sys_fio.h>
#include <compiler.h>
#include <assert.h>

#ifndef MLIBC_USING_SYSTEM
#define HEAP_DEFAULT_SIZE   1024 * 10
#define MLIBC_HEAP_START    (&MLIBC_HEAP[0])
#define MLIBC_HEAP_END      (&MLIBC_HEAP[HEAP_DEFAULT_SIZE - 1])

static char MLIBC_HEAP[HEAP_DEFAULT_SIZE];
static char *cur_point = MLIBC_HEAP_START;
static char *prev_point = NULL;

mlibc_weak void *__mlibc_heap_sbrk(size_t size)
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

    return (void *)prev_point;
}


mlibc_weak void __mlibc_heap_free(void *mem)
{
    assert(0);
}

mlibc_weak int __mlibc_sys_close(int fd)
{
    (void)fd;

    return 0;
}

mlibc_weak int __mlibc_sys_fcntl(int fd, int cmd, ...)
{
    (void)fd;
    (void)cmd;
    
    return 0;
}

mlibc_weak int __mlibc_sys_ioctl(int fd, int cmd, ...)
{
    (void)fd;
    (void)cmd;

    return -1;
}

mlibc_weak off_t __mlibc_sys_lseek(int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    
    return -1;
}

mlibc_weak int __mlibc_sys_open(const char *path, int flags, ...)
{
    (void)path;
    (void)flags;
    
    return -1;
}

mlibc_weak ssize_t __mlibc_sys_read(int fd, void* buf, size_t buf_size)
{
    (void)fd;
    (void)buf;
    (void)buf_size;

    return -1;
}

mlibc_weak ssize_t __mlibc_sys_write(int fd, void *buf, size_t buf_size)
{
    (void)fd;
    (void)buf;
    (void)buf_size;

    return -1;
}

#endif /* MLIBC_USING_SYSTEM */