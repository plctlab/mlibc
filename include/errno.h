/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */
#ifndef MLIBC_ERRNO_H__
#define MLIBC_ERRNO_H__

#include "sys/errno.h"

#ifdef __GNUC__
__attribute__((const))
#endif /* __GNUC__ */
int *__errno_location(void);

#ifndef errno
#define errno (*__errno_location())
#endif /* errno */

#endif /* MLIBC_ERRNO_H__ */