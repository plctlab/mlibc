/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/26  0Bitbiscuits the first version
 */
#include <stdlib.h>
#include <compiler.h>

mlibc_weak void exit(int status)
{
    fflush(NULL);
    abort();
}
