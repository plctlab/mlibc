local project_path = os.projectdir()

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
    local TARGET_DIR = "build/.objs/crt0/release"
    set_kind("object")
    on_load(function (target)
        target:set("arch", "")
        target:set("plat", "")
        target:set("mode", "release")
        target:set("toolchains", config.toolchain)
        target:add("files", path.join("crt", config.target, "crt0.c"))
        target:add("includedirs", path.join("include"))
    end)
    
    after_build(function (target)
        os.mkdir("mlibc/lib")
        os.cp(path.join(project_path, TARGET_DIR, "crt", config.target, "crt0.c.o"), "mlibc/lib/crt0.o")
        if os.isfile(path.join(project_path, "crt", config.target, "crti.s")) then
            os.cp(path.join(project_path, "crt", config.target, "crti.s"), "mlibc/lib/crti.s")
        end
        if os.isfile(path.join(project_path, "crt", config.target, "crtn.s")) then
            os.cp(path.join(project_path, "crt", config.target, "crtn.s"), "mlibc/lib/crtn.s")
        end
    end)
target_end()