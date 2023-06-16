/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_STDBOOL_H__
#define MLIBC_STDBOOL_H__

#define __bool_true_false_are_defined   1

#ifndef __cplusplus

#define bool    _Bool
#define false   0
#define true    1

#endif /* __cplusplus */

#endif /*MLIBC_STDBOOL_H__*/
