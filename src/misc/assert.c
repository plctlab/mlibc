/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/7/03      bernard      first version
 */

#include <stdio.h>
#include <stdlib.h>

void __assert_fail (const char* expr, const char* file, int line)
{
    fprintf(stderr, "%s:%d: Assertion failed: %s\n", file, line, expr);
    while(1)
    {
        ;
    }
}
