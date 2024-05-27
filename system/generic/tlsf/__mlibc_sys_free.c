/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/27   0Bitbiscuits  the first version
 */
#include <libc_config.h>

#ifdef MLIBC_MEM_USING_TLSF

#include <sys/sys_mem.h>
#include <assert.h>
#include <compiler.h>
#include "inc/tlsf.h"

extern tlsf_t tlsf;

mlibc_weak void __mlibc_sys_free(void* ptr)
{
    tlsf_free(tlsf, ptr);
}

#endif /* MLIBC_MEM_USING_TLSF */