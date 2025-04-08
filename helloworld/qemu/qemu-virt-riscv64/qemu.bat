@echo off
qemu-system-riscv64 -machine virt -nographic -m 256M -kernel ../../../build/riscv64/qemu/qemu-virt-riscv64/qemu-virt-riscv64.elf ^
-device virtio-serial-device