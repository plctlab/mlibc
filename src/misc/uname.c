/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <sys/utsname.h>
#include <stdint.h>
#include <string.h>

int uname(struct utsname *uts)
{
    char *sysname = "RT-Thread";

    if (uts)
    {
        memcpy(uts->sysname, sysname, strlen(sysname));
    }

    return 0;
}
