/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_UNISTD_H__
#define MLIBC_UNISTD_H__

#include <sys/types.h>

#define _POSIX_VDISABLE 0

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#define F_OK    0
#define R_OK    4
#define W_OK    2
#define X_OK    1

int pipe(int [2]);
int pipe2(int [2], int);
int creat(const char *path, mode_t mode);
int close(int d);
int read(int fd, void *buf, size_t len);
int fsync(int fildes);

ssize_t write(int fd, const void *buf, size_t count);

/*
* long sysconf(int name);
* Gets the configuration information executed by the system
*
* size_t confstr(int name, char *buf, size_t len);
* Gets the value of a string variable that depends on configuration
*/

off_t lseek(int fd, off_t offset, int whence);
int unlink(const char *pathname);

char *getcwd(char *buf, size_t size);
int fcntl(int fd, int cmd, ... /* arg */ );
int ioctl(int fildes, int cmd, ...);

int chdir(const char *path);
int mkdir(const char *pathname, mode_t mode);
int rmdir(const char *pathname);
int truncate(const char *, off_t);
int ftruncate(int, off_t);
int access(const char *, int);
int faccessat(int, const char *, int, int);

#include <sys/stat.h>

#endif /*MLIBC_UNISTD_H__*/
