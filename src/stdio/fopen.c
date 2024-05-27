/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/24  0Bitbiscuits  the first version
 */
#include "../internal/stdio_impl.h"
#include <sys/sys_fio.h>
#include <fcntl.h>
#include <string.h>
#include <compiler.h>

/**
 * @brief 
 * 
 * @param path The file path
 * @param mode The file open mode.
 * support:
 * r: you can readonly
 * w: clean the file and writeonly
 * a: append the file
 * r+: you can read and write file
 * w+: file will clean first and you can read and write file
 * a+: append the file
 * @return FILE* A pointer to the FILE structure associated with the file descriptor.
 */
FILE *fopen(const char *path, const char *mode)
{
    FILE *f;
    int fd;
    int flags = 0;

    /* Check for valid initial mode character */
    if (!strchr("rwa", *mode)) {
        /* error operation */
        return 0;
    }

    /* Compute the flags to pass to open() */
    flags = __mlibc_fmode_to_flags(mode);

    fd = __mlibc_sys_open(path, flags);
    if (fd < 0) return 0;

    f = fdopen(fd, mode);
    if (f) return f;

    __mlibc_sys_close(fd);

    return NULL;
}