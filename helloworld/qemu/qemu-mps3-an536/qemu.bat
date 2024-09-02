@echo off
qemu-system-arm -M mps3-an536 -kernel qemu-mps3-an536.elf -serial stdio -m 512