/*
 * test_string_conversion.c - PSE51 stdlib.h string conversion test suite
 * Tests: atoi, atol, atoll, atof, strtol, strtoll, strtoul, strtoull, strtod, strtof, strtold
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for string conversion functions
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <float.h>

#include "../test_colors.h"
/* Test atoi() function */
void test_atoi(void) {
    int result;
    
    TEST_START("atoi()");
    
    /* Test basic conversions */
    result = atoi("123");
    assert(result == 123);
    
    result = atoi("-456");
    assert(result == -456);
    
    result = atoi("0");
    assert(result == 0);
    
    result = atoi("+789");
    assert(result == 789);
    
    /* Test with leading whitespace */
    result = atoi("  \t\n  42");
    assert(result == 42);
    
    /* Test with trailing non-numeric characters */
    result = atoi("123abc");
    assert(result == 123);
    
    result = atoi("45 67");
    assert(result == 45);
    
    /* Test invalid input */
    result = atoi("abc");
    assert(result == 0);
    
    result = atoi("");
    assert(result == 0);
    
    result = atoi("   ");
    assert(result == 0);
    
    /* Test overflow behavior (implementation-defined) */
    result = atoi("2147483647"); /* INT_MAX */
    assert(result == INT_MAX);
    
    result = atoi("-2147483648"); /* INT_MIN */
    assert(result == INT_MIN);
    
    /* Test with plus/minus only */
    result = atoi("+");
    assert(result == 0);
    
    result = atoi("-");
    assert(result == 0);
    
    /* Test hexadecimal notation (should stop at 'x') */
    result = atoi("0x123");
    assert(result == 0);
    
    /* Test octal notation */
    result = atoi("0123");
    assert(result == 123); /* Decimal, not octal */
    
    TEST_PASSED("atoi()");
}

/* Test atol() function */
void test_atol(void) {
    long result;
    
    TEST_START("atol()");
    
    /* Test basic conversions */
    result = atol("123456789");
    assert(result == 123456789L);
    
    result = atol("-987654321");
    assert(result == -987654321L);
    
    /* Test limits */
    char buf[64];
    sprintf(buf, "%ld", LONG_MAX);
    result = atol(buf);
    assert(result == LONG_MAX);
    
    sprintf(buf, "%ld", LONG_MIN);
    result = atol(buf);
    assert(result == LONG_MIN);
    
    /* Test with whitespace and invalid chars */
    result = atol("  123  abc");
    assert(result == 123L);
    
    result = atol("xyz");
    assert(result == 0L);
    
    TEST_PASSED("atol()");
}

/* Test atoll() function */
void test_atoll(void) {
    long long result;
    
    TEST_START("atoll()");
    
    /* Test basic conversions */
    result = atoll("1234567890123");
    assert(result == 1234567890123LL);
    
    result = atoll("-9876543210987");
    assert(result == -9876543210987LL);
    
    /* Test limits */
    char buf[64];
    sprintf(buf, "%lld", LLONG_MAX);
    result = atoll(buf);
    assert(result == LLONG_MAX);
    
    sprintf(buf, "%lld", LLONG_MIN);
    result = atoll(buf);
    assert(result == LLONG_MIN);
    
    /* Test edge cases */
    result = atoll("  +42  ");
    assert(result == 42LL);
    
    result = atoll("0");
    assert(result == 0LL);
    
    TEST_PASSED("atoll()");
}

/* Test atof() function */
void test_atof(void) {
    double result;
    double epsilon = 1e-9;
    
    TEST_START("atof()");
    
    /* Test basic conversions */
    result = atof("123.456");
    assert(result >= 123.456 - epsilon && result <= 123.456 + epsilon);
    
    result = atof("-78.9");
    assert(result >= -78.9 - epsilon && result <= -78.9 + epsilon);
    
    result = atof("0.0");
    assert(result == 0.0);
    
    /* Test scientific notation */
    result = atof("1.23e4");
    assert(result >= 12300.0 - epsilon && result <= 12300.0 + epsilon);
    
    result = atof("1.23E-4");
    assert(result >= 0.000123 - epsilon && result <= 0.000123 + epsilon);
    
    /* Test special cases */
    result = atof("  3.14159  ");
    assert(result >= 3.14159 - epsilon && result <= 3.14159 + epsilon);
    
    result = atof("123");  /* Integer string */
    assert(result == 123.0);
    
    result = atof(".456");  /* No leading digit */
    assert(result >= 0.456 - epsilon && result <= 0.456 + epsilon);
    
    result = atof("789.");  /* No trailing digit */
    assert(result == 789.0);
    
    /* Test invalid input */
    result = atof("abc");
    assert(result == 0.0);
    
    result = atof("");
    assert(result == 0.0);
    
    /* Test infinity and NaN if supported */
    result = atof("inf");
    /* Implementation-defined */
    
    result = atof("-inf");
    /* Implementation-defined */
    
    TEST_PASSED("atof()");
}

