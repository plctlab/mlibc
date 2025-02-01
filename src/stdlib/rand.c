/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 * 2025/2/1   Chris-godz   the second version
 */
#include <stdlib.h>
#include <stdint.h>

static unsigned int __MLIBC_rand_next;

int rand_r(unsigned int* seed)
{
    return ((*seed + 1664525) * 1013904223) & INT32_MAX;
}

void srand(unsigned int seed)
{
    __MLIBC_rand_next = seed;
}

int rand(void)
{
    __MLIBC_rand_next = rand_r(&__MLIBC_rand_next);
    return __MLIBC_rand_next;
}
