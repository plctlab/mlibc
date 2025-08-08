/*
 * test_string_io.c - PSE51 stdio.h string I/O test suite
 * Tests: fgets, fputs, gets, puts
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for string I/O
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "../test_colors.h"
#define TEST_FILE "test_string_io.txt"
#define BUFFER_SIZE 256
#define SMALL_BUFFER_SIZE 10

/* Test fputs() */
void test_fputs(void) {
    FILE *fp;
    int ret;
    const char *test_strings[] = {
        "Hello, World!",
        "Line with newline\n",
        "Line without newline",
        "",  /* Empty string */
        "String with\ttab",
        "Multi\nline\nstring",
        NULL
    };
    int i;
    
    TEST_START("fputs()");
    
    /* Test writing strings to file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    for (i = 0; test_strings[i] != NULL; i++) {
        ret = fputs(test_strings[i], fp);
        assert(ret >= 0); /* Non-negative on success */
    }
    
    fclose(fp);
    
    /* Test writing to stdout */
    printf("  Visual test - you should see: Test String\n  ");
    ret = fputs("Test String\n", stdout);
    assert(ret >= 0);
    
    /* Test error conditions */
    fp = fopen(TEST_FILE, "r"); /* Open for reading only */
    assert(fp != NULL);
    
    ret = fputs("This should fail", fp);
    assert(ret == EOF); /* Should fail on read-only file */
    
    fclose(fp);
    
    /* Test with special characters */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = fputs("Special: \x01\x02\x03\xFF", fp);
    assert(ret >= 0);
    
    fclose(fp);
    
    /* Test very long string */
    char *long_string = malloc(10000);
    assert(long_string != NULL);
    memset(long_string, 'A', 9999);
    long_string[9999] = '\0';
    
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = fputs(long_string, fp);
    assert(ret >= 0);
    
    fclose(fp);
    free(long_string);
    
    TEST_PASSED("fputs()");
}

/* Test fgets() */
void test_fgets(void) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char small_buffer[SMALL_BUFFER_SIZE];
    char *result;
    
    TEST_START("fgets()");
    
    /* Create test file with known content */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "First line\n");
    fprintf(fp, "Second line\n");
    fprintf(fp, "Line without newline");
    fclose(fp);
    
    /* Test reading lines */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    /* Read first line */
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    assert(strcmp(buffer, "First line\n") == 0);
    
    /* Read second line */
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    assert(strcmp(buffer, "Second line\n") == 0);
    
    /* Read line without newline */
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    assert(strcmp(buffer, "Line without newline") == 0);
    
    /* Try to read past EOF */
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == NULL);
    assert(feof(fp));
    
    fclose(fp);
    
    /* Test with small buffer (line truncation) */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(small_buffer, SMALL_BUFFER_SIZE, fp);
    assert(result == small_buffer);
    assert(strlen(small_buffer) == SMALL_BUFFER_SIZE - 1);
    assert(small_buffer[SMALL_BUFFER_SIZE - 1] == '\0');
    assert(strncmp(small_buffer, "First lin", SMALL_BUFFER_SIZE - 1) == 0);
    
    fclose(fp);
    
    /* Test with buffer size 1 (only null terminator) */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(buffer, 1, fp);
    assert(result == buffer);
    assert(buffer[0] == '\0');
    
    fclose(fp);
    
    /* Test with empty file */
    fp = fopen(TEST_FILE, "w");
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == NULL);
    assert(feof(fp));
    
    fclose(fp);
    
    /* Test with file containing only newlines */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "\n\n\n");
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    assert(strcmp(buffer, "\n") == 0);
    
    fclose(fp);
    
    /* Test with binary data */
    fp = fopen(TEST_FILE, "wb");
    assert(fp != NULL);
    fprintf(fp, "Binary\x00\x01\x02\nData");
    fclose(fp);
    
    fp = fopen(TEST_FILE, "rb");
    assert(fp != NULL);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    /* fgets stops at newline, not null byte */
    assert(memcmp(buffer, "Binary\x00\x01\x02\n", 10) == 0);
    
    fclose(fp);
    
    TEST_PASSED("fgets()");
}

/* Test puts() */
void test_puts(void) {
    int ret;
    
    TEST_START("puts()");
    
    /* Test basic puts */
    printf("  Visual test - you should see: Hello from puts!\n  ");
    ret = puts("Hello from puts!");
    assert(ret >= 0); /* Non-negative on success */
    
    /* Test empty string */
    printf("  Visual test - you should see empty line:\n  ");
    ret = puts("");
    assert(ret >= 0);
    
    /* Test string without newline (puts adds one) */
    printf("  Visual test - you should see: No newline (with newline added)\n  ");
    ret = puts("No newline (with newline added)");
    assert(ret >= 0);
    
    /* Test with special characters */
    printf("  Visual test - you should see: Tab[TAB]here\n  ");
    ret = puts("Tab\there");
    assert(ret >= 0);
    
    /* Note: puts() always writes to stdout, so error testing is limited */
    
    TEST_PASSED("puts()");
}

