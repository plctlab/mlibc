@echo off
qemu-system-arm -M vexpress-a9 -kernel qemu-vexpress-a9.elf -serial stdio -m 512