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
#include <tlsf.h>
#include <arch_config.h>

MLIBC mlibc;
tlsf_t tlsf;

FILE* stdin = NULL;
FILE* stdout = NULL;
FILE* stderr = NULL;

static FILE stdin_file;
static FILE stdout_file;
static FILE stderr_file;



int _libc_init(void);

int __libc_init_array(void)
{
    mlibc.RUN_IN_OS = 1;
    if(!mlibc.RUN_IN_OS)
    {
        __mlibc_heap_nosys_init((void *)HEAP_BEGIN, (void *)HEAP_END);
    }

    return 0;
}

int _libc_init(void)
{
    
    stdin_file.fd = STDIN_FILENO;
    stdin = &stdin_file;
    
    stdout_file.fd = STDOUT_FILENO;
    stdout = &stdout_file;
    
    stderr_file.fd = STDERR_FILENO;
    stderr = &stderr_file;

    return 0;
}
