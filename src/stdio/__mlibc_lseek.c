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
#include <unistd.h>

/**
 * @brief set file point position and get position after setting
 * 
 * @param fd the file descriptor
 * @param offset the offset of file point
 * @param whence the operation type of offset
 * SEEK_SET    0   set file offset to offset
 * SEEK_CUR    1   set file offset to current plus offset
 * SEEK_END    2   set file offset to EOF plus offset
 * @return off_t current file point position
 */
off_t __mlibc_lseek(FILE *f, off_t offset, int whence)
{
    return lseek(f->fd, offset, whence);
}
