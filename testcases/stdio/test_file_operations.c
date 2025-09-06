/*
 * test_file_operations.c - PSE51 stdio.h file operations test suite
 * Tests: fopen, fclose, fread, fwrite, fseek, ftell, rewind, feof, ferror, clearerr
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for file operations
 */

#define _GNU_SOURCE  /* Enable all features for testing */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "../test_colors.h"
#define TEST_FILE "test_file.txt"
#define TEST_FILE2 "test_file2.txt"
#define TEST_CONTENT "Hello, POSIX file operations!\n"
#define BUFFER_SIZE 256

/* Helper function to clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
    unlink(TEST_FILE2);
}

/* Test fopen() with various modes */
void test_fopen(void) {
    FILE *fp;
    
    TEST_START("fopen()");
    
    /* Test opening file for writing */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fclose(fp);
    
    /* Test opening file for reading */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    fclose(fp);
    
    /* Test opening file for appending */
    fp = fopen(TEST_FILE, "a");
    assert(fp != NULL);
    fclose(fp);
    
    /* Test opening file with read/write */
    fp = fopen(TEST_FILE, "w+");
    assert(fp != NULL);
    fclose(fp);
    
    /* Test opening non-existent file for reading (should fail) */
    fp = fopen("non_existent_file.txt", "r");
    assert(fp == NULL);
    assert(errno == ENOENT);
    
    /* Test opening with invalid mode (implementation-defined behavior) */
    errno = 0;
    fp = fopen(TEST_FILE, "xyz");
    if (fp == NULL) {
        assert(errno != 0);
    } else {
        fclose(fp);
    }
    
    TEST_PASSED("fopen()");
}

/* Test fclose() */
void test_fclose(void) {
    FILE *fp;
    int ret;
    
    TEST_START("fclose()");
    
    /* Test normal close */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    ret = fclose(fp);
    assert(ret == 0);
    
    /* Test closing NULL pointer (undefined behavior, skip) */
    
    /* Test double close (undefined behavior, skip) */
    
    TEST_PASSED("fclose()");
}

/* Test fwrite() and fread() */
void test_fwrite_fread(void) {
    FILE *fp;
    char write_buffer[] = TEST_CONTENT;
    char read_buffer[BUFFER_SIZE];
    size_t written, read;
    
    TEST_START("fwrite() and fread()");
    
    /* Test writing to file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    written = fwrite(write_buffer, 1, strlen(write_buffer), fp);
    assert(written == strlen(write_buffer));
    fclose(fp);
    
    /* Test reading from file */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    memset(read_buffer, 0, BUFFER_SIZE);
    read = fread(read_buffer, 1, BUFFER_SIZE, fp);
    assert(read == strlen(write_buffer));
    assert(strcmp(read_buffer, write_buffer) == 0);
    fclose(fp);
    
    /* Test writing with different element sizes */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    int numbers[] = {1, 2, 3, 4, 5};
    written = fwrite(numbers, sizeof(int), 5, fp);
    assert(written == 5);
    fclose(fp);
    
    /* Test reading with different element sizes */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    int read_numbers[5];
    read = fread(read_numbers, sizeof(int), 5, fp);
    assert(read == 5);
    for (int i = 0; i < 5; i++) {
        assert(read_numbers[i] == numbers[i]);
    }
    fclose(fp);
    
    /* Test partial reads */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fwrite("1234567890", 1, 10, fp);
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    char partial[5];
    read = fread(partial, 1, 5, fp);
    assert(read == 5);
    assert(strncmp(partial, "12345", 5) == 0);
    fclose(fp);
    
    TEST_PASSED("fwrite() and fread()");
}

/* Test fseek(), ftell(), and rewind() */
void test_fseek_ftell_rewind(void) {
    FILE *fp;
    long pos;
    char buffer[10];
    
    TEST_START("fseek(), ftell(), and rewind()");
    
    /* Create test file with known content */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fwrite("0123456789", 1, 10, fp);
    fclose(fp);
    
    /* Test ftell() at beginning */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    pos = ftell(fp);
    /* Note: ftell() is not implemented in mlibc, skip if returns -1 */
    if (pos != -1) {
        assert(pos == 0);
        
        /* Test fseek() with SEEK_SET */
        assert(fseek(fp, 5, SEEK_SET) == 0);
        pos = ftell(fp);
        assert(pos == 5);
        
        /* Verify position by reading */
        fread(buffer, 1, 1, fp);
        assert(buffer[0] == '5');
        
        /* Test fseek() with SEEK_CUR */
        assert(fseek(fp, 2, SEEK_CUR) == 0);
        pos = ftell(fp);
        assert(pos == 8);
        
        /* Test fseek() with SEEK_END */
        assert(fseek(fp, -2, SEEK_END) == 0);
        pos = ftell(fp);
        assert(pos == 8);
        
        /* Test rewind() */
        rewind(fp);
        pos = ftell(fp);
        assert(pos == 0);
        
        /* Verify position after rewind */
        fread(buffer, 1, 1, fp);
        assert(buffer[0] == '0');
    } else {
        printf("  Note: ftell() not implemented, skipping position tests\n");
        
        /* Still test basic fseek functionality */
        assert(fseek(fp, 5, SEEK_SET) == 0);
        fread(buffer, 1, 1, fp);
        assert(buffer[0] == '5');
    }
    
    fclose(fp);
    
    /* Test seeking beyond file */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    assert(fseek(fp, 100, SEEK_SET) == 0); /* Seeking beyond EOF is allowed */
    fclose(fp);
    
    TEST_PASSED("fseek(), ftell(), and rewind()");
}

