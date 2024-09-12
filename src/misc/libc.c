/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/6/16      bernard      first version
 */
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include "../internal/stdio_impl.h"

extern FILE *stdout;
extern FILE *stdout;
extern FILE *stderr;

int __libc_init_array(void)
{
    return 0;
}

int _libc_init(void)
{
    extern void __mlibc_sys_heap_init(void);

    stdin->fd = STDIN_FILENO;
    stdout->fd = STDOUT_FILENO;
    stderr->fd = STDERR_FILENO;

    __mlibc_sys_heap_init();

    return 0;
}
