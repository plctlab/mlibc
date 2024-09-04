OUTPUT_ARCH( "riscv" )

__flash =      0x80200000;
__flash_size = 0x00010000;
__ram =        0x82000000;
__ram_size   = 0x00010000;
__stack_size = 1k;

INCLUDE mlibc.ld