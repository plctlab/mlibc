/*
 * test_char_io.c - PSE51 stdio.h character I/O test suite
 * Tests: fgetc, fputc, getc, putc, getchar, putchar, ungetc
 *        getc_unlocked, putc_unlocked, getchar_unlocked, putchar_unlocked
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for character I/O
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "../test_colors.h"
#define TEST_FILE "test_char_io.txt"
#define TEST_STRING "Hello, Character I/O!"

/* Test fputc() and fgetc() */
void test_fputc_fgetc(void) {
    FILE *fp;
    int c;
    const char *test_str = TEST_STRING;
    size_t i;
    
    TEST_START("fputc() and fgetc()");
    
    /* Write characters using fputc */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    for (i = 0; test_str[i] != '\0'; i++) {
        c = fputc(test_str[i], fp);
        assert(c == (unsigned char)test_str[i]);
    }
    
    /* Write EOF marker behavior test */
    c = fputc(EOF, fp);
    /* Implementation-defined, but typically returns EOF */
    
    fclose(fp);
    
    /* Read characters using fgetc */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    for (i = 0; test_str[i] != '\0'; i++) {
        c = fgetc(fp);
        assert(c == (unsigned char)test_str[i]);
    }
    
    /* Test EOF */
    c = fgetc(fp);
    assert(c == EOF);
    
    /* Multiple EOF reads should keep returning EOF */
    c = fgetc(fp);
    assert(c == EOF);
    
    fclose(fp);
    
    /* Test with binary data */
    fp = fopen(TEST_FILE, "wb");
    assert(fp != NULL);
    
    /* Write all possible byte values */
    for (i = 0; i < 256; i++) {
        c = fputc(i, fp);
        assert(c == (int)i);
    }
    fclose(fp);
    
    /* Read and verify all byte values */
    fp = fopen(TEST_FILE, "rb");
    assert(fp != NULL);
    
    for (i = 0; i < 256; i++) {
        c = fgetc(fp);
        assert(c == (int)i);
    }
    
    c = fgetc(fp);
    assert(c == EOF);
    
    fclose(fp);
    
    TEST_PASSED("fputc() and fgetc()");
}

/* Test getc() and putc() */
void test_getc_putc(void) {
    FILE *fp;
    int c;
    const char *test_str = "Testing getc and putc";
    size_t i;
    
    TEST_START("getc() and putc()");
    
    /* Note: getc and putc may be macros, but should behave like fgetc/fputc */
    
    /* Write using putc */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    for (i = 0; test_str[i] != '\0'; i++) {
        c = putc(test_str[i], fp);
        assert(c == (unsigned char)test_str[i]);
    }
    fclose(fp);
    
    /* Read using getc */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    for (i = 0; test_str[i] != '\0'; i++) {
        c = getc(fp);
        assert(c == (unsigned char)test_str[i]);
    }
    
    c = getc(fp);
    assert(c == EOF);
    
    fclose(fp);
    
    /* Test macro evaluation - getc/putc should evaluate stream only once */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    FILE **fpp = &fp;
    c = putc('X', *fpp);
    assert(c == 'X');
    
    fclose(fp);
    
    TEST_PASSED("getc() and putc()");
}

/* Test getchar() and putchar() */
void test_getchar_putchar(void) {
    int c;
    
    TEST_START("getchar() and putchar()");
    
    /* Note: These functions work with stdin/stdout */
    /* For automated testing, we'll test putchar output */
    
    printf("  Visual test - you should see: ABC\n  ");
    c = putchar('A');
    assert(c == 'A');
    c = putchar('B');
    assert(c == 'B');
    c = putchar('C');
    assert(c == 'C');
    c = putchar('\n');
    assert(c == '\n');
    
    /* Test with special characters */
    c = putchar('\t');
    assert(c == '\t');
    c = putchar('\n');
    assert(c == '\n');
    
    /* Note: Testing getchar() requires user input or stdin redirection */
    /* Skipping interactive getchar() test */
    
    TEST_PASSED("getchar() and putchar()");
}

/* Test ungetc() */
void test_ungetc(void) {
    FILE *fp;
    int c, c2;
    
    TEST_START("ungetc()");
    
    /* Create test file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "ABC");
    fclose(fp);
    
    /* Test basic ungetc */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    c = fgetc(fp);  /* Read 'A' */
    assert(c == 'A');
    
    c2 = ungetc('X', fp);  /* Push 'X' back */
    assert(c2 == 'X');
    
    c = fgetc(fp);  /* Should read 'X' */
    assert(c == 'X');
    
    c = fgetc(fp);  /* Should read 'B' */
    assert(c == 'B');
    
    /* Test multiple ungetc (only one pushback guaranteed) */
    c = ungetc('Y', fp);
    assert(c == 'Y');
    
    c2 = ungetc('Z', fp);
    /* Second ungetc may fail, but if it succeeds... */
    if (c2 != EOF) {
        c = fgetc(fp);
        assert(c == 'Z' || c == 'Y'); /* Implementation-defined order */
    }
    
    /* Test ungetc at beginning of file */
    rewind(fp);
    c = ungetc('!', fp);
    assert(c == '!');
    
    c = fgetc(fp);
    assert(c == '!');
    
    c = fgetc(fp);
    assert(c == 'A');
    
    /* Test ungetc with EOF */
    fseek(fp, 0, SEEK_END);
    c = ungetc('$', fp);
    assert(c == '$');
    
    c = fgetc(fp);
    assert(c == '$');
    
    c = fgetc(fp);
    assert(c == EOF);
    
    /* Test ungetc(EOF, fp) - should fail */
    c = ungetc(EOF, fp);
    assert(c == EOF);
    
    fclose(fp);
    
    /* Test ungetc on write-only file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    c = ungetc('A', fp);
    /* May fail on write-only stream */
    
    fclose(fp);
    
    TEST_PASSED("ungetc()");
}

