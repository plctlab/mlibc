/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/24   0Bitbiscuits  the first version
 */
#include <stdlib.h>
#include "../internal/stdio_impl.h"

int fclose(FILE *f)
{
    int res = 0;

    FLOCK(f);
    res = fflush(f);
    res |= f->close(f);
    FUNLOCK(f);

    __mlibc_file_remove(f);

    FLOCK_DEINIT(f);
    free(f->getln_buf);
    free(f);

    return 0;
}