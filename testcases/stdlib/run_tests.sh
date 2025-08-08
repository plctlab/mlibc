#!/bin/bash

# PSE51 stdlib.h test suite runner script

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test results directory
RESULTS_DIR="test_results"

# Function to print colored output
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓${NC} $2"
    else
        echo -e "${RED}✗${NC} $2"
    fi
}

# Function to build tests
build_tests() {
    echo "Building stdlib.h tests..."
    if make clean && make; then
        print_status 0 "Build completed successfully"
        return 0
    else
        print_status 1 "Build failed"
        return 1
    fi
}

# Function to run a single test
run_test() {
    local test_name=$1
    local test_exe="${RESULTS_DIR}/${test_name}"
    local log_file="${RESULTS_DIR}/${test_name}.log"
    
    if [ ! -x "$test_exe" ]; then
        echo -e "${RED}✗${NC} ${test_name}: executable not found"
        return 1
    fi
    
    # Run the test and capture output
    echo -n "Running ${test_name}... "
    if $test_exe > "$log_file" 2>&1; then
        print_status 0 "${test_name}"
        return 0
    else
        print_status 1 "${test_name} (see ${log_file} for details)"
        return 1
    fi
}

# Function to run all tests
run_all_tests() {
    local total=0
    local passed=0
    local failed=0
    
    echo -e "\n${YELLOW}Running PSE51 stdlib.h Test Suite${NC}"
    echo "=================================="
    
    # List of all tests
    tests=(
        "test_memory"
        "test_string_conversion"
        "test_random"
        "test_integer_math"
        "test_program_control"
        "test_environment_utils"
    )
    
    # Run each test
    for test in "${tests[@]}"; do
        ((total++))
        if run_test "$test"; then
            ((passed++))
        else
            ((failed++))
        fi
    done
    
    # Summary
    echo "=================================="
    echo -e "Total tests: ${total}"
    echo -e "${GREEN}Passed: ${passed}${NC}"
    if [ $failed -gt 0 ]; then
        echo -e "${RED}Failed: ${failed}${NC}"
    fi
    
    return $failed
}

# Function to run with valgrind
run_valgrind() {
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}Warning: valgrind not found, skipping memory checks${NC}"
        return 1
    fi
    
    echo -e "\n${YELLOW}Running tests with valgrind${NC}"
    echo "=================================="
    
    tests=(
        "test_memory"
        "test_string_conversion"
        "test_random"
        "test_integer_math"
        "test_program_control"
        "test_environment_utils"
    )
    
    for test in "${tests[@]}"; do
        local test_exe="${RESULTS_DIR}/${test}"
        if [ -x "$test_exe" ]; then
            echo -n "Checking ${test} for memory leaks... "
            if valgrind --quiet --leak-check=full --error-exitcode=1 "$test_exe" > /dev/null 2>&1; then
                print_status 0 "no leaks detected"
            else
                print_status 1 "memory issues found"
            fi
        fi
    done
}

# Function to generate test report
generate_report() {
    local report_file="${RESULTS_DIR}/test_report.txt"
    
    echo -e "\n${YELLOW}Generating test report${NC}"
    
    {
        echo "PSE51 stdlib.h Test Suite Report"
        echo "Generated on: $(date)"
        echo "=================================="
        echo ""
        
        # Summary of each test
        for log_file in ${RESULTS_DIR}/*.log; do
            if [ -f "$log_file" ]; then
                test_name=$(basename "$log_file" .log)
                echo "Test: $test_name"
                echo "-------------------"
                
                # Check if test passed
                if tail -n 1 "$log_file" | grep -q "All.*tests passed"; then
                    echo "Status: PASSED"
                    # Count individual test functions
                    test_count=$(grep -c "Testing" "$log_file" || true)
                    echo "Test functions: $test_count"
                else
                    echo "Status: FAILED"
                    # Show last few lines of failure
                    echo "Error output:"
                    tail -n 10 "$log_file" | sed 's/^/  /'
                fi
                echo ""
            fi
        done
        
        # Overall summary
        echo "=================================="
        echo "Overall Summary:"
        passed_count=$(ls ${RESULTS_DIR}/*.log 2>/dev/null | xargs grep -l "All.*tests passed" | wc -l)
        total_count=$(ls ${RESULTS_DIR}/*.log 2>/dev/null | wc -l)
        echo "Total test files: $total_count"
        echo "Passed: $passed_count"
        echo "Failed: $((total_count - passed_count))"
        
    } > "$report_file"
    
    echo -e "${GREEN}Report saved to: ${report_file}${NC}"
}

# Function to clean up
clean_up() {
    echo "Cleaning up..."
    make clean
    rm -f *.tmp
    print_status 0 "Cleanup completed"
}

# Main script logic
main() {
    case "${1:-all}" in
        build)
            build_tests
            ;;
        test)
            if [ -d "$RESULTS_DIR" ]; then
                run_all_tests
            else
                echo "Tests not built. Run '$0 build' first."
                exit 1
            fi
            ;;
        valgrind)
            if [ -d "$RESULTS_DIR" ]; then
                run_valgrind
            else
                echo "Tests not built. Run '$0 build' first."
                exit 1
            fi
            ;;
        report)
            if [ -d "$RESULTS_DIR" ]; then
                generate_report
            else
                echo "No test results found. Run tests first."
                exit 1
            fi
            ;;
        clean)
            clean_up
            ;;
        all|"")
            build_tests && run_all_tests && generate_report
            ;;
        help)
            echo "PSE51 stdlib.h Test Suite Runner"
            echo ""
            echo "Usage: $0 [command]"
            echo ""
            echo "Commands:"
            echo "  build     - Build all tests"
            echo "  test      - Run all tests"
            echo "  valgrind  - Run tests with valgrind (if available)"
            echo "  report    - Generate test report"
            echo "  clean     - Clean build artifacts"
            echo "  all       - Build, test, and generate report (default)"
            echo "  help      - Show this help message"
            ;;
        *)
            echo "Unknown command: $1"
            echo "Run '$0 help' for usage information."
            exit 1
            ;;
    esac
}

# Run main function
main "$@"