/*
 * Copyright (c) 2023
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/2/1       linshire     the first version
 */

#include <stdio.h>
#include <stdarg.h>
#include <typedef.h>

extern void m_putc(char chr);

void m_putc(char chr)
{
    ;
}

/* calculate m^n */
unsigned long m_pow_n(unsigned long m, unsigned long n)
{
    unsigned long i = 0, ret = 1;
    if (n < 0)
        return 0;
    for (i = 0; i < n; i++)
    {
        ret *= m;
    }
    return ret;
}

int print(const char *str, ...)
{
    if (str == NULL)
        return -1;

    unsigned int ret_num = 0;       /*return printf char num*/
    char *ptr_string = (char *)str; /*point to str*/
    int arg_int_val = 0;            /*receive int*/
    unsigned long arg_hex_val = 0;  /*receive HEX*/
    char *arg_string_val = NULL;    /*receive string*/
    double arg_float_val = 0.0;     /*receive float*/
    unsigned long val_seg = 0;      /* val_seg */
    unsigned long val_temp = 0;     /* val temp */
    int cnt = 0;
    int i = 0;

    va_list ptr_args;
    va_start(ptr_args, str);

    while (*ptr_string != '\0')
    {
        switch (*ptr_string)
        {
        case ' ':
            m_putc(*ptr_string);
            ret_num++;
            break;
        case '\t':
            m_putc(*ptr_string);
            ret_num += 4;
            break;
        case '\r':
            m_putc(*ptr_string);
            ret_num++;
            break;
        case '\n':
            m_putc(*ptr_string);
            ret_num++;
            break;
        case '%':
            ptr_string++;

            /* anlynize %*/
            switch (*ptr_string)
            {
            case '%':
                m_putc('%');
                ret_num++;
                ptr_string++;
                continue;
            case 'c':
                arg_int_val = va_arg(ptr_args, int);
                m_putc((char)arg_int_val);
                ret_num++;
                ptr_string++;
                continue;
            case 'd':
                arg_int_val = va_arg(ptr_args, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;
                    m_putc('-');
                    ret_num++;
                }
                val_seg = arg_int_val;

                /* arg_int_val length */
                if (arg_int_val)
                {
                    while (val_seg)
                    {
                        cnt++;
                        val_seg /= 10;
                    }
                }
                else
                    cnt = 1;

                ret_num += cnt;

                while (cnt)
                {
                    val_seg = arg_int_val / m_pow_n(10, cnt - 1);
                    arg_int_val %= m_pow_n(10, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }

                ptr_string++;

                continue;

            case 'o':

                arg_int_val = va_arg(ptr_args, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;

                    m_putc('-');
                    ret_num++;
                }
                val_seg = arg_int_val;

                if (arg_int_val)
                {
                    while (val_seg)
                    {
                        cnt++;
                        val_seg /= 8;
                    }
                }
                else
                    cnt = 1;

                ret_num += cnt;

                while (cnt)
                {
                    val_seg = arg_int_val / m_pow_n(8, cnt - 1);
                    arg_int_val %= m_pow_n(8, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                ptr_string++;
                continue;
 
            case 'x':
                arg_hex_val = va_arg(ptr_args, unsigned long);
                val_seg = arg_hex_val;

                /* get arg_int_val length */
                if (arg_hex_val)
                {
                    while (val_seg)
                    {
                        cnt++;
                        val_seg /= 16;
                    }
                }
                else
                    cnt = 1;
                ret_num += cnt;
                while (cnt)
                {
                    val_seg = arg_hex_val / m_pow_n(16, cnt - 1);
                    arg_hex_val %= m_pow_n(16, cnt - 1);
                    if (val_seg <= 9)
                        m_putc((char)val_seg + '0');
                    else
                    {
                        // m_putc((char)val_seg - 10 + 'a');
                        m_putc((char)val_seg - 10 + 'A');
                    }
                    cnt--;
                }
                ptr_string++;
                continue;

            case 'b':
                arg_int_val = va_arg(ptr_args, int);
                val_seg = arg_int_val;

                if (arg_int_val)
                {
                    while (val_seg)
                    {
                        cnt++;
                        val_seg /= 2;
                    }
                }
                else
                    cnt = 1;

                ret_num += cnt;

                while (cnt)
                {
                    val_seg = arg_int_val / m_pow_n(2, cnt - 1);
                    arg_int_val %= m_pow_n(2, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                ptr_string++;
                continue;

            case 's':
                arg_string_val = va_arg(ptr_args, char *);
                ret_num += (unsigned int)strlen(arg_string_val);
                while (*arg_string_val)
                {
                    m_putc(*arg_string_val);
                    arg_string_val++;
                }
                ptr_string++;
                continue;

            case 'f':
                arg_float_val = va_arg(ptr_args, double);
                val_seg = (unsigned long)arg_float_val;
                val_temp = val_seg;
                arg_float_val = arg_float_val - val_seg;
                if (val_seg)
                {
                    while (val_seg)
                    {
                        cnt++;
                        val_seg /= 10;
                    }
                }
                else
                    cnt = 1;
                ret_num += cnt;
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                m_putc('.');
                ret_num++;
                arg_float_val *= 1000000;
                // printf("\r\n %f\r\n", arg_float_val);
                cnt = 6;
                val_temp = (int)arg_float_val;
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                ret_num += 6;
                ptr_string++;
                continue;
            default:
                m_putc(' ');
                ret_num++;
                continue;
            }
        default:
            m_putc(*ptr_string);
            ret_num++;
            break;
        }
        ptr_string++;
    }
    va_end(ptr_args);

    return ret_num;
}
