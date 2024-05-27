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

/**
 * @brief If the parameter is null, it flushes the buffers of all files;
 * if a file is specified, it flushes the buffer of the specified file.
 * 
 * @param f file pointer
 * @return int 
 */
int fflush(FILE *f)
{
    int r = 0;

    /* If f is null, flush all file buffer */
    if(!f)
    {
        if(stdout)
            r |= fflush(stdout);
        if(stderr)
            r |= fflush(stderr);

        for(f = __mlibc_file_head(); f; f = f->next)
        {
            /* use write buffer */
            if(f->wpos != f->wbase)
            {
                r |= fflush(f);
            }
        }

        return r;
    }

    /* f is not null */
    /* If writing, flush output */
    if(f->wpos != f->wbase)
    {
        f->write(f, 0, 0);
        if(!f->wpos)
        {
            return EOF;
        }
    }

    /* If reading, sync position to the end of file */
    if (f->rpos != f->rend) f->seek(f, f->rpos-f->rend, SEEK_CUR);

    /* Clear read and write modes */
    f->wpos = f->wbase = f->wend = 0;
    f->rpos = f->rend = 0;

    return 0;
}