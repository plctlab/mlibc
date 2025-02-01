/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025/2/1   Chris-godz   the first version
 */
#include <stdlib.h>

extern void (*_Atfuns[])(void);
extern size_t _Atcount;

int atexit(void (*func)(void))
{
    if (_Atcount == 0)
    {
        return (-1);
    }
    _Atfuns[--_Atcount] = func;
    return (0);
}
