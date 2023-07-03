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

FILE* stdin = NULL;
FILE* stdout = NULL;
FILE* stderr = NULL;

static FILE stdin_file;
static FILE stdout_file;
static FILE stderr_file;

int __libc_init_array(void)
{
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
