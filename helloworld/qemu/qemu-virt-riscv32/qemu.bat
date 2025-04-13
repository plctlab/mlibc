@echo off
qemu-system-riscv32 -machine virt -nographic -m 256M -kernel ../../../build/riscv32/qemu/qemu-virt-riscv32/qemu-virt-riscv32.elf ^
-device virtio-serial-device