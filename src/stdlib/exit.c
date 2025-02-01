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
#include <compiler.h>
#include <unistd.h>

#define NATS	32
void (*_Atfuns[NATS])(void) = {0};
size_t _Atcount = {NATS};

mlibc_weak void exit(int status)
{
    while (_Atcount < NATS)
    {
        (*_Atfuns[_Atcount++])();
    }
    fflush(NULL);
    _exit(status);
}
