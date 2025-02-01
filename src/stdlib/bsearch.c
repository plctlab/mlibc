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

void *bsearch(const void *key, const void *base, size_t nelem, size_t elem_size, int (*cmp)(const void *, const void *))
{
    int result = 0;
    void *current = NULL;
    size_t left = 0;
    size_t right = nelem;
    size_t mid = 0;

    if (nelem == 0 || elem_size == 0)
        return NULL;

    while (left < right)
    {
        mid = (left + right) >> 1;
        current = (void *)(((uint8_t *)base) + (mid * elem_size));

        result = cmp(key, current);

        if (result < 0)
            right = mid;
        else if (result > 0)
            left = mid + 1;
        else
            return current;
    }

    return NULL;
}