/* Test gets() - deprecated but part of PSE51 */
void test_gets(void) {
    TEST_START("gets()");
    
    /* Note: gets() is deprecated and dangerous (no buffer size limit) */
    /* It may not be implemented in mlibc */
    /* For safety, we'll skip actual testing of gets() */
    
    printf("  Note: gets() is deprecated and unsafe, skipping tests\n");
    printf("  gets() tests skipped!\n");
}

/* Test string I/O line ending handling */
void test_line_endings(void) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char *result;
    
    TEST_START("line ending handling");
    
    /* Test Unix line endings (LF) */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fputs("Unix line\n", fp);
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result != NULL);
    assert(strcmp(buffer, "Unix line\n") == 0);
    fclose(fp);
    
    /* Test carriage return handling */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fputs("Line with CR\r", fp);
    fputs("Next line", fp);
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result != NULL);
    /* Behavior with \r is implementation-defined */
    fclose(fp);
    
    /* Test mixed line endings */
    fp = fopen(TEST_FILE, "wb"); /* Binary mode to preserve endings */
    assert(fp != NULL);
    fputs("Line1\n", fp);
    fputs("Line2\r\n", fp);
    fputs("Line3\r", fp);
    fputs("Line4", fp);
    fclose(fp);
    
    fp = fopen(TEST_FILE, "rb");
    assert(fp != NULL);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result != NULL);
    assert(strcmp(buffer, "Line1\n") == 0);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result != NULL);
    /* May be "Line2\r\n" or "Line2\r" depending on implementation */
    
    fclose(fp);
    
    TEST_PASSED("Line ending");
}

/* Test string I/O with unicode/multibyte (basic) */
void test_multibyte_strings(void) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    const char *utf8_string = "Hello, 世界! 🌍"; /* UTF-8 encoded */
    int ret;
    char *result;
    
    TEST_START("multibyte string I/O");
    
    /* Test fputs with UTF-8 */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = fputs(utf8_string, fp);
    assert(ret >= 0);
    fputs("\n", fp);
    
    fclose(fp);
    
    /* Test fgets with UTF-8 */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    /* Remove newline for comparison */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    assert(strcmp(buffer, utf8_string) == 0);
    
    fclose(fp);
    
    TEST_PASSED("Multibyte string");
}

/* Test string I/O buffer boundary conditions */
void test_buffer_boundaries(void) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char exact_buffer[11]; /* Exactly sized for "0123456789\0" */
    char *result;
    int i;
    
    TEST_START("buffer boundary conditions");
    
    /* Create file with exact-fit line */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fputs("0123456789", fp); /* 10 chars, no newline */
    fclose(fp);
    
    /* Test reading with exact-fit buffer */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(exact_buffer, sizeof(exact_buffer), fp);
    assert(result == exact_buffer);
    assert(strcmp(exact_buffer, "0123456789") == 0);
    assert(strlen(exact_buffer) == 10);
    
    fclose(fp);
    
    /* Test reading with buffer one byte too small */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(exact_buffer, sizeof(exact_buffer) - 1, fp);
    assert(result == exact_buffer);
    assert(strcmp(exact_buffer, "012345678") == 0);
    assert(strlen(exact_buffer) == 9);
    
    fclose(fp);
    
    /* Test with very long line */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    for (i = 0; i < 1000; i++) {
        fputc('A' + (i % 26), fp);
    }
    fputc('\n', fp);
    
    fclose(fp);
    
    /* Read long line with small buffer */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(buffer, 50, fp);
    assert(result == buffer);
    assert(strlen(buffer) == 49); /* 48 chars + null */
    assert(buffer[48] == '\0');
    
    /* Continue reading the rest of the line */
    result = fgets(buffer, 50, fp);
    assert(result == buffer);
    
    fclose(fp);
    
    TEST_PASSED("Buffer boundary");
}

/* Test string I/O error recovery */
void test_error_recovery(void) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    int ret;
    char *result;
    
    TEST_START("error recovery");
    
    /* Test recovery after failed fputs */
    fp = fopen(TEST_FILE, "r"); /* Read-only */
    assert(fp != NULL);
    
    ret = fputs("This fails", fp);
    assert(ret == EOF);
    
    /* File should still be usable for reading */
    fclose(fp);
    
    /* Create file with content */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fputs("Recovery test\n", fp);
    fclose(fp);
    
    /* Test fgets after error */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    result = fgets(buffer, BUFFER_SIZE, fp);
    assert(result == buffer);
    assert(strcmp(buffer, "Recovery test\n") == 0);
    
    fclose(fp);
    
    TEST_PASSED("Error recovery");
}

/* Clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdio.h String I/O Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any existing test files */
    cleanup_files();
    
    /* Run tests */
    test_fputs();
    test_fgets();
    test_puts();
    test_gets();
    test_line_endings();
    test_multibyte_strings();
    test_buffer_boundaries();
    test_error_recovery();
    
    /* Clean up test files */
    cleanup_files();
    
    TEST_SUITE_PASSED("string I/O");
    return 0;
}
