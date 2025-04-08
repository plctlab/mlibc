@echo off
qemu-system-arm -M mps3-an536 -kernel ../../../build/arm/qemu/qemu-mps3-an536/qemu-mps3-an536.elf -serial stdio -m 512