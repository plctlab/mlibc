/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/23   0Bitbiscuits  the first version
 */
#include <libc_config.h>

#include <sys/sys_mem.h>
#include <assert.h>
#include <compiler.h>
#include "inc/tlsf.h"

extern tlsf_t tlsf;

void *__mlibc_sys_realloc(void* ptr, size_t size)
{
    return tlsf_realloc(tlsf, ptr, size);
}
