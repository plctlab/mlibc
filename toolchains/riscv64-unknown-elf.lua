PATH = "D:\\My\\plugin\\riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-w64-mingw32"
BINDIR =PATH.."\\bin"
PREFIX = "riscv64-unknown-elf-"

TOOLCHAIN_NAME = "riscv64-unknown-elf"

toolchain("riscv64-unknown-elf")

    set_kind("standalone")
    set_sdkdir(PATH)
    set_bindir(BINDIR)

toolchain_end()
