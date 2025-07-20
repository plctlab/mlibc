------

### **📦 使用 Docker 编译 ARM 工具链指南**

**适用于 `plctlab/mlibc` 仓库的交叉编译环境配置**

------

#### **🔧 准备工作**

1. **安装 Docker**

   ```
   # Ubuntu/Debian
   sudo apt update
   sudo apt install docker.io docker-compose
   
   # 验证安装
   docker --version
   ```

   > 注：其他系统参考 [Docker 官方文档](https://docs.docker.com/engine/install/)。

2. **解决网络问题（国内用户推荐，可选）**

   ```
   cd mlibc/toolchain  # 进入工具链配置目录
   sudo cp daemon.json /etc/docker/
   sudo systemctl restart docker
   ```

   > 此步骤使用国内镜像加速拉取基础镜像。

------

#### **🚀 编译流程**

1. **克隆仓库并进入目录**

   ```
   git clone https://github.com/plctlab/mlibc.git
   cd mlibc/toolchain 
   ```

2. **构建 Docker 镜像**

   ```
   sudo ./docker_build.sh  # 首次构建需10-30分钟
   ```

   > 若遇权限问题，尝试 `sudo chmod +x *.sh`。

3. **启动容器环境**

   ```
   ./docker_run.sh  # 自动进入容器终端
   ```

4. **在容器内编译工具链**

   ```
   # 进入工具链目录（容器内路径已映射）
   cd ~/workspace/toolchain
   make arch=arm abi=linux-eabi  # 执行编译，生成ARM工具链，如果是riscv，使用`make arch=riscv64 abi=unkwown-elf`
   ```

5. **验证编译结果**

   ```
   # 检查工具链版本（容器内执行）
   ./arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-gcc --version
   ```

   > 成功输出类似：
   >
   > arm-linux-eabi-gcc (GCC) 12.2.0
   > Copyright (C) 2022 Free Software Foundation, Inc.
   > This is free software; see the source for copying conditions.  There is NO
   > warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   >
   > build date: Jun 26 2025 16:07:39
   > musl  sha: 
   > build sha: 
   > build job: 
   > build pipeline: 

------

#### **🔍 编译与测试示例程序**

1. **编译 helloworld（容器内操作）**

   ```
   cd ~/workspace/helloworld
   ../toolchain/arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-gcc \
     main.c -o main \
     -Wl,-T./qemu/qemu-vexpress-a9/link.ld \
     -L .. -I ./qemu/qemu-vexpress-a9/ -mthumb
   ```

------

#### **⚠️ 常见问题解决**

- **权限问题**：
   容器内操作无需 `sudo`，若宿主机命令报权限错误，尝试：

  ```
   sudo usermod -aG docker $USER  # 将用户加入docker组
   newgrp docker  # 刷新组权限
  ```

------

#### **📂 目录结构说明**

```
mlibc/
├── toolchain/             # 工具链源码和编译脚本
│   ├── docker/            # Docker配置
│   │   ├── docker_build.sh
│   │   ├── docker_run.sh
│   │   └── daemon.json
│   └── arm-linux-eabi_for_x86_64-pc-linux-gnu/  # 生成的工具链
└── helloworld/            # 测试程序
    ├── main.c
    └── qemu/
        └── qemu-vexpress-a9/
            └── link.ld    # ARM内存布局脚本
```

> 通过 Docker 环境编译可确保依赖一致性，避免宿主机环境差异导致的问题。完成验证后退出容器：`exit`。
