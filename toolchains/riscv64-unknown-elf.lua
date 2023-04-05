PATH = "/home/linshire/toolchains/riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-linux-ubuntu14"
BINDIR =PATH.."/bin"
PREFIX = "riscv64-unknown-elf-"

TOOLCHAIN_NAME = "riscv64-unknown-elf"

toolchain("riscv64-unknown-elf")

    set_kind("standalone")
    set_sdkdir(PATH)
    set_bindir(BINDIR)

toolchain_end()