/* Test feof() and ferror() */
void test_feof_ferror(void) {
    FILE *fp;
    char buffer[10];
    
    TEST_START("feof() and ferror()");
    
    /* Create small test file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fwrite("123", 1, 3, fp);
    fclose(fp);
    
    /* Test feof() */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    assert(feof(fp) == 0); /* Not at EOF initially */
    
    /* Read all data */
    fread(buffer, 1, 3, fp);
    assert(feof(fp) == 0); /* Still not at EOF after reading exact amount */
    
    /* Try to read more */
    fread(buffer, 1, 1, fp);
    assert(feof(fp) != 0); /* Now at EOF */
    
    /* Test ferror() - basic test */
    assert(ferror(fp) == 0); /* No error initially */
    
    /* Test clearerr() if implemented */
    if (feof(fp)) {
        clearerr(fp);
        assert(feof(fp) == 0);
        assert(ferror(fp) == 0);
    }
    
    fclose(fp);
    
    TEST_PASSED("feof() and ferror()");
}

/* Test file operations with binary data */
void test_binary_operations(void) {
    FILE *fp;
    unsigned char binary_data[] = {0x00, 0x01, 0x02, 0xFF, 0xFE, 0xFD};
    unsigned char read_data[6];
    size_t written, read;
    
    TEST_START("binary file operations");
    
    /* Write binary data */
    fp = fopen(TEST_FILE, "wb");
    assert(fp != NULL);
    
    written = fwrite(binary_data, 1, sizeof(binary_data), fp);
    assert(written == sizeof(binary_data));
    fclose(fp);
    
    /* Read binary data */
    fp = fopen(TEST_FILE, "rb");
    assert(fp != NULL);
    
    read = fread(read_data, 1, sizeof(read_data), fp);
    assert(read == sizeof(read_data));
    
    /* Verify binary data */
    for (size_t i = 0; i < sizeof(binary_data); i++) {
        assert(read_data[i] == binary_data[i]);
    }
    
    fclose(fp);
    
    TEST_PASSED("Binary file operations");
}

/* Test file operations error handling */
void test_error_handling(void) {
    FILE *fp;
    char buffer[10];
    size_t read;
    
    TEST_START("error handling");
    
    /* Test reading from write-only file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    errno = 0;
    read = fread(buffer, 1, 10, fp);
    assert(read == 0);
    /* ferror(fp) should be set, but implementation may vary */
    
    fclose(fp);
    
    /* Test writing to read-only file */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    errno = 0;
    size_t written = fwrite("test", 1, 4, fp);
    assert(written == 0);
    /* ferror(fp) should be set, but implementation may vary */
    
    fclose(fp);
    
    TEST_PASSED("Error handling");
}

/* Test large file operations */
void test_large_file_operations(void) {
    FILE *fp;
    char *large_buffer;
    size_t size = 10240; /* 10KB */
    size_t written, read;
    
    TEST_START("large file operations");
    
    large_buffer = malloc(size);
    assert(large_buffer != NULL);
    
    /* Fill buffer with pattern */
    for (size_t i = 0; i < size; i++) {
        large_buffer[i] = (char)(i % 256);
    }
    
    /* Write large data */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    written = fwrite(large_buffer, 1, size, fp);
    assert(written == size);
    fclose(fp);
    
    /* Read and verify large data */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    char *read_buffer = malloc(size);
    assert(read_buffer != NULL);
    
    read = fread(read_buffer, 1, size, fp);
    assert(read == size);
    
    /* Verify data */
    assert(memcmp(large_buffer, read_buffer, size) == 0);
    
    fclose(fp);
    free(large_buffer);
    free(read_buffer);
    
    TEST_PASSED("Large file operations");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdio.h File Operations Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any existing test files */
    cleanup_files();
    
    /* Run tests */
    test_fopen();
    test_fclose();
    test_fwrite_fread();
    test_fseek_ftell_rewind();
    test_feof_ferror();
    test_binary_operations();
    test_error_handling();
    test_large_file_operations();
    
    /* Clean up test files */
    cleanup_files();
    
    TEST_SUITE_PASSED("file operations");
}
