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

ldiv_t ldiv(long num, long den)
{
    return (ldiv_t){ num/den, num%den };
}