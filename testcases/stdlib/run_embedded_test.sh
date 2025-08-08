#!/bin/bash
# Script to build and run embedded stdlib tests

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=== Building mlibc for RISC-V 32-bit ==="

# Build mlibc if not already built
if [ ! -f "../../../build/riscv32/libmlibc.a" ]; then
    echo -e "${YELLOW}Building mlibc library...${NC}"
    (cd ../../.. && make ARCH=riscv32)
    if [ $? -ne 0 ]; then
        echo -e "${RED}Failed to build mlibc!${NC}"
        exit 1
    fi
else
    echo -e "${GREEN}mlibc library already built${NC}"
fi

# Check for CRT files
if [ ! -f "../../../build/riscv32/crtobj/crt0.o" ]; then
    echo -e "${YELLOW}Building CRT files...${NC}"
    (cd ../../.. && make crt ARCH=riscv32)
    if [ $? -ne 0 ]; then
        echo -e "${RED}Failed to build CRT files!${NC}"
        exit 1
    fi
else
    echo -e "${GREEN}CRT files already built${NC}"
fi

echo ""
echo "=== Building stdlib test ==="

# Build test
make -f Makefile.embedded clean
make -f Makefile.embedded

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}Build successful!${NC}"
echo ""
echo "=== Running stdlib tests ==="
echo "This test suite includes:"
echo "  - Memory management (malloc, free, calloc, realloc)"
echo "  - Integer math (abs, labs, div, ldiv)"
echo "  - String conversions (atoi, strtol, etc.)"
echo "  - Random numbers (rand, srand)"
echo "  - Sorting and searching (qsort, bsearch)"
echo "  - Environment and system functions"
echo ""
echo "Press Ctrl-A X to exit QEMU"
echo ""

# Run test
make -f Makefile.embedded run

# Check if user wants to run with more memory
echo ""
echo -n "Run tests with more memory (debug mode)? [y/N]: "
read -r response
if [[ "$response" =~ ^[Yy]$ ]]; then
    echo ""
    echo "=== Running with debug options (512MB RAM) ==="
    make -f Makefile.embedded run-debug
fi