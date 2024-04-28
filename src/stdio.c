/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/2/1       linshire     the first version
 * 2024/4/24      bitbiscuits  fix printf bug and realize snprintf
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>


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
            fputc((char)*ptr_string, stream);
            ret_num++;
            break;
        case '\t':
            fputc(*ptr_string, stream);
            ret_num += 4;
            break;
        case '\r':
            fputc(*ptr_string, stream);
            ret_num++;
            break;
        case '\n':
            fputc(*ptr_string, stream);
            ret_num++;
            break;
        case '%':
            ptr_string++;

            /* anlynize %*/
            switch (*ptr_string)
            {
            case '%':
                fputc('%', stream);
                ret_num++;
                ptr_string++;
                continue;
            case 'c':
                arg_int_val = va_arg(arg_temp, int);
                fputc((char)arg_int_val, stream);
                ret_num++;
                ptr_string++;
                continue;
            case 'd':
                arg_int_val = va_arg(arg_temp, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;
                    fputc('-', stream);
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
                    fputc((char)val_seg + '0', stream);
                    cnt--;
                }

                ptr_string++;

                continue;

            case 'o':
                arg_int_val = va_arg(arg_temp, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;
                    fputc('-', stream);
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
                    fputc((char)val_seg + '0', stream);
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
                        fputc((char)val_seg + '0', stream);
                    else
                    {
                        // fputc(stream, (char)val_seg - 10 + 'a');
                        fputc((char)val_seg - 10 + 'A', stream);
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
                    fputc((char)val_seg + '0', stream);
                    cnt--;
                }
                ptr_string++;
                continue;

            case 's':
                arg_string_val = va_arg(arg_temp, char *);
                ret_num += (unsigned int)strlen(arg_string_val);
                while (*arg_string_val)
                {
                    fputc(*arg_string_val, stream);
                    arg_string_val++;
                }
                ptr_string++;
                continue;

            case 'f':
                arg_float_val = va_arg(arg_temp, double);
                if (arg_float_val < 0)
                {
                    arg_float_val = -arg_float_val;
                    fputc('-', stream);
                    ret_num++;
                }

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
                    fputc((char)val_seg + '0', stream);
                    cnt--;
                }
                fputc('.', stream);
                ret_num++;
                arg_float_val *= 1000000;
                cnt = 6;
                val_temp = (int)arg_float_val;
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    fputc((char)val_seg + '0', stream);
                    cnt--;
                }
                ret_num += 6;
                ptr_string++;
                continue;
            default:
                fputc(' ', stream);
                ret_num++;
                continue;
            }
        default:
            fputc(*ptr_string, stream);
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

int printf(const char *str, ...)
{
    va_list args;
    int res;

    va_start (args, str);
    res = vfprintf (stdout, str, args);
    va_end (args);
    return res;
}

int putchar(int c)
{
    fputc(c, stdin);
    return c;
}

int puts(const char* str)
{
    int r = 0;

    for(const char* c = str; *c != 0; c++)
    {
        putchar((int)*c);
        r++;
    }

    // puts adds a newline
    if (r)
    {
        putchar('\n');
        r++;
    }

    return r ? r : EOF;
}

static int write_char_to_arr(char* buf, char ch)
{
    *buf = ch;
    return ch;
}

