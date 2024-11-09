/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_FCNTL_H__
#define MLIBC_FCNTL_H__

#define UNGET 8

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32
#define F_SVB 64
#define F_APP 128

#define FD_CLOEXEC 1

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_DSYNC      010000
#define O_SYNC     04010000
#define O_RSYNC    04010000
#define O_BINARY    0100000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_CLOEXEC  02000000

#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_LARGEFILE 0100000
#define O_NOATIME  01000000
#define O_PATH    010000000
#define O_TMPFILE 020200000
#define O_NDELAY O_NONBLOCK

#define O_SEARCH  O_PATH
#define O_EXEC    O_PATH

#define O_ACCMODE (03|O_SEARCH)

#define F_DUPFD  0
#define F_GETFD  1
#define F_SETFD  2
#define F_GETFL  3
#define F_SETFL  4

#define F_SETOWN 8
#define F_GETOWN 9
#define F_SETSIG 10
#define F_GETSIG 11

#define F_GETLK 12
#define F_SETLK 13
#define F_SETLKW 14

#define F_SETOWN_EX 15
#define F_GETOWN_EX 16

#define F_GETOWNER_UIDS 17

#define F_DUPFD_CLOEXEC 1030

#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2


struct flock {
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};


int open(const char *file, int flags, ...);
int openat(int, const char *, int, ...);

#endif /*MLIBC_FCNTL_H__*/
