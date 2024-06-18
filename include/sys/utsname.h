/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-27     xqyjlj       add uname
 */

#ifndef MLIBC_SYS_UTSNAME_H__
#define MLIBC_SYS_UTSNAME_H__

#ifdef __cplusplus
extern "C" {
#endif

struct utsname
{
    char sysname[65];
    char nodename[65];
    char release[65];
    char version[65];
    char machine[65];
    char domainname[65];
};

int uname(struct utsname *);

#ifdef __cplusplus
}
#endif

#endif /* MLIBC_SYS_UTSNAME_H__ */
