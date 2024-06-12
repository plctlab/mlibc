@echo off
qemu-system-arm --version
qemu-system-arm -M mps3-an536 -kernel cortex-r52.elf -serial stdio -m 512