/* Test strtol() function */
void test_strtol(void) {
    long result;
    char *endptr;
    
    TEST_START("strtol()");
    
    /* Test basic decimal conversion */
    result = strtol("123", &endptr, 10);
    assert(result == 123);
    assert(*endptr == '\0');
    
    result = strtol("-456", &endptr, 10);
    assert(result == -456);
    assert(*endptr == '\0');
    
    /* Test with different bases */
    result = strtol("1111", &endptr, 2);  /* Binary */
    assert(result == 15);
    assert(*endptr == '\0');
    
    result = strtol("777", &endptr, 8);   /* Octal */
    assert(result == 511);
    assert(*endptr == '\0');
    
    result = strtol("FF", &endptr, 16);   /* Hexadecimal */
    assert(result == 255);
    assert(*endptr == '\0');
    
    result = strtol("0xFF", &endptr, 16); /* Hex with prefix */
    assert(result == 255);
    assert(*endptr == '\0');
    
    result = strtol("0xff", &endptr, 16); /* Lowercase hex */
    assert(result == 255);
    assert(*endptr == '\0');
    
    /* Test base 0 (auto-detect) */
    result = strtol("123", &endptr, 0);   /* Decimal */
    assert(result == 123);
    
    result = strtol("0123", &endptr, 0);  /* Octal */
    assert(result == 83);
    
    result = strtol("0x123", &endptr, 0); /* Hex */
    assert(result == 291);
    
    /* Test with trailing characters */
    result = strtol("123abc", &endptr, 10);
    assert(result == 123);
    assert(strcmp(endptr, "abc") == 0);
    
    /* Test with leading whitespace */
    result = strtol("  \t 42", &endptr, 10);
    assert(result == 42);
    assert(*endptr == '\0');
    
    /* Test overflow detection */
    errno = 0;
    result = strtol("99999999999999999999999", &endptr, 10);
    assert(result == LONG_MAX);
    assert(errno == ERANGE);
    
    errno = 0;
    result = strtol("-99999999999999999999999", &endptr, 10);
    assert(result == LONG_MIN);
    assert(errno == ERANGE);
    
    /* Test invalid input */
    errno = 0;
    result = strtol("abc", &endptr, 10);
    assert(result == 0);
    assert(endptr[0] == 'a');
    
    /* Test NULL endptr */
    result = strtol("42", NULL, 10);
    assert(result == 42);
    
    /* Test empty string */
    result = strtol("", &endptr, 10);
    assert(result == 0);
    assert(*endptr == '\0');
    
    /* Test base out of range */
    errno = 0;
    result = strtol("123", &endptr, 1);   /* Invalid base */
    assert(errno == EINVAL || result == 0);
    
    errno = 0;
    result = strtol("123", &endptr, 37);  /* Invalid base */
    assert(errno == EINVAL || result == 0);
    
    /* Test base 36 (maximum valid base) */
    result = strtol("z", &endptr, 36);
    assert(result == 35);
    
    result = strtol("10", &endptr, 36);
    assert(result == 36);
    
    TEST_PASSED("strtol()");
}

