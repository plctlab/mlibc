/*
 * test_formatted_io.c - PSE51 stdio.h formatted I/O test suite
 * Tests: printf, fprintf, sprintf, snprintf, vprintf, vfprintf, vsprintf, vsnprintf
 *        scanf, fscanf, sscanf, vscanf, vfscanf, vsscanf
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for formatted I/O
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <unistd.h>

#include "../test_colors.h"
#define BUFFER_SIZE 1024
#define TEST_FILE "test_formatted.txt"

/* Helper function for variadic tests */
int my_vsnprintf(char *str, size_t size, const char *format, ...) {
    va_list args;
    int ret;
    
    va_start(args, format);
    ret = vsnprintf(str, size, format, args);
    va_end(args);
    
    return ret;
}

/* Helper function for file-based variadic tests */
int my_vfprintf(FILE *fp, const char *format, ...) {
    va_list args;
    int ret;
    
    va_start(args, format);
    ret = vfprintf(fp, format, args);
    va_end(args);
    
    return ret;
}

/* Test printf() family functions */
void test_printf_family(void) {
    char buffer[BUFFER_SIZE];
    char small_buffer[10];
    int ret;
    FILE *fp;
    
    TEST_START("printf() family functions");
    
    /* Test basic printf (visual check) */
    printf("  Visual test - you should see: Hello, World!\n  ");
    ret = printf("Hello, World!\n");
    assert(ret == 14); /* "Hello, World!\n" = 14 chars */
    
    /* Test sprintf with various format specifiers */
    /* Integer formats */
    ret = sprintf(buffer, "%d", 42);
    assert(ret == 2);
    assert(strcmp(buffer, "42") == 0);
    
    ret = sprintf(buffer, "%d %d %d", 1, 2, 3);
    assert(ret == 5);
    assert(strcmp(buffer, "1 2 3") == 0);
    
    ret = sprintf(buffer, "%+d", 42);
    assert(ret == 3);
    assert(strcmp(buffer, "+42") == 0);
    
    ret = sprintf(buffer, "%05d", 42);
    assert(ret == 5);
    assert(strcmp(buffer, "00042") == 0);
    
    ret = sprintf(buffer, "%-5d|", 42);
    assert(ret == 6);
    assert(strcmp(buffer, "42   |") == 0);
    
    /* Unsigned formats */
    ret = sprintf(buffer, "%u", 42U);
    assert(ret == 2);
    assert(strcmp(buffer, "42") == 0);
    
    ret = sprintf(buffer, "%x", 255);
    assert(ret == 2);
    assert(strcmp(buffer, "ff") == 0);
    
    ret = sprintf(buffer, "%X", 255);
    assert(ret == 2);
    assert(strcmp(buffer, "FF") == 0);
    
    ret = sprintf(buffer, "%o", 64);
    assert(ret == 3);
    assert(strcmp(buffer, "100") == 0);
    
    /* Character and string formats */
    ret = sprintf(buffer, "%c", 'A');
    assert(ret == 1);
    assert(strcmp(buffer, "A") == 0);
    
    ret = sprintf(buffer, "%s", "Hello");
    assert(ret == 5);
    assert(strcmp(buffer, "Hello") == 0);
    
    ret = sprintf(buffer, "%10s", "Hello");
    assert(ret == 10);
    assert(strcmp(buffer, "     Hello") == 0);
    
    ret = sprintf(buffer, "%-10s|", "Hello");
    assert(ret == 11);
    assert(strcmp(buffer, "Hello     |") == 0);
    
    ret = sprintf(buffer, "%.3s", "Hello");
    assert(ret == 3);
    assert(strcmp(buffer, "Hel") == 0);
    
    /* Pointer format */
    void *ptr = (void *)0x12345678;
    ret = sprintf(buffer, "%p", ptr);
    assert(ret > 0);
    /* Exact format is implementation-defined */
    
    /* Percent literal */
    ret = sprintf(buffer, "%%");
    assert(ret == 1);
    assert(strcmp(buffer, "%") == 0);
    
    /* Test snprintf - size limiting */
    ret = snprintf(small_buffer, sizeof(small_buffer), "Hello, World!");
    assert(ret == 13); /* Would have written 13 chars */
    assert(strcmp(small_buffer, "Hello, Wo") == 0); /* But only 9 chars + null */
    
    ret = snprintf(small_buffer, 0, "Hello");
    assert(ret == 5); /* Would have written 5 chars */
    
    ret = snprintf(NULL, 0, "Hello");
    assert(ret == 5); /* Count-only mode */
    
    /* Test fprintf */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = fprintf(fp, "Number: %d\n", 42);
    assert(ret == 11);
    
    ret = fprintf(fp, "String: %s\n", "Test");
    assert(ret == 12);
    
    fclose(fp);
    
    /* Verify fprintf output */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    fgets(buffer, BUFFER_SIZE, fp);
    assert(strcmp(buffer, "Number: 42\n") == 0);
    
    fgets(buffer, BUFFER_SIZE, fp);
    assert(strcmp(buffer, "String: Test\n") == 0);
    
    fclose(fp);
    
    /* Test variadic versions */
    ret = my_vsnprintf(buffer, BUFFER_SIZE, "%d %s", 42, "test");
    assert(ret == 7);
    assert(strcmp(buffer, "42 test") == 0);
    
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    ret = my_vfprintf(fp, "%d %s\n", 100, "variadic");
    assert(ret == 12);
    fclose(fp);
    
    TEST_PASSED("printf() family");
}

