#!/bin/bash
#
# run_tests.sh - Test runner script for PSE51 stdio.h test suite
# This script builds and runs all stdio tests with various configurations

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test results
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to print colored output
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓ $2${NC}"
    else
        echo -e "${RED}✗ $2${NC}"
    fi
}

# Function to run a single test
run_test() {
    local test_name=$1
    local test_binary="$test_name.out"
    
    echo -e "\n${YELLOW}Running $test_name...${NC}"
    
    if [ ! -f "$test_binary" ]; then
        echo -e "${RED}Error: $test_binary not found. Run 'make' first.${NC}"
        return 1
    fi
    
    # Create log file
    local log_file="test_results/${test_name}.log"
    mkdir -p test_results
    
    # Run the test
    if ./"$test_binary" > "$log_file" 2>&1; then
        print_status 0 "$test_name PASSED"
        ((PASSED_TESTS++))
        
        # Show summary from log
        grep -E "=== All|tests passed" "$log_file" | sed 's/^/  /'
    else
        print_status 1 "$test_name FAILED (see $log_file)"
        ((FAILED_TESTS++))
        
        # Show last few lines of error
        echo "  Last 5 lines of output:"
        tail -5 "$log_file" | sed 's/^/    /'
    fi
    
    ((TOTAL_TESTS++))
}

# Function to build tests
build_tests() {
    echo -e "${YELLOW}Building tests...${NC}"
    
    if make clean && make; then
        print_status 0 "Build successful"
        return 0
    else
        print_status 1 "Build failed"
        return 1
    fi
}

# Function to run all tests
run_all_tests() {
    local tests=(
        "test_file_operations"
        "test_formatted_io"
        "test_char_io"
        "test_string_io"
        "test_misc_stdio"
    )
    
    for test in "${tests[@]}"; do
        run_test "$test"
    done
}

# Function to run tests with valgrind (if available)
run_with_valgrind() {
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}Valgrind not found, skipping memory tests${NC}"
        return
    fi
    
    echo -e "\n${YELLOW}Running tests with Valgrind...${NC}"
    
    local tests=(
        "test_file_operations"
        "test_formatted_io"
        "test_char_io"
        "test_string_io"
        "test_misc_stdio"
    )
    
    for test in "${tests[@]}"; do
        echo -e "\n${YELLOW}Valgrind check for $test...${NC}"
        valgrind --leak-check=full --error-exitcode=1 ./"$test.out" > /dev/null 2>&1
        print_status $? "$test memory check"
    done
}

# Function to generate test report
generate_report() {
    local report_file="test_results/test_report.txt"
    
    echo -e "\n${YELLOW}Generating test report...${NC}"
    
    cat > "$report_file" << EOF
PSE51 stdio.h Test Suite Report
================================
Date: $(date)
Architecture: $(uname -m)
Compiler: $(${CC:-gcc} --version | head -1)

Test Summary
------------
Total Tests: $TOTAL_TESTS
Passed: $PASSED_TESTS
Failed: $FAILED_TESTS
Success Rate: $(awk "BEGIN {printf \"%.1f%%\", ($PASSED_TESTS/$TOTAL_TESTS)*100}")

Individual Test Results
----------------------
EOF
    
    for log in test_results/*.log; do
        if [ -f "$log" ]; then
            test_name=$(basename "$log" .log)
            echo -e "\n$test_name:" >> "$report_file"
            grep -E "=== All|tests passed|FAILED" "$log" | sed 's/^/  /' >> "$report_file"
        fi
    done
    
    echo -e "\nDetailed logs available in test_results/" >> "$report_file"
    
    print_status 0 "Report generated: $report_file"
}

# Main script
main() {
    echo -e "${GREEN}=== PSE51 stdio.h Test Suite Runner ===${NC}"
    echo "Testing mlibc stdio.h implementation compliance with POSIX.13"
    
    # Parse arguments
    case "${1:-all}" in
        build)
            build_tests
            ;;
        test)
            if [ ! -f "test_file_operations.out" ]; then
                build_tests || exit 1
            fi
            run_all_tests
            ;;
        valgrind)
            if [ ! -f "test_file_operations.out" ]; then
                build_tests || exit 1
            fi
            run_with_valgrind
            ;;
        clean)
            make clean
            rm -rf test_results/
            print_status 0 "Cleaned test artifacts"
            ;;
        report)
            run_all_tests
            generate_report
            ;;
        all|*)
            build_tests || exit 1
            run_all_tests
            generate_report
            ;;
    esac
    
    # Final summary
    if [ $TOTAL_TESTS -gt 0 ]; then
        echo -e "\n${YELLOW}=== Final Summary ===${NC}"
        echo "Total Tests: $TOTAL_TESTS"
        echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
        echo -e "Failed: ${RED}$FAILED_TESTS${NC}"
        
        if [ $FAILED_TESTS -eq 0 ]; then
            echo -e "\n${GREEN}All tests PASSED!${NC}"
            exit 0
        else
            echo -e "\n${RED}Some tests FAILED!${NC}"
            exit 1
        fi
    fi
}

# Run main function
main "$@"