/* Test strtoll() function */
void test_strtoll(void) {
    long long result;
    char *endptr;
    
    TEST_START("strtoll()");
    
    /* Test basic conversion */
    result = strtoll("1234567890123", &endptr, 10);
    assert(result == 1234567890123LL);
    assert(*endptr == '\0');
    
    /* Test negative numbers */
    result = strtoll("-9876543210987", &endptr, 10);
    assert(result == -9876543210987LL);
    assert(*endptr == '\0');
    
    /* Test limits */
    char buf[64];
    sprintf(buf, "%lld", LLONG_MAX);
    errno = 0;
    result = strtoll(buf, &endptr, 10);
    assert(result == LLONG_MAX);
    assert(errno == 0);
    
    sprintf(buf, "%lld", LLONG_MIN);
    errno = 0;
    result = strtoll(buf, &endptr, 10);
    assert(result == LLONG_MIN);
    assert(errno == 0);
    
    /* Test overflow */
    errno = 0;
    result = strtoll("9999999999999999999999999999", &endptr, 10);
    assert(result == LLONG_MAX);
    assert(errno == ERANGE);
    
    TEST_PASSED("strtoll()");
}

/* Test strtoul() function */
void test_strtoul(void) {
    unsigned long result;
    char *endptr;
    
    TEST_START("strtoul()");
    
    /* Test basic conversion */
    result = strtoul("123456789", &endptr, 10);
    assert(result == 123456789UL);
    assert(*endptr == '\0');
    
    /* Test maximum value */
    char buf[64];
    sprintf(buf, "%lu", ULONG_MAX);
    errno = 0;
    result = strtoul(buf, &endptr, 10);
    assert(result == ULONG_MAX);
    assert(errno == 0);
    
    /* Test overflow */
    errno = 0;
    result = strtoul("99999999999999999999999999", &endptr, 10);
    assert(result == ULONG_MAX);
    assert(errno == ERANGE);
    
    /* Test negative number (wraps around) */
    result = strtoul("-1", &endptr, 10);
    assert(result == ULONG_MAX);
    
    /* Test hex conversion */
    result = strtoul("DEADBEEF", &endptr, 16);
    assert(result == 0xDEADBEEF);
    
    /* Test octal conversion */
    result = strtoul("755", &endptr, 8);
    assert(result == 493); /* 7*64 + 5*8 + 5 */
    
    TEST_PASSED("strtoul()");
}

/* Test strtoull() function */
void test_strtoull(void) {
    unsigned long long result;
    char *endptr;
    
    TEST_START("strtoull()");
    
    /* Test basic conversion */
    result = strtoull("18446744073709551615", &endptr, 10);
    assert(result == ULLONG_MAX);
    assert(*endptr == '\0');
    
    /* Test overflow */
    errno = 0;
    result = strtoull("99999999999999999999999999999", &endptr, 10);
    assert(result == ULLONG_MAX);
    assert(errno == ERANGE);
    
    /* Test binary conversion */
    result = strtoull("1111111111111111", &endptr, 2);
    assert(result == 65535);
    
    /* Test base 36 */
    result = strtoull("ZZZZZ", &endptr, 36);
    /* Calculate: 35*36^4 + 35*36^3 + 35*36^2 + 35*36 + 35 */
    assert(result == 60466175);
    
    TEST_PASSED("strtoull()");
}

