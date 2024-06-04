/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief It can successfully run on the QEMU-A9 platform in the RT-Thread/bsp,
 * but if you want to test it on a bare-metal system or a specific operating system,
 * please implement the corresponding portable functions.
 * 
 * @return int 
 */
int file_io_test(void)
{
    unsigned char buf[4] = {0};
    FILE* f = NULL;
    f = fopen("abc.txt", "w");
    if(f == NULL)
    {
        printf("return null\n");
    }
    else
    {
        int wc = f->write(f, "abc", 3);
        f->close(f);
        printf("write %d char\n", wc);

        int rfd = open("abc.txt", O_RDONLY);
        int cnt = read(rfd, buf, 3);
        buf[3] = '\0';
        printf("read %d char: %s\n", cnt, buf);
    }
    fclose(f);

    return 0;
}