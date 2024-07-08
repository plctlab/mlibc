PATH = "D:\\My\\plugin\\arm-gnu-toolchain-13.2.Rel1-mingw-w64-i686-aarch64-none-elf"
BIN_DIR = PATH .. "\\bin"

toolchain("aarch64-none-elf")
    set_kind("standalone")
    set_sdkdir(PATH)
    set_bindir(BIN_DIR)
toolchain_end()