/* Test strtod() function */
void test_strtod(void) {
    double result;
    char *endptr;
    double epsilon = 1e-9;
    
    TEST_START("strtod()");
    
    /* Test basic conversion */
    result = strtod("123.456", &endptr);
    assert(result >= 123.456 - epsilon && result <= 123.456 + epsilon);
    assert(*endptr == '\0');
    
    /* Test scientific notation */
    result = strtod("1.23e10", &endptr);
    assert(result >= 1.23e10 - 1e1 && result <= 1.23e10 + 1e1);
    assert(*endptr == '\0');
    
    result = strtod("1.23E-10", &endptr);
    assert(result >= 1.23e-10 - 1e-19 && result <= 1.23e-10 + 1e-19);
    assert(*endptr == '\0');
    
    /* Test special values */
    result = strtod("0", &endptr);
    assert(result == 0.0);
    
    result = strtod("-0", &endptr);
    assert(result == -0.0);
    
    /* Test with plus sign */
    result = strtod("+3.14", &endptr);
    assert(result >= 3.14 - epsilon && result <= 3.14 + epsilon);
    
    /* Test no leading digit */
    result = strtod(".5", &endptr);
    assert(result >= 0.5 - epsilon && result <= 0.5 + epsilon);
    
    /* Test no decimal part */
    result = strtod("42.", &endptr);
    assert(result == 42.0);
    
    /* Test whitespace */
    result = strtod("  \t\n  2.718", &endptr);
    assert(result >= 2.718 - epsilon && result <= 2.718 + epsilon);
    
    /* Test trailing characters */
    result = strtod("3.14abc", &endptr);
    assert(result >= 3.14 - epsilon && result <= 3.14 + epsilon);
    assert(strcmp(endptr, "abc") == 0);
    
    /* Test overflow/underflow */
    errno = 0;
    result = strtod("1e400", &endptr);  /* Overflow */
    /* Result should be HUGE_VAL or inf */
    assert(errno == ERANGE);
    
    errno = 0;
    result = strtod("1e-400", &endptr); /* Underflow */
    /* Result should be close to 0 */
    assert(result >= 0.0 && result <= epsilon);
    
    /* Test invalid input */
    result = strtod("abc", &endptr);
    assert(result == 0.0);
    assert(endptr[0] == 'a');
    
    /* Test infinity (if supported) */
    result = strtod("inf", &endptr);
    /* Implementation-defined */
    
    result = strtod("infinity", &endptr);
    /* Implementation-defined */
    
    result = strtod("-inf", &endptr);
    /* Implementation-defined */
    
    /* Test NaN (if supported) */
    result = strtod("nan", &endptr);
    /* Implementation-defined */
    
    /* Test hex float (C99, may not be supported) */
    result = strtod("0x1.8p+0", &endptr);
    /* Should be 1.5 if supported */
    
    TEST_PASSED("strtod()");
}

/* Test edge cases for all conversion functions */
void test_conversion_edge_cases(void) {
    char *endptr;
    long result;
    
    TEST_START("conversion edge cases");
    
    /* Test multiple signs */
    result = strtol("++123", &endptr, 10);
    assert(result == 0);
    assert(endptr[0] == '+');
    
    result = strtol("--123", &endptr, 10);
    assert(result == 0);
    assert(endptr[0] == '-');
    
    result = strtol("+-123", &endptr, 10);
    assert(result == 0);
    assert(endptr[0] == '-');
    
    /* Test base prefixes with wrong base */
    result = strtol("0x123", &endptr, 10);
    assert(result == 0);
    assert(endptr[0] == 'x');
    
    /* Test invalid digits for base */
    result = strtol("012389", &endptr, 8);
    assert(result == 83);  /* 0123 in octal */
    assert(endptr[0] == '8');
    
    result = strtol("ABCG", &endptr, 16);
    assert(result == 0xABC);
    assert(endptr[0] == 'G');
    
    /* Test very long input */
    char longstr[1000];
    memset(longstr, '1', 999);
    longstr[999] = '\0';
    
    errno = 0;
    result = strtol(longstr, &endptr, 10);
    assert(result == LONG_MAX);
    assert(errno == ERANGE);
    
    /* Test mixed case in hex */
    result = strtol("0xAbCdEf", &endptr, 16);
    assert(result == 0xABCDEF);
    
    result = strtol("0XaBcDeF", &endptr, 16);
    assert(result == 0xABCDEF);
    
    TEST_PASSED("Conversion edge case");
}

/* Test locale-dependent behavior (basic) */
void test_locale_behavior(void) {
    double result;
    char *endptr;
    
    TEST_START("locale-dependent behavior");
    
    /* In "C" locale, decimal point is '.' */
    result = strtod("123.456", &endptr);
    assert(result > 123.0 && result < 124.0);
    
    /* Comma should not be decimal separator in C locale */
    result = strtod("123,456", &endptr);
    assert(result == 123.0);
    assert(endptr[0] == ',');
    
    TEST_PASSED("Locale behavior");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdlib.h String Conversion Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_atoi();
    test_atol();
    test_atoll();
    test_atof();
    test_strtol();
    test_strtoll();
    test_strtoul();
    test_strtoull();
    test_strtod();
    test_conversion_edge_cases();
    test_locale_behavior();
    
    TEST_SUITE_PASSED("string conversion");
}