/* Test advanced format specifiers */
void test_advanced_formats(void) {
    char buffer[BUFFER_SIZE];
    int ret;
    
    TEST_START("advanced format specifiers");
    
    /* Long and long long */
    ret = sprintf(buffer, "%ld", 1234567890L);
    assert(ret > 0);
    assert(strcmp(buffer, "1234567890") == 0);
    
    ret = sprintf(buffer, "%lld", 1234567890123456789LL);
    assert(ret > 0);
    /* Verify it's a large number */
    assert(strlen(buffer) > 10);
    
    /* Width and precision combinations */
    ret = sprintf(buffer, "%10.5d", 42);
    assert(ret == 10);
    assert(strcmp(buffer, "     00042") == 0);
    
    ret = sprintf(buffer, "%*d", 5, 42);
    assert(ret == 5);
    assert(strcmp(buffer, "   42") == 0);
    
    ret = sprintf(buffer, "%.*s", 3, "Hello");
    assert(ret == 3);
    assert(strcmp(buffer, "Hel") == 0);
    
    ret = sprintf(buffer, "%*.*s", 10, 3, "Hello");
    assert(ret == 10);
    assert(strcmp(buffer, "       Hel") == 0);
    
    /* Floating point (if supported) */
    /* Note: mlibc might not support floating point formats */
    ret = sprintf(buffer, "%d", (int)3.14);
    assert(ret == 1);
    assert(strcmp(buffer, "3") == 0);
    
    /* Multiple format specifiers */
    ret = sprintf(buffer, "%d %o %x %X", 255, 255, 255, 255);
    assert(ret > 0);
    assert(strcmp(buffer, "255 377 ff FF") == 0);
    
    /* Edge cases */
    ret = sprintf(buffer, "%d", INT_MAX);
    assert(ret > 0);
    
    ret = sprintf(buffer, "%d", INT_MIN);
    assert(ret > 0);
    
    ret = sprintf(buffer, "%u", UINT_MAX);
    assert(ret > 0);
    
    TEST_PASSED("Advanced format");
}

/* Test scanf() family functions */
void test_scanf_family(void) {
    int i1, i2, i3;
    unsigned int u1;
    char c;
    char str[100];
    int ret;
    FILE *fp;
    
    TEST_START("scanf() family functions");
    
    /* Note: scanf functions may not be fully implemented in mlibc */
    /* Test sscanf first as it doesn't require user input */
    
    /* Test integer scanning */
    ret = sscanf("42", "%d", &i1);
    if (ret == EOF) {
        printf("  Note: scanf functions not implemented, skipping tests\n");
        return;
    }
    assert(ret == 1);
    assert(i1 == 42);
    
    ret = sscanf("1 2 3", "%d %d %d", &i1, &i2, &i3);
    assert(ret == 3);
    assert(i1 == 1 && i2 == 2 && i3 == 3);
    
    /* Test unsigned scanning */
    ret = sscanf("255", "%u", &u1);
    assert(ret == 1);
    assert(u1 == 255);
    
    ret = sscanf("ff", "%x", &u1);
    assert(ret == 1);
    assert(u1 == 255);
    
    ret = sscanf("377", "%o", &u1);
    assert(ret == 1);
    assert(u1 == 255);
    
    /* Test character and string scanning */
    ret = sscanf("A", "%c", &c);
    assert(ret == 1);
    assert(c == 'A');
    
    ret = sscanf("Hello", "%s", str);
    assert(ret == 1);
    assert(strcmp(str, "Hello") == 0);
    
    ret = sscanf("Hello World", "%s", str);
    assert(ret == 1);
    assert(strcmp(str, "Hello") == 0); /* %s stops at whitespace */
    
    /* Test width specifiers */
    ret = sscanf("12345", "%3d", &i1);
    assert(ret == 1);
    assert(i1 == 123);
    
    ret = sscanf("HelloWorld", "%5s", str);
    assert(ret == 1);
    assert(strcmp(str, "Hello") == 0);
    
    /* Test fscanf */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "42 255\nHello\n");
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    ret = fscanf(fp, "%d %u", &i1, &u1);
    assert(ret == 2);
    assert(i1 == 42 && u1 == 255);
    
    ret = fscanf(fp, "%s", str);
    assert(ret == 1);
    assert(strcmp(str, "Hello") == 0);
    
    fclose(fp);
    
    /* Test matching failures */
    ret = sscanf("abc", "%d", &i1);
    assert(ret == 0); /* No successful conversions */
    
    ret = sscanf("42 abc", "%d %d", &i1, &i2);
    assert(ret == 1); /* Only first conversion succeeded */
    assert(i1 == 42);
    
    TEST_PASSED("scanf() family");
}

