PATH = "D:\\My\\plugin\\riscv32-gnu-toolchain-win"
BINDIR =PATH.."\\bin"
PREFIX = "riscv32-unknown-elf-"

TOOLCHAIN_NAME = "riscv32-unknown-elf"

toolchain("riscv32-unknown-elf")

    set_kind("standalone")
    set_sdkdir(PATH)
    set_bindir(BINDIR)

toolchain_end()
