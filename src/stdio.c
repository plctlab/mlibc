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
#include <string.h>

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

int vfprintf(FILE *stream, const char *format, va_list arg)
{

    va_list arg_temp;
    unsigned int ret_num = 0;       /*return printf char num*/
    char *ptr_string = (char *)format; /*point to str*/
    int arg_int_val = 0;            /*receive int*/
    unsigned long arg_hex_val = 0;  /*receive HEX*/
    char *arg_string_val = NULL;    /*receive string*/
    double arg_float_val = 0.0;     /*receive float*/
    unsigned long val_seg = 0;      /* val_seg */
    unsigned long val_temp = 0;     /* val temp */
    int cnt = 0;
    int i = 0;

    va_copy(arg_temp, arg);

    if (format == NULL)
    {
        va_end(arg_temp);
        return -1;
    }

    while (*ptr_string != '\0')
    {
        switch (*ptr_string)
        {
        case ' ':
            putchar(*ptr_string);
            ret_num++;
            break;
        case '\t':
            putchar(*ptr_string);
            ret_num += 4;
            break;
        case '\r':
            putchar(*ptr_string);
            ret_num++;
            break;
        case '\n':
            putchar(*ptr_string);
            ret_num++;
            break;
        case '%':
            ptr_string++;

            /* anlynize %*/
            switch (*ptr_string)
            {
            case '%':
                putchar('%');
                ret_num++;
                ptr_string++;
                continue;
            case 'c':
                arg_int_val = va_arg(arg_temp, char);
                putchar((char)arg_int_val);
                ret_num++;
                ptr_string++;
                continue;
            case 'd':
                arg_int_val = va_arg(arg_temp, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;
                    putchar('-');
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
                    putchar((char)val_seg + '0');
                    cnt--;
                }

                ptr_string++;

                continue;

            case 'o':

                arg_int_val = va_arg(arg_temp, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;

                    putchar('-');
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
                    putchar((char)val_seg + '0');
                    cnt--;
                }
                ptr_string++;
                continue;

            case 'x':
                arg_hex_val = va_arg(arg_temp, unsigned long);
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
                        putchar((char)val_seg + '0');
                    else
                    {
                        // putchar((char)val_seg - 10 + 'a');
                        putchar((char)val_seg - 10 + 'A');
                    }
                    cnt--;
                }
                ptr_string++;
                continue;

            case 'b':
                arg_int_val = va_arg(arg_temp, int);
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
                    putchar((char)val_seg + '0');
                    cnt--;
                }
                ptr_string++;
                continue;

            case 's':
                arg_string_val = va_arg(arg_temp, char *);
                ret_num += (unsigned int)strlen(arg_string_val);
                while (*arg_string_val)
                {
                    putchar(*arg_string_val);
                    arg_string_val++;
                }
                ptr_string++;
                continue;

            case 'f':
                arg_float_val = va_arg(arg_temp, double);
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
                    putchar((char)val_seg + '0');
                    cnt--;
                }
                putchar('.');
                ret_num++;
                arg_float_val *= 1000000;
                // printf("\r\n %f\r\n", arg_float_val);
                cnt = 6;
                val_temp = (int)arg_float_val;
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    putchar((char)val_seg + '0');
                    cnt--;
                }
                ret_num += 6;
                ptr_string++;
                continue;
            default:
                putchar(' ');
                ret_num++;
                continue;
            }
        default:
            putchar(*ptr_string);
            ret_num++;
            break;
        }
        ptr_string++;
    }
    va_end(arg_temp);

    return ret_num;
}

int fprintf(FILE *stream, const char *format, ...)
{
    va_list args;
    int res;

    va_start (args, format);
    res = vfprintf (stream, format, args);
    va_end (args);
    return res;
}


int vprintf(const char *str,  va_list arg)
{
    int res;
    FILE *stream;
    va_list arg_temp = arg;

    va_copy(arg_temp, arg);

    res = vfprintf(stream, str, arg_temp);

    va_end (arg);
    return res;
}

int printf(const char *str, ...)
{
  va_list args;
  int res;

  va_start (args, str);
  res = vprintf (str, args);
  va_end (args);
  return res;
}