/* Test unlocked versions (thread-unsafe but potentially faster) */
void test_unlocked_functions(void) {
    FILE *fp;
    int c;
    
    TEST_START("unlocked character I/O functions");
    
    /* Note: These functions may not be implemented in mlibc */
    /* They should behave like their locked counterparts in single-threaded use */
    
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    /* Test putc_unlocked */
    c = putc_unlocked('T', fp);
    if (c == EOF && errno == ENOSYS) {
        printf("  Note: Unlocked functions not implemented, skipping tests\n");
        fclose(fp);
        return;
    }
    assert(c == 'T');
    
    c = putc_unlocked('e', fp);
    assert(c == 'e');
    
    c = putc_unlocked('s', fp);
    assert(c == 's');
    
    c = putc_unlocked('t', fp);
    assert(c == 't');
    
    fclose(fp);
    
    /* Test getc_unlocked */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    c = getc_unlocked(fp);
    assert(c == 'T');
    
    c = getc_unlocked(fp);
    assert(c == 'e');
    
    c = getc_unlocked(fp);
    assert(c == 's');
    
    c = getc_unlocked(fp);
    assert(c == 't');
    
    c = getc_unlocked(fp);
    assert(c == EOF);
    
    fclose(fp);
    
    /* Test putchar_unlocked */
    printf("  Visual test - you should see: OK\n  ");
    c = putchar_unlocked('O');
    assert(c == 'O');
    c = putchar_unlocked('K');
    assert(c == 'K');
    c = putchar_unlocked('\n');
    assert(c == '\n');
    
    TEST_PASSED("Unlocked character I/O");
}

/* Test character I/O with different file modes */
void test_char_io_modes(void) {
    FILE *fp;
    int c;
    
    TEST_START("character I/O with different file modes");
    
    /* Test append mode */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fputc('1', fp);
    fputc('2', fp);
    fclose(fp);
    
    fp = fopen(TEST_FILE, "a");
    assert(fp != NULL);
    fputc('3', fp);
    fputc('4', fp);
    fclose(fp);
    
    /* Verify append worked */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    assert(fgetc(fp) == '1');
    assert(fgetc(fp) == '2');
    assert(fgetc(fp) == '3');
    assert(fgetc(fp) == '4');
    assert(fgetc(fp) == EOF);
    fclose(fp);
    
    /* Test read/write mode */
    fp = fopen(TEST_FILE, "w+");
    assert(fp != NULL);
    
    fputc('A', fp);
    fputc('B', fp);
    fputc('C', fp);
    
    /* Seek back and read */
    fseek(fp, 0, SEEK_SET);
    assert(fgetc(fp) == 'A');
    
    /* Overwrite */
    fseek(fp, 1, SEEK_SET);
    fputc('X', fp);
    
    /* Verify */
    fseek(fp, 0, SEEK_SET);
    assert(fgetc(fp) == 'A');
    assert(fgetc(fp) == 'X');
    assert(fgetc(fp) == 'C');
    
    fclose(fp);
    
    TEST_PASSED("Character I/O mode");
}

/* Test error conditions */
void test_char_io_errors(void) {
    FILE *fp;
    int c;
    
    TEST_START("character I/O error conditions");
    
    /* Test reading from write-only file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    c = fgetc(fp);
    assert(c == EOF);
    /* ferror(fp) should be set */
    
    fclose(fp);
    
    /* Test writing to read-only file */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    c = fputc('X', fp);
    assert(c == EOF);
    /* ferror(fp) should be set */
    
    fclose(fp);
    
    /* Test operations on closed file */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    fclose(fp);
    
    /* These should fail, but behavior is undefined */
    /* Don't test to avoid crashes */
    
    TEST_PASSED("Character I/O error");
}

/* Test character I/O performance with large files */
void test_char_io_performance(void) {
    FILE *fp;
    int c;
    int i;
    const int size = 10000;
    
    TEST_START("character I/O with large data");
    
    /* Write large amount of data character by character */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    for (i = 0; i < size; i++) {
        c = fputc('A' + (i % 26), fp);
        assert(c == 'A' + (i % 26));
    }
    fclose(fp);
    
    /* Read and verify */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    for (i = 0; i < size; i++) {
        c = fgetc(fp);
        assert(c == 'A' + (i % 26));
    }
    
    c = fgetc(fp);
    assert(c == EOF);
    
    fclose(fp);
    
    TEST_PASSED("Large data character I/O");
}

/* Clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdio.h Character I/O Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any existing test files */
    cleanup_files();
    
    /* Run tests */
    test_fputc_fgetc();
    test_getc_putc();
    test_getchar_putchar();
    test_ungetc();
    test_unlocked_functions();
    test_char_io_modes();
    test_char_io_errors();
    test_char_io_performance();
    
    /* Clean up test files */
    cleanup_files();
    
    TEST_SUITE_PASSED("character I/O");
}
