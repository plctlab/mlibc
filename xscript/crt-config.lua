local project_path = os.projectdir()
add_rules("mode.debug", "mode.release")

option("crt-arch")
    set_default("arm")
    set_showmenu(true)
    set_description("Select the crt0 to build for")
    set_values("arm", "riscv32", "riscv64", "aarch64", "common")
option_end()

local crt_config = {
    ["arm"] = {
        target = "arm",
        toolchain = "arm-none-eabi",
    },
    ["riscv32"] = {
        target = "riscv",
        toolchain = "riscv32-unknown-elf"
    },
    ["riscv64"] = {
        target = "riscv",
        toolchain = "riscv64-unknown-elf"
    },
    ["aarch64"] = {
        target = "aarch64",
        toolchain = "aarch64-none-elf"
    },
    ["common"] = {
        target = "common",
        toolchain = "arm-none-eabi" -- default
    }
}

target("crt0")
    local arch = get_config("crt-arch")
    local config = crt_config[arch]
    local TARGET_DIR = "build"
    set_kind("object")
    on_load(function (target)
        target:set("toolchains", config.toolchain)
        target:add("files", path.join("crt", config.target, "crt0.c"))
        target:add("includedirs", path.join("include"))
        target:set("arch", "")
        target:set("plat", "")
        target:set("mode", "release")
    end)
target_end()