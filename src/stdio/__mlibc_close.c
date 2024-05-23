/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7    0Bitbiscuits  the first version
 */
#include "../internal/stdio_impl.h"
#include <sys/sys_fio.h>

/**
 * @brief call sys_close to close file
 * 
 * @param f FILE point
 * @return ssize_t 0 on successful, -1 on failed.
 */
ssize_t __mlibc_close(FILE *f)
{
    return __mlibc_sys_close(f->fd);
}