int __vsprintf(char* buf, int buf_nbytes, const char *format, va_list arg)
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
    unsigned long str_length = 0;   /* string buf_nbytesgth */
    int cnt = 0;
    int outline = 0;

    if(buf_nbytes == 0)
    {
        return 0;
    }

    va_copy(arg_temp, arg);

    if (format == NULL)
    {
        va_end(arg_temp);
        return -1;
    }

    while (*ptr_string != '\0')
    {
        if(buf_nbytes > 0 && !outline && ret_num + 1 >= buf_nbytes)
            break;

        switch (*ptr_string)
        {
        case ' ':
            write_char_to_arr(buf , (char)*ptr_string);
            buf++;
            ret_num++;
            break;
        case '\t':
            write_char_to_arr(buf , *ptr_string);
            ret_num++;
            buf++;
            break;
        case '\r':
            write_char_to_arr(buf , *ptr_string);
            ret_num++;
            buf++;
            break;
        case '\n':
            write_char_to_arr(buf , *ptr_string);
            ret_num++;
            buf++;
            break;
        case '%':
            ptr_string++;

            /* anlynize %*/
            switch (*ptr_string)
            {
            case '%':
                write_char_to_arr(buf , '%');
                ret_num++;
                buf++;
                ptr_string++;
                continue;
            case 'c':
                arg_int_val = va_arg(arg_temp, int);
                write_char_to_arr(buf , (char)arg_int_val);
                ret_num++;
                buf++;
                ptr_string++;
                continue;
            case 'd':
                arg_int_val = va_arg(arg_temp, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;
                    write_char_to_arr(buf , '-');
                    ret_num++;
                    buf++;
                }
                val_seg = arg_int_val;

                /* arg_int_val buf_nbytesgth */
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

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += cnt;

                while (cnt)
                {
                    val_seg = arg_int_val / m_pow_n(10, cnt - 1);
                    arg_int_val %= m_pow_n(10, cnt - 1);
                    write_char_to_arr(buf , (char)val_seg + '0');
                    buf++;
                    cnt--;
                }

                ptr_string++;

                continue;

            case 'o':

                arg_int_val = va_arg(arg_temp, int);
                if (arg_int_val < 0)
                {
                    arg_int_val = -arg_int_val;

                    write_char_to_arr(buf , '-');
                    ret_num++;
                    buf++;
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

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += cnt;

                while (cnt)
                {
                    val_seg = arg_int_val / m_pow_n(8, cnt - 1);
                    arg_int_val %= m_pow_n(8, cnt - 1);
                    write_char_to_arr(buf , (char)val_seg + '0');
                    buf++;
                    cnt--;
                }
                ptr_string++;
                continue;

            case 'x':
                arg_hex_val = va_arg(arg_temp, unsigned long);
                val_seg = arg_hex_val;

                /* get arg_int_val buf_nbytesgth */
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

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += cnt;
                
                while (cnt)
                {
                    val_seg = arg_hex_val / m_pow_n(16, cnt - 1);
                    arg_hex_val %= m_pow_n(16, cnt - 1);
                    if (val_seg <= 9)
                        write_char_to_arr(buf , (char)val_seg + '0');
                    else
                    {
                        // write_char_to_arr(buf , (char)val_seg - 10 + 'a');
                        write_char_to_arr(buf , (char)val_seg - 10 + 'A');
                    }
                    buf++;
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

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += cnt;

                while (cnt)
                {
                    val_seg = arg_int_val / m_pow_n(2, cnt - 1);
                    arg_int_val %= m_pow_n(2, cnt - 1);
                    write_char_to_arr(buf , (char)val_seg + '0');
                    buf++;
                    cnt--;
                }
                ptr_string++;
                continue;

            case 's':
                arg_string_val = va_arg(arg_temp, char *);
                str_length = (unsigned int)strlen(arg_string_val);

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += str_length;

                while (*arg_string_val)
                {
                    write_char_to_arr(buf , *arg_string_val);
                    arg_string_val++;
                    buf++;
                }
                ptr_string++;
                continue;

            case 'f':
                arg_float_val = va_arg(arg_temp, double);
                if (arg_float_val < 0)
                {
                    arg_float_val = -arg_float_val;
                    write_char_to_arr(buf, '-');
                    ret_num++;
                    buf++;
                }

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

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += cnt;

                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    write_char_to_arr(buf , (char)val_seg + '0');
                    cnt--;
                    buf++;
                }
                write_char_to_arr(buf , '.');
                ret_num++;
                buf++;

                arg_float_val *= 1000000;
                cnt = 6;

                if(buf_nbytes > 0 && ret_num + cnt >= buf_nbytes)
                {
                    outline = 1;
                    break;
                }
                ret_num += cnt;
                
                val_temp = (int)arg_float_val;
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    write_char_to_arr(buf , (char)val_seg + '0');
                    cnt--;
                    buf++;
                }
                
                ptr_string++;
                continue;
            default:
                write_char_to_arr(buf , ' ');
                ret_num++;
                buf++;
                continue;
            }
        default:
            if(outline)
                break;
            write_char_to_arr(buf , *ptr_string);
            ret_num++;
            buf++;
            break;
        }
        ptr_string++;
    }
    va_end(arg_temp);
    
    
    return ret_num;
}

int vsnprintf(char* buf, int buf_nbytes, const char *format, va_list arg)
{
    va_list args_temp;
    int res;

    va_copy(args_temp, arg);

    res = __vsprintf(buf, buf_nbytes, format, arg);

    va_end(args_temp);

    return res;
}

int vsprintf(char* buf, const char* format, va_list arg)
{
    va_list args_temp;
    int res;

    va_copy(args_temp, arg);

    res = __vsprintf(buf, -1, format, arg);

    va_end(args_temp);

    return res;
}

int sprintf(char* buf, const char* format, ...)
{
    va_list args;
    int res;

    va_start(args, format);

    res = vsprintf(buf, format, args);
    *(buf + res) = '\0';

    va_end(args);

    return res;
}

int snprintf (char* buf, size_t buf_nbytes, const char* format, ...)
{
    va_list args;
    int res;

    va_start (args, format);

    res = vsnprintf(buf, buf_nbytes, format, args);
    buf[res] = '\0';

    va_end (args);

    return res;
}

int fputc(int character, FILE* stream)
{
    return putc(character, stream);
}

int putc (int character, FILE* stream)
{
    if(stream == NULL)
    {
        return EOF;
    }
    
    return write(stream->fd, character)
}


int getc (FILE* stream)
{
    int buf = EOF;

    if(stream == NULL)
    {
        return EOF;
    }

    if(read(stream->fd, &buf, 1) == 1)
    {
        return buf;
    }
    else
    {
        return EOF;
    }
}

int fgetc (FILE* stream)
{
    return getc(stream);
}

int getchar(void)
{
    return fgetc(stdout);
}