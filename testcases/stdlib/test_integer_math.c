/*
 * test_integer_math.c - PSE51 stdlib.h integer mathematics test suite
 * Tests: abs, labs, llabs, div, ldiv, lldiv
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for integer math functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>

#include "../test_colors.h"
/* Test abs() function */
void test_abs(void) {
    int result;
    
    TEST_START("abs()");
    
    /* Test positive numbers */
    result = abs(42);
    assert(result == 42);
    
    result = abs(0);
    assert(result == 0);
    
    result = abs(1);
    assert(result == 1);
    
    /* Test negative numbers */
    result = abs(-42);
    assert(result == 42);
    
    result = abs(-1);
    assert(result == 1);
    
    /* Test limits */
    result = abs(INT_MAX);
    assert(result == INT_MAX);
    
    /* Test INT_MIN (special case - may overflow) */
    if (INT_MIN == -INT_MAX - 1) {
        /* Two's complement system */
        result = abs(INT_MIN);
        /* Result is undefined behavior, but typically returns INT_MIN */
        /* Don't assert specific value due to UB */
    }
    
    /* Test near limits */
    result = abs(INT_MAX - 1);
    assert(result == INT_MAX - 1);
    
    result = abs(INT_MIN + 1);
    assert(result == INT_MAX);
    
    /* Test common values */
    result = abs(-100);
    assert(result == 100);
    
    result = abs(-999);
    assert(result == 999);
    
    TEST_PASSED("abs()");
}

/* Test labs() function */
void test_labs(void) {
    long result;
    
    TEST_START("labs()");
    
    /* Test positive numbers */
    result = labs(123456789L);
    assert(result == 123456789L);
    
    result = labs(0L);
    assert(result == 0L);
    
    /* Test negative numbers */
    result = labs(-123456789L);
    assert(result == 123456789L);
    
    result = labs(-1L);
    assert(result == 1L);
    
    /* Test limits */
    result = labs(LONG_MAX);
    assert(result == LONG_MAX);
    
    /* Test LONG_MIN (special case) */
    if (LONG_MIN == -LONG_MAX - 1) {
        result = labs(LONG_MIN);
        /* Undefined behavior, don't assert */
    }
    
    result = labs(LONG_MIN + 1);
    assert(result == LONG_MAX);
    
    /* Test various values */
    result = labs(-1000000L);
    assert(result == 1000000L);
    
    result = labs(-9999999L);
    assert(result == 9999999L);
    
    TEST_PASSED("labs()");
}

/* Test llabs() function */
void test_llabs(void) {
    long long result;
    
    TEST_START("llabs()");
    
    /* Test positive numbers */
    result = llabs(1234567890123LL);
    assert(result == 1234567890123LL);
    
    result = llabs(0LL);
    assert(result == 0LL);
    
    /* Test negative numbers */
    result = llabs(-1234567890123LL);
    assert(result == 1234567890123LL);
    
    result = llabs(-1LL);
    assert(result == 1LL);
    
    /* Test limits */
    result = llabs(LLONG_MAX);
    assert(result == LLONG_MAX);
    
    /* Test LLONG_MIN (special case) */
    if (LLONG_MIN == -LLONG_MAX - 1) {
        result = llabs(LLONG_MIN);
        /* Undefined behavior, don't assert */
    }
    
    result = llabs(LLONG_MIN + 1);
    assert(result == LLONG_MAX);
    
    /* Test large values */
    result = llabs(-999999999999LL);
    assert(result == 999999999999LL);
    
    result = llabs(-1000000000000000LL);
    assert(result == 1000000000000000LL);
    
    TEST_PASSED("llabs()");
}

