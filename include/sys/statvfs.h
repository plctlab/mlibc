/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/8   0Bitbiscuits the first version
 */
#ifndef MLIBC_SYS_STATVFS_H__
#define MLIBC_SYS_STATVFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <alltypes.h>

struct statvfs {
    unsigned long f_bsize, f_frsize;
    fsblkcnt_t f_blocks, f_bfree, f_bavail;
    fsfilcnt_t f_files, f_ffree, f_favail;
    unsigned :8*(2*sizeof(int)-sizeof(long));
    unsigned long f_fsid;
    unsigned long f_flag, f_namemax;
    unsigned int f_type;
    int __reserved[5];
};

int statvfs (const char *__restrict, struct statvfs *__restrict);
int fstatvfs (int, struct statvfs *);

#ifdef __cplusplus
}
#endif

#endif
