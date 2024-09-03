/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/13  0Bitbiscuits the first version
 */
#include <errno.h>
#include <compiler.h>

static int internal_errno;

mlibc_weak int *__errno_location(void)
{
    return &internal_errno;
}