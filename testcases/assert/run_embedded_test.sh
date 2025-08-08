#!/bin/bash
# Script to build and run embedded assert tests

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
echo "=== Building assert test ==="

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
echo "=== Running test with assertions enabled ==="
echo "Press Ctrl-A X to exit QEMU"
echo ""

# Run test
make -f Makefile.embedded run

echo ""
echo "=== Running test with NDEBUG (assertions disabled) ==="
echo "Press Ctrl-A X to exit QEMU"
echo ""

# Run NDEBUG version
make -f Makefile.embedded run-ndebug