/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_SYS_STAT_H__
#define MLIBC_SYS_STAT_H__

#include <stdint.h>
#include <time.h>

/* File types */
#define S_IFMT               00170000
#define S_IFSOCK             0140000
#define S_IFLNK              0120000
#define S_IFREG              0100000
#define S_IFBLK              0060000
#define S_IFDIR              0040000
#define S_IFCHR              0020000
#define S_IFIFO              0010000

/* File permission */
#define S_ISUID              0004000
#define S_ISGID              0002000
#define S_ISVTX              0001000

#define S_ISLNK(m)           (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)           (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)           (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)           (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)           (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)          (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)          (((m) & S_IFMT) == S_IFSOCK)

#define S_IRWXU              00700
#define S_IRUSR              00400
#define S_IWUSR              00200
#define S_IXUSR              00100

#define S_IRWXG              00070
#define S_IRGRP              00040
#define S_IWGRP              00020
#define S_IXGRP              00010

#define S_IRWXO              00007
#define S_IROTH              00004
#define S_IWOTH              00002
#define S_IXOTH              00001

/* stat structure */

struct stat
{
    dev_t st_dev;
    uint16_t  st_ino;
    uint16_t  st_mode;
    uint16_t  st_nlink;
    uint16_t  st_uid;
    uint16_t  st_gid;
    dev_t st_rdev;
    uint32_t  st_size;
    struct timespec    st_atim;
    long      st_spare1;
    struct timespec    st_mtim;
    long      st_spare2;
    struct timespec    st_ctim;
    long      st_spare3;
    uint32_t  st_blksize;
    uint32_t  st_blocks;
    long      st_spare4[2];
};

#define st_atime st_atim.tv_sec
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec

int mkdir(const char *, mode_t);
int stat(const char *__restrict, struct stat *__restrict);
int fstat(int, struct stat *);
int utimensat(int, const char *, const struct timespec [2], int);

#endif /*MLIBC_SYS_STAT_H__*/