/* Test div() function */
void test_div(void) {
    div_t result;
    
    TEST_START("div()");
    
    /* Test basic division */
    result = div(10, 3);
    assert(result.quot == 3);
    assert(result.rem == 1);
    
    result = div(20, 4);
    assert(result.quot == 5);
    assert(result.rem == 0);
    
    /* Test negative dividends */
    result = div(-10, 3);
    assert(result.quot == -3);
    assert(result.rem == -1);
    
    result = div(-20, 4);
    assert(result.quot == -5);
    assert(result.rem == 0);
    
    /* Test negative divisors */
    result = div(10, -3);
    assert(result.quot == -3);
    assert(result.rem == 1);
    
    result = div(-10, -3);
    assert(result.quot == 3);
    assert(result.rem == -1);
    
    /* Test division by 1 and -1 */
    result = div(42, 1);
    assert(result.quot == 42);
    assert(result.rem == 0);
    
    result = div(42, -1);
    assert(result.quot == -42);
    assert(result.rem == 0);
    
    /* Test zero dividend */
    result = div(0, 5);
    assert(result.quot == 0);
    assert(result.rem == 0);
    
    /* Test identity: dividend = divisor * quot + rem */
    int dividend = 17;
    int divisor = 5;
    result = div(dividend, divisor);
    assert(dividend == divisor * result.quot + result.rem);
    
    /* Test various cases */
    result = div(100, 7);
    assert(result.quot == 14);
    assert(result.rem == 2);
    
    result = div(-100, 7);
    assert(result.quot == -14);
    assert(result.rem == -2);
    
    result = div(100, -7);
    assert(result.quot == -14);
    assert(result.rem == 2);
    
    result = div(-100, -7);
    assert(result.quot == 14);
    assert(result.rem == -2);
    
    /* Test edge cases */
    result = div(INT_MAX, 2);
    assert(result.quot == INT_MAX / 2);
    assert(result.rem == INT_MAX % 2);
    
    result = div(INT_MIN, 2);
    assert(result.quot == INT_MIN / 2);
    assert(result.rem == 0);
    
    /* Verify sign of remainder matches dividend (C99 requirement) */
    result = div(7, 3);
    assert(result.rem >= 0);
    
    result = div(-7, 3);
    assert(result.rem <= 0);
    
    TEST_PASSED("div()");
}

/* Test ldiv() function */
void test_ldiv(void) {
    ldiv_t result;
    
    TEST_START("ldiv()");
    
    /* Test basic division */
    result = ldiv(1000000L, 7L);
    assert(result.quot == 142857L);
    assert(result.rem == 1L);
    
    result = ldiv(123456789L, 1000L);
    assert(result.quot == 123456L);
    assert(result.rem == 789L);
    
    /* Test negative numbers */
    result = ldiv(-1000000L, 7L);
    assert(result.quot == -142857L);
    assert(result.rem == -1L);
    
    result = ldiv(1000000L, -7L);
    assert(result.quot == -142857L);
    assert(result.rem == 1L);
    
    result = ldiv(-1000000L, -7L);
    assert(result.quot == 142857L);
    assert(result.rem == -1L);
    
    /* Test large values */
    result = ldiv(LONG_MAX, 2L);
    assert(result.quot == LONG_MAX / 2L);
    assert(result.rem == LONG_MAX % 2L);
    
    /* Test identity */
    long dividend = 999999999L;
    long divisor = 12345L;
    result = ldiv(dividend, divisor);
    assert(dividend == divisor * result.quot + result.rem);
    
    /* Test zero dividend */
    result = ldiv(0L, 100L);
    assert(result.quot == 0L);
    assert(result.rem == 0L);
    
    /* Test power of 2 divisors */
    result = ldiv(1000L, 8L);
    assert(result.quot == 125L);
    assert(result.rem == 0L);
    
    result = ldiv(1023L, 16L);
    assert(result.quot == 63L);
    assert(result.rem == 15L);
    
    TEST_PASSED("ldiv()");
}

/* Test lldiv() function */
void test_lldiv(void) {
    lldiv_t result;
    
    TEST_START("lldiv()");
    
    /* Test basic division */
    result = lldiv(1000000000000LL, 7LL);
    assert(result.quot == 142857142857LL);
    assert(result.rem == 1LL);
    
    /* Test very large numbers */
    result = lldiv(9223372036854775807LL, 1000000000LL);
    assert(result.quot == 9223372036LL);
    assert(result.rem == 854775807LL);
    
    /* Test negative numbers */
    result = lldiv(-1000000000000LL, 7LL);
    assert(result.quot == -142857142857LL);
    assert(result.rem == -1LL);
    
    /* Test limits */
    result = lldiv(LLONG_MAX, 2LL);
    assert(result.quot == LLONG_MAX / 2LL);
    assert(result.rem == LLONG_MAX % 2LL);
    
    /* Test identity */
    long long dividend = 123456789012345LL;
    long long divisor = 9876543LL;
    result = lldiv(dividend, divisor);
    assert(dividend == divisor * result.quot + result.rem);
    
    /* Test various cases */
    result = lldiv(1LL << 50, 1LL << 10);
    assert(result.quot == 1LL << 40);
    assert(result.rem == 0LL);
    
    result = lldiv((1LL << 50) + 1023LL, 1LL << 10);
    assert(result.quot == 1LL << 40);
    assert(result.rem == 1023LL);
    
    TEST_PASSED("lldiv()");
}

