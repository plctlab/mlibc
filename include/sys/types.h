/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/9   0Bitbiscuits  the first version
 */

#ifndef MLIBC_SYS_TYPES_H__
#define MLIBC_SYS_TYPES_H__

typedef signed int       clockid_t;
typedef signed int       key_t;         /* Used for interprocess communication. */
typedef int              pid_t;         /* Used for process IDs and process group IDs. */
typedef unsigned short   uid_t;
typedef unsigned short   gid_t;
typedef signed long      off_t;
typedef int              mode_t;
typedef unsigned long    size_t;

#if (defined(__GNUC__) && (__SIZEOF_POINTER__ == 8))
typedef long signed int  ssize_t;       /* Used for a count of bytes or an error indication. */
#else
typedef signed int       ssize_t;       /* Used for a count of bytes or an error indication. */
#endif /* __GNUC__ */
typedef unsigned long    __timer_t;
typedef __timer_t        timer_t;
typedef unsigned long    useconds_t;    /* microseconds (unsigned) */

typedef unsigned long    dev_t;

typedef unsigned int     u_int;
typedef unsigned char    u_char;
typedef unsigned long    u_long;

#endif /*MLIBC_SYS_TYPES_H__*/
