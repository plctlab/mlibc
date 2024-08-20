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
#include <fcntl.h>

#ifndef __stdout_FILE
static FILE *volatile __stdout_FILE = 0;
#endif
#ifndef __stderr_FILE
static FILE *volatile __stderr_FILE = 0;
#endif

/**
 * @brief If the parameter is null, it flushes the buffers of all files;
 * if a file is specified, it flushes the buffer of the specified file.
 * 
 * @param f file pointer
 * @return int 
 */
int fflush(FILE *f)
{
    if (!f) {
        int r = 0;
        if (__stdout_FILE) r |= fflush(__stdout_FILE);
        if (__stderr_FILE) r |= fflush(__stderr_FILE);

        for (f = *__ofl_lock(); f; f = f->next)
        {
            FLOCK(f);
            if (f->wpos != f->wbase) r |= fflush(f);
            FUNLOCK(f);
        }
        __ofl_unlock();

        return r;
    }

    FLOCK(f);

    /* If writing, flush buffer */
    if (f->wpos != f->wbase) {
        f->write(f, 0, 0);
        if (!f->wpos) {
            FUNLOCK(f);
            return EOF;
        }
    }

    /* If reading, sync position, per POSIX */
    if (f->rpos != f->rend) f->seek(f, f->rpos-f->rend, SEEK_CUR);

    /* Clear read and write modes */
    f->wpos = f->wbase = f->wend = 0;
    f->rpos = f->rend = 0;

    FUNLOCK(f);
    return 0;
}