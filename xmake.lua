add_rules("mode.debug", "mode.release")
includes("toolchains/*.lua")

TARGET_DIR  = "."

DEVICE = " "

--set target
target("mlibc")
    -- set toolchains
    set_toolchains("arm-none-eabi")
    -- set target file type
    set_kind("static")
    --strip all infomation
    set_strip("all")
    --set default target
    set_default(true)
    -- set target dir
    set_targetdir(TARGET_DIR)
    -- set target filename
    set_filename("libmlibc.a")
    --set languages standard
    set_languages("c99", "cxx11")
    -- set compiler cflags for device
    add_cflags(DEVICE, {force = true})
    -- set compiler cflags for no standard library
    add_cflags("-nostdlib","-ffreestanding","-nostdinc", "-Wl,-Map=cc.map",{force = true})
    -- add all files
    add_files("src/*.c")
    add_files("src/crt/*.c")
    add_files("src/stdio/*.c")
    add_files("src/stdlib/*.c")
    -- add headfile dir
    add_includedirs("./include", {public = true})
    
target_end() 

target("cortex-a9")
    set_toolchains("arm-none-eabi") 

    set_filename("cortex-a9.elf")
    set_targetdir("testcase/hello_arm_cortex_a9")

    add_files("testcase/hello_arm_cortex_a9/*.c")
    add_files("testcase/hello_arm_cortex_a9/*.s")
    add_includedirs("./testcase/hello_arm_cortex_a9")

    add_linkdirs(".")
    add_links("mlibc")

    add_cflags("-g")
    add_asflags("-mcpu=cortex-a9", "-mthumb")
    add_ldflags("-T testcase/hello_arm_cortex_a9/link.ld", "-nostartfiles", {force = true})

    on_run(function (target)
        -- 运行命令
        os.exec("qemu-system-arm -M vexpress-a9 -kernel %s -serial stdio -m 512 -S -s", target:targetfile())
    end)
target_end()

target("cortex-r52")
    set_toolchains("arm-none-eabi")
    set_filename("cortex-r52.elf")
    set_targetdir("testcase/hello_arm_cortex_r52")

    add_files("testcase/hello_arm_cortex_r52/*.c")
    add_files("testcase/hello_arm_cortex_r52/*.s")
    add_includedirs("./testcase/hello_arm_cortex_r52")

    add_linkdirs(".")
    add_links("mlibc")

    add_cflags("-g")
    add_asflags("-mcpu=cortex-r52", "-mthumb")
    add_ldflags("-T testcase/hello_arm_cortex_r52/link.ld", "-nostartfiles", {force = true})

    on_run(function (target)
        -- 运行命令
        os.exec("qemu-system-arm -M mps3-an536 -kernel %s -serial stdio -m 512", target:targetfile())
    end)
target_end()
