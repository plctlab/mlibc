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
#include <tlsf.h>
#include <assert.h>

#define HEAP_MGT_SIZE 2 * 1024

MLIBC mlibc;
tlsf_t tlsf;

FILE* stdin = NULL;
FILE* stdout = NULL;
FILE* stderr = NULL;

static FILE stdin_file;
static FILE stdout_file;
static FILE stderr_file;

static char HEAP_MGT[HEAP_MGT_SIZE];

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
    
    stdin_file.fd = STDIN_FILENO;
    stdin = &stdin_file;
    
    stdout_file.fd = STDOUT_FILENO;
    stdout = &stdout_file;
    
    stderr_file.fd = STDERR_FILENO;
    stderr = &stderr_file;

    tlsf = __mlibc_heap_init(HEAP_MGT, HEAP_MGT_SIZE);

    return 0;
}
