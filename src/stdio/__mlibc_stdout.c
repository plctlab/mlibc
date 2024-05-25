/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/23   0Bitbiscuits  the first version
 */
#include "../internal/stdio_impl.h"
#include <fcntl.h>

static unsigned char stdout_buf[BUFSIZ + UNGET];
FILE __stdout_FILE = {
	.fd = 0,
    .buf = stdout_buf + UNGET,
	.buf_size = BUFSIZ,
	.flags = F_PERM | F_NORD,
    .lbf = '\n',
	.write = __mlibc_write,
	.seek = __mlibc_lseek,
	.close = __mlibc_close,
};
FILE *stdout = &__stdout_FILE;