/* Test edge cases for all functions */
void test_edge_cases(void) {
    div_t div_result;
    ldiv_t ldiv_result;
    lldiv_t lldiv_result;
    
    TEST_START("edge cases");
    
    /* Test division with quot = 0 */
    div_result = div(5, 10);
    assert(div_result.quot == 0);
    assert(div_result.rem == 5);
    
    ldiv_result = ldiv(5L, 10L);
    assert(ldiv_result.quot == 0L);
    assert(ldiv_result.rem == 5L);
    
    /* Test division where dividend equals divisor */
    div_result = div(100, 100);
    assert(div_result.quot == 1);
    assert(div_result.rem == 0);
    
    /* Test consecutive divisions */
    div_result = div(100, 7);
    int remainder = div_result.rem;
    div_result = div(remainder, 3);
    assert(div_result.quot == remainder / 3);
    assert(div_result.rem == remainder % 3);
    
    /* Test symmetry for negative operands */
    div_result = div(10, 3);
    int pos_quot = div_result.quot;
    int pos_rem = div_result.rem;
    
    div_result = div(-10, -3);
    assert(div_result.quot == pos_quot);
    assert(div_result.rem == -pos_rem);
    
    TEST_PASSED("Edge case");
}

/* Test performance patterns */
void test_performance_patterns(void) {
    int i;
    div_t result;
    
    TEST_START("performance patterns");
    
    /* Pattern 1: Repeated divisions (modular arithmetic) */
    int value = 1000000;
    int count = 0;
    while (value > 0) {
        result = div(value, 10);
        value = result.quot;
        count++;
    }
    assert(count == 7); /* 1000000 has 7 digits */
    
    /* Pattern 2: Extracting digits */
    value = 12345;
    int digits[5];
    for (i = 4; i >= 0; i--) {
        result = div(value, 10);
        digits[i] = result.rem;
        value = result.quot;
    }
    assert(digits[0] == 1);
    assert(digits[1] == 2);
    assert(digits[2] == 3);
    assert(digits[3] == 4);
    assert(digits[4] == 5);
    
    /* Pattern 3: Base conversion */
    value = 255;
    char hex[10];
    int pos = 0;
    while (value > 0) {
        result = div(value, 16);
        int digit = result.rem;
        hex[pos++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        value = result.quot;
    }
    assert(pos == 2);
    assert(hex[0] == 'F');
    assert(hex[1] == 'F');
    
    TEST_PASSED("Performance pattern");
}

/* Test mathematical properties */
void test_mathematical_properties(void) {
    div_t d;
    ldiv_t ld;
    int a, b;
    
    TEST_START("mathematical properties");
    
    /* Test Euclidean division property */
    for (a = -10; a <= 10; a++) {
        for (b = -10; b <= 10; b++) {
            if (b == 0) continue;
            
            d = div(a, b);
            /* a = b * q + r, where |r| < |b| */
            assert(a == b * d.quot + d.rem);
            assert(abs(d.rem) < abs(b));
            
            /* Sign of remainder matches dividend (C99) */
            if (a != 0) {
                assert((a > 0 && d.rem >= 0) || (a < 0 && d.rem <= 0));
            }
        }
    }
    
    /* Test quotient rounding toward zero */
    d = div(7, 3);
    assert(d.quot == 2);  /* Not 3 */
    
    d = div(-7, 3);
    assert(d.quot == -2); /* Not -3 */
    
    d = div(7, -3);
    assert(d.quot == -2); /* Not -3 */
    
    d = div(-7, -3);
    assert(d.quot == 2);  /* Not 3 */
    
    /* Test consistency across types */
    int int_quot = 100 / 7;
    int int_rem = 100 % 7;
    d = div(100, 7);
    assert(d.quot == int_quot);
    assert(d.rem == int_rem);
    
    long long_quot = 100L / 7L;
    long long_rem = 100L % 7L;
    ld = ldiv(100L, 7L);
    assert(ld.quot == long_quot);
    assert(ld.rem == long_rem);
    
    TEST_PASSED("Mathematical property");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdlib.h Integer Math Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_abs();
    test_labs();
    test_llabs();
    test_div();
    test_ldiv();
    test_lldiv();
    test_edge_cases();
    test_performance_patterns();
    test_mathematical_properties();
    
    TEST_SUITE_PASSED("integer math");
    return 0;
}
