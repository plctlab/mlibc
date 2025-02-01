/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 * 2025/2/1   Chris-godz   the second version
 */
#include <stdlib.h>
#include <ctype.h>

int atoi (const char *s)
{
  return (int) strtol (s, NULL, 10);
}

long atol (const char *s)
{
  return strtol (s, NULL, 10);
}

long long atoll (const char *str)
{
	return strtoll(str, (char **)NULL, 10);
}
