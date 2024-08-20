/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/20  0Bitbiscuits the first version
 */
#include "../internal/stdio_impl.h"
#include <errno.h>
#include <fcntl.h>

int __fseeko_unlocked(FILE *f, off_t off, int whence)
{
    /* Fail immediately for invalid whence argument. */
    if (whence != SEEK_CUR && whence != SEEK_SET && whence != SEEK_END) {
        errno = EINVAL;
        return -1;
    }

    /* Adjust relative offset for unread data in buffer, if any. */
    if (whence == SEEK_CUR && f->rend) off -= f->rend - f->rpos;

    /* Flush write buffer, and report error on failure. */
    if (f->wpos != f->wbase) {
        f->write(f, f->wbase, f->wpos - f->wbase);
        if (!f->wpos) return -1;
    }

    /* Leave writing mode */
    f->wpos = f->wbase = f->wend = 0;

    /* Perform the underlying seek. */
    if (f->seek(f, off, whence) < 0) return -1;

    /* If seek succeeded, file is seekable and we discard read buffer. */
    f->rpos = f->rend = 0;
    f->flags &= ~F_EOF;
    
    return 0;
}

int fseek(FILE *f, long off, int whence)
{
    int result;
    FLOCK(f);
    result = __fseeko_unlocked(f, off, whence);
    FUNLOCK(f);
    return result;
}