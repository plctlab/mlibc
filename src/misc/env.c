/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

char **__environ = 0;
void __env_rm_add(char *old, char *new)
{
    static char **env_alloced;
    static size_t env_alloced_n;
    for (size_t i=0; i < env_alloced_n; i++)
    {
        if (env_alloced[i] == old)
        {
            env_alloced[i] = new;
            free(old);
            return;
        }
        else if (!env_alloced[i] && new)
        {
            env_alloced[i] = new;
            new = 0;
        }
    }
    if (!new) return;
    char **t = realloc(env_alloced, sizeof *t * (env_alloced_n+1));
    if (!t) return;
    (env_alloced = t)[env_alloced_n++] = new;
}

int __putenv(char *s, size_t l, char *r)
{
    size_t i=0;
    if (__environ)
    {
        for (char **e = __environ; *e; e++, i++)
            if (!strncmp(s, *e, l+1))
            {
                char *tmp = *e;
                *e = s;
                __env_rm_add(tmp, r);
                return 0;
            }
    }
    static char **oldenv;
    char **newenv;
    if (__environ == oldenv)
    {
        newenv = realloc(oldenv, sizeof *newenv * (i+2));
        if (!newenv) goto oom;
    }
    else
    {
        newenv = malloc(sizeof *newenv * (i+2));
        if (!newenv) goto oom;
        if (i) memcpy(newenv, __environ, sizeof *newenv * i);
        free(oldenv);
    }
    newenv[i] = s;
    newenv[i+1] = 0;
    __environ = oldenv = newenv;
    if (r) __env_rm_add(0, r);
    return 0;
oom:
    free(r);
    return -1;
}

int putenv(char *s)
{
    size_t l = __strchrnul(s, '=') - s;
    if (!l || !s[l]) return unsetenv(s);
    return __putenv(s, l, 0);
}

char *getenv(const char *name)
{
    size_t l = __strchrnul(name, '=') - name;
    if (l && !name[l] && __environ)
        for (char **e = __environ; *e; e++)
            if (!strncmp(name, *e, l) && l[*e] == '=')
                return *e + l+1;
    return 0;
}

int setenv(const char *var, const char *value, int overwrite)
{
    char *s;
    size_t l1, l2;

    if (!var || !(l1 = __strchrnul(var, '=') - var) || var[l1])
    {
        return -1;
    }
    if (!overwrite && getenv(var)) return 0;

    l2 = strlen(value);
    s = malloc(l1+l2+2);
    if (!s) return -1;
    memcpy(s, var, l1);
    s[l1] = '=';
    memcpy(s+l1+1, value, l2+1);
    return __putenv(s, l1, s);
}

int unsetenv(const char *name)
{
    size_t l = __strchrnul(name, '=') - name;
    if (!l || name[l])
    {
        return -1;
    }
    if (__environ)
    {
        char **e = __environ, **eo = e;
        for (; *e; e++)
        {
            if (!strncmp(name, *e, l) && l[*e] == '=')
                __env_rm_add(*e, 0);
            else if (eo != e)
                *eo++ = *e;
            else
                eo++;
        }
        if (eo != e) *eo = 0;
    }
    return 0;
}