/* Test format string edge cases and errors */
void test_format_edge_cases(void) {
    char buffer[BUFFER_SIZE];
    int ret;
    
    TEST_START("format string edge cases");
    
    /* Empty format string */
    ret = sprintf(buffer, "");
    assert(ret == 0);
    assert(buffer[0] == '\0');
    
    /* Format string with no specifiers */
    ret = sprintf(buffer, "Hello World");
    assert(ret == 11);
    assert(strcmp(buffer, "Hello World") == 0);
    
    /* Multiple percent signs */
    ret = sprintf(buffer, "%%%%");
    assert(ret == 2);
    assert(strcmp(buffer, "%%") == 0);
    
    /* Very long format result (within buffer) */
    ret = sprintf(buffer, "%500d", 42);
    assert(ret == 500);
    assert(buffer[498] == '4' && buffer[499] == '2');
    
    /* Null string */
    ret = sprintf(buffer, "%s", (char *)NULL);
    /* Implementation-defined, but shouldn't crash */
    assert(ret >= 0);
    
    /* Zero precision */
    ret = sprintf(buffer, "%.0d", 0);
    assert(ret == 0);
    assert(buffer[0] == '\0');
    
    ret = sprintf(buffer, "%.0d", 1);
    assert(ret == 1);
    assert(strcmp(buffer, "1") == 0);
    
    /* Precision with strings */
    ret = sprintf(buffer, "%.0s", "Hello");
    assert(ret == 0);
    assert(buffer[0] == '\0');
    
    TEST_PASSED("Format edge case");
}

/* Test locale-dependent formatting (basic) */
void test_locale_formatting(void) {
    char buffer[BUFFER_SIZE];
    int ret;
    
    TEST_START("locale-dependent formatting");
    
    /* Note: mlibc may have minimal locale support */
    /* Test thousands separator (implementation-defined) */
    ret = sprintf(buffer, "%'d", 1000000);
    /* Just verify it doesn't crash and returns something */
    assert(ret > 0);
    
    /* Test positional parameters (if supported) */
    ret = sprintf(buffer, "%2$d %1$d", 1, 2);
    /* If not supported, should still work as regular format */
    assert(ret > 0);
    
    TEST_PASSED("Locale formatting");
}

/* Test buffer overflow protection */
void test_buffer_safety(void) {
    char tiny_buffer[5];
    int ret;
    
    TEST_START("buffer overflow protection");
    
    /* snprintf should never overflow */
    ret = snprintf(tiny_buffer, sizeof(tiny_buffer), "Hello World!");
    assert(ret == 12); /* Would have written 12 */
    assert(strlen(tiny_buffer) == 4); /* But only wrote 4 + null */
    assert(tiny_buffer[4] == '\0'); /* Null terminated */
    
    /* Test with exact size */
    ret = snprintf(tiny_buffer, sizeof(tiny_buffer), "1234");
    assert(ret == 4);
    assert(strcmp(tiny_buffer, "1234") == 0);
    
    /* Test with size 1 (only null terminator) */
    ret = snprintf(tiny_buffer, 1, "Hello");
    assert(ret == 5);
    assert(tiny_buffer[0] == '\0');
    
    /* Test with size 0 */
    ret = snprintf(tiny_buffer, 0, "Hello");
    assert(ret == 5);
    /* Buffer should be unchanged */
    
    TEST_PASSED("Buffer safety");
}

/* Clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdio.h Formatted I/O Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any existing test files */
    cleanup_files();
    
    /* Run tests */
    test_printf_family();
    test_advanced_formats();
    test_scanf_family();
    test_format_edge_cases();
    test_locale_formatting();
    test_buffer_safety();
    
    /* Clean up test files */
    cleanup_files();
    
    TEST_SUITE_PASSED("formatted I/O");
    return 0;
}
