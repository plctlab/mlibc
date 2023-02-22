PATH = "D:\\RT-ThreadStudio\\repo\\Extract\\ToolChain_Support_Packages\\WCH\\RISC-V-GCC-WCH\\8.2.0"
BIN_DIR = PATH .. "\\bin"
PREFIX = "riscv-none-embed-"

TOOLCHAIN_NAME = "riscv-none-embed"

toolchain(TOOLCHAIN_NAME)

    set_kind("standalone")
    set_sdkdir(PATH)
    set_bindir(BIN_DIR)

toolchain_end()


