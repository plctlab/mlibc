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
    add_cflags("-nostdlib", "-ffreestanding", "-nostdinc", "-Wl,-Map=cc.map",{force = true})
    -- add all files
    add_files("src/*.c")
    add_files("src/crt/*.c")
    add_files("src/stdio/*.c")
    add_files("src/stdlib/*.c")
    -- add headfile dir
    add_includedirs("./include", {public = true})
    
target_end() 

target("cortex-a9")
    local CFLAGS  = "-O0 -gdwarf-2"
    local AFLAGS  = "-gdwarf-2 -mcpu=cortex-a9 -mthumb"
    local LDFALGS = "-T testcase/hello_arm_cortex_a9/link.ld -nostartfiles -nostdlib -nostdinc -lgcc"

    set_toolchains("arm-none-eabi") 
    set_filename("cortex-a9.elf")
    set_targetdir("testcase/hello_arm_cortex_a9")

    add_files("testcase/hello_arm_cortex_a9/*.c")
    add_files("testcase/hello_arm_cortex_a9/*.s")

    add_includedirs("./include")
    add_includedirs("./testcase/hello_arm_cortex_a9")

    add_linkdirs(".")
    add_links("mlibc")

    add_cflags(CFLAGS)
    add_asflags(AFLAGS)
    add_ldflags(LDFALGS , {force = true})

    after_build(function (target) 
        os.cp(target:targetfile(), './target.elf')
    end)

    on_run(function (target)
        import("core.base.option")
        local args = option.get("arguments")

        if args and args[1] == 'debug' then
            os.exec("qemu-system-arm -M vexpress-a9 -kernel %s -serial stdio -m 512 -S -s", target:targetfile())    
        else 
            os.exec("qemu-system-arm -M vexpress-a9 -kernel %s -serial stdio -m 512", target:targetfile())
        end 
    end)
target_end()

target("cortex-r52")

    local CFLAGS  = "-O0 -gdwarf-2"
    local AFLAGS  = "-gdwarf-2 -mcpu=cortex-r52 -mthumb"
    local LDFALGS = "-T testcase/hello_arm_cortex_r52/link.ld -nostartfiles -nostdlib -nostdinc -lgcc"

    set_toolchains("arm-none-eabi")
    set_filename("cortex-r52.elf")
    set_targetdir("testcase/hello_arm_cortex_r52")

    add_files("testcase/hello_arm_cortex_r52/*.c")
    add_files("testcase/hello_arm_cortex_r52/*.s")

    add_includedirs("./include")
    add_includedirs("./testcase/hello_arm_cortex_r52")

    add_linkdirs(".")
    add_links("mlibc")

    add_cflags(CFLAGS)
    add_asflags(AFLAGS)
    add_ldflags(LDFALGS , {force = true})

    after_build(function (target) 
        os.cp(target:targetfile(), './target.elf')
    end)

    on_run(function (target)
        import("core.base.option")
        local args = option.get("arguments")

        if args and args[1] == 'debug' then
            os.exec("qemu-system-arm -M mps3-an536 -kernel %s -serial stdio -m 512 -S -s", target:targetfile())    
        else 
            os.exec("qemu-system-arm -M mps3-an536 -kernel %s -serial stdio -m 512", target:targetfile())
        end 
    end)
target_end()
