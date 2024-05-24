/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/6/16      bernard      first version
 */
#include <stdio.h>
#include <unistd.h>
#include <libc.h>
#include <libc_config.h>
#include "internal/tlsf.h"
#include <assert.h>
#include <fcntl.h>

#define HEAP_MGT_SIZE 2 * 1024

MLIBC mlibc;
tlsf_t tlsf;

extern FILE *stdout;
extern FILE *stdout;
extern FILE *stderr;

static unsigned char HEAP_MGT[HEAP_MGT_SIZE];

int __libc_init_array(void)
{
    mlibc.RUN_IN_OS = 1;

    return 0;
}

/* Initialize mlibc memory heap */
static tlsf_t __mlibc_heap_init(void *mem, size_t size)
{
    assert(size > tlsf_size() && "Need more memory to init heap management");
    
    return tlsf_create(mem);
}

int _libc_init(void)
{
    
    stdin->fd = STDIN_FILENO;
    stdout->fd = STDOUT_FILENO;
    stderr->fd = STDERR_FILENO;

    tlsf = __mlibc_heap_init(HEAP_MGT, HEAP_MGT_SIZE);

    return 0;
}
