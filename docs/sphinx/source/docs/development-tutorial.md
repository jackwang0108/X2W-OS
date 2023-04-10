# X2W-OS Installation

本文介绍如何安装 `X2W-OS`的开发环境，从而开始开发 `X2W-OS`。

`X2W-OS`在开发过程中所使用的工具有：

- 针对 `RISC-V`的 `GNU-Toolchain`：
  - `gcc`编译器
  - `as`汇编器
  - `ld`链接器
  - `GDB`调试器
  - `objdump`目标文件信息查看器
  - ……
- `QEMU`模拟器
- `MAKE`构建工具
- `Sphinx`和 `Doxygen`文档生成工具

> 教程有诸多不完善之处，敬请谅解

> 目前教程仅包括从源码安装这一方式，从包管理器安装预编译的工具链的教程后续会添加

## GNU-Toolchain

`GNU-Toolchain`是一个包含了 `GCC`编译器、`AS`汇编器、`ld`链接器等诸多工具在内的工具集合。

其主要功能为：

- 将源代码编译称为目标文件、可执行文件等二进制文件（`GCC`、`AS`、`ld`……）
- 对二进制文件进行处理的工具（`objdump`、`objcopy`、`nm`……）
- 调试器（`GDB`）
- ……

可以说，`GNU-Toolchain`是进行诸如嵌入式开发、系统开发等地底层开发所必须的工具链。

### 下载GNU-Toolchain

`GNU-Toolchain`可以分为三部分：

- 编译工具：`GCC`、`AS`、`LD`
- 二进制文件处理工具：`objcopy`、`objdump`……
- 调试器：`GDB`

因此在安装的时候需要分别下载这三大类工具的源代码，进行编译。

幸运的是 `RISC-V`基金会将所有工具的 `Git`仓库以子模块的形式集合到了一个 `Git`仓库中。因此利用 `Git `即可下载所有的工具的源代码。

首先下载提供了一键化安装 `Makefile`的 `RISCV-GNU-Toolchain`：

```shell
git clone https://github.com/riscv-collab/riscv-gnu-toolchain.git
```

然后下载所有工具源代码。由于 `QEMU`的 `Git`仓库的子模块非常多，因此稍后直接下载压缩包，这里不使用 `Git`下载 `QEMU`

```shell
cd <path-to-riscv-gnu-toolchain>
git submodule init
git -c submodule.qemu.update=none submodule update --recursive
```

其中：

- `--recursive`选项会对所有的子模块进行下载

> 如果因为网络问题导致 `git clone`失败，则可以考虑使用如下的命令为命令行开启代理。
>
> ```shell
> export http_proxy="http://127.0.0.1:<port>"
> export https_proxy="https://127.0.0.1:<port>"
> alias set_proxy='export http_proxy="http://127.0.0.1:<port>"; export https_proxy="https://127.0.0.1:<port>"'
> alias unset_proxy='unset -v http_proxy; unset -v https_proxy'
> ```
>
> 注意，将 `<port>`更换为你所使用的工具在本机的端口号

### 安装GNU-Toolchain依赖

编译 `GNU-Toolchain`会依赖一些工具，在不同的系统上使用对应的的命令安装这些依赖

- `Ubuntu`/`Debian`：

```shell
sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build
```

- `Fedora`/`CentOS`/`RHEL OS`：

```shell
sudo yum install autoconf automake python3 libmpc-devel mpfr-devel gmp-devel gawk  bison flex texinfo patchutils gcc gcc-c++ zlib-devel expat-devel
```

- `Arch Linux`：

```shell
sudo pacman -Syyu autoconf automake curl python3 libmpc mpfr gmp gawk base-devel bison flex texinfo gperf libtool patchutils bc zlib expat
```

- `MacOS`：

```shell
brew install python3 gawk gnu-sed gmp mpfr libmpc isl zlib expat texinfo flock
```

### 编译GNU-Toolchain

使用如下命令编译针对 `RISC-V`的 `GNU-Toolchain`

```shell
export PREFIX="~/opt/riscv-tools"
mkdir -p $PREFIX
cd <path-to-riscv-gnu-toolchain>
./configure --prefix=$PREFIX --enable-gdb  --enable-gcc-checkin
make -j
```

你也可以修改变量 `PREFIX`的值来更改 `RISCV-GNU-Toolchain`的安装位置

编译过程中会自动安装编译得到的工具到指定的路径，编译完成后，在安装位置中可以看到所有的工具：

```shell
tree -d -L 2 $PREFIX
```

```shell
/Users/jack/opt/riscv-tools/
├── bin
├── include
├── lib
│   ├── bfd-plugins
│   └── gcc
├── libexec
│   └── gcc
├── riscv64-unknown-elf
│   ├── bin
│   ├── include
│   └── lib
└── share
    ├── gcc-12.2.0
    ├── info
    ├── locale
    └── man


16 directories
```

其中，`bin`文件夹下存放所有的工具：

```shell
ls -l $PREFIX/bin
```

```shell
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-addr2line
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-ar
.rwxr-xr-x@ 1.6M jack  7 Apr 19:17 riscv64-unknown-elf-as
.rwxr-xr-x@ 1.4M jack  7 Apr 19:21 riscv64-unknown-elf-c++
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-c++filt
.rwxr-xr-x@ 1.4M jack  7 Apr 19:21 riscv64-unknown-elf-cpp
.rwxr-xr-x@ 108k jack  7 Apr 19:17 riscv64-unknown-elf-elfedit
.rwxr-xr-x@ 1.4M jack  7 Apr 19:21 riscv64-unknown-elf-g++
.rwxr-xr-x@ 1.4M jack  7 Apr 19:21 riscv64-unknown-elf-gcc
.rwxr-xr-x@ 1.4M jack  7 Apr 19:21 riscv64-unknown-elf-gcc-12.2.0
.rwxr-xr-x@  64k jack  7 Apr 19:21 riscv64-unknown-elf-gcc-ar
.rwxr-xr-x@  65k jack  7 Apr 19:21 riscv64-unknown-elf-gcc-nm
.rwxr-xr-x@  65k jack  7 Apr 19:21 riscv64-unknown-elf-gcc-ranlib
.rwxr-xr-x@ 778k jack  7 Apr 19:21 riscv64-unknown-elf-gcov
.rwxr-xr-x@ 644k jack  7 Apr 19:21 riscv64-unknown-elf-gcov-dump
.rwxr-xr-x@ 672k jack  7 Apr 19:21 riscv64-unknown-elf-gcov-tool
.rwxr-xr-x@ 1.2M jack  7 Apr 19:17 riscv64-unknown-elf-gprof
.rwxr-xr-x@ 2.0M jack  7 Apr 19:17 riscv64-unknown-elf-ld
.rwxr-xr-x@ 2.0M jack  7 Apr 19:17 riscv64-unknown-elf-ld.bfd
.rwxr-xr-x@  23M jack  7 Apr 19:21 riscv64-unknown-elf-lto-dump
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-nm
.rwxr-xr-x@ 1.2M jack  7 Apr 19:17 riscv64-unknown-elf-objcopy
.rwxr-xr-x@ 1.9M jack  7 Apr 19:17 riscv64-unknown-elf-objdump
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-ranlib
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-readelf
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-size
.rwxr-xr-x@ 1.1M jack  7 Apr 19:17 riscv64-unknown-elf-strings
.rwxr-xr-x@ 1.2M jack  7 Apr 19:17 riscv64-unknown-elf-strip
```

### 增加Tab补全

最后，将 `bin`文件夹添加到 `PATH`环境变量中，以实现 `Tab`补全支持。

- `bash`：

```shell
printf 'PATH=$PATH:%s' $PREFIX/bin >> ~/.bashrc
```

- `zsh`

```shell
printf 'PATH=$PATH:%s' $PREFIX/bin >> ~/.zshrc
```

重新加载 `Shell`配置文件即可实现 `Tab`补全

- `bash`：

```shell
source ~/.bashrc
```

- `zsh`：

```shell
source ~/.zshrc
```

## QEMU

`QEMU`用于模拟一个 `RISC-V`开发板，以测试、运行 `X2W-OS`。

> 截止目前为止，`QEMU`在 `Mac`上的编译并没有通过，但是通过 `brew`却可以安装 `7.1.0`版本的 `QEMU`
>
> 因此下文只介绍如何在 `Ubuntu`等平台上安装 `QEMU`

### 下载QEMU

`Git`克隆 `QEMU`的 `Git`仓库比较慢，所以直接使用 `wget`下载 `QEMU`的压缩包代码。

```shell
wget https://download.qemu.org/qemu-7.2.0.tar.xz
tar xvJf qemu-7.2.0.tar.xz
```

> 如果因为网络问题导致下载失败，请参考上面的建议

### 安装QEMU依赖

编译 `QEMU`时会依赖一些程序，因此先安装这些依赖

```shell
sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev git libglib2.0-dev libfdt-dev libpixman-1-dev libncurses5-dev libncursesw5-dev
```

### 编译QEMU

使用下述命令编译 `QEMU`

```shell
cd qemu-7.2.0
export PREFIX='~/opt/qemu'
./configure --prefix=$PREFIX --target-list=riscv64-softmmu,riscv64-linux-user
make -j
```

你也可以修改变量 `PREFIX`的值来更改 `QEMU`的安装位置

### 安装QEMU

编译完成后，将得到的 `QEMU`程序安装到指定位置

```shell
make install
```

而后就能看到 `QEMU`的工具：

```shell
tree -d -L 2 $PREIFX
```

```shell
/home/jack/opt/qemu
├── bin
├── bin-back
├── include
├── libexec
├── log
├── share
│   ├── applications
│   ├── doc
│   ├── icons
│   ├── man
│   └── qemu
└── var
    └── run

13 directories
```

其中，`bin`文件夹下存放所有的工具：

```shell
ls -l $PREFIX/bin
```

```shell
-rwxr-xr-x 1 jack jack  2644160 Apr  8 00:33 qemu-edid
-rwxr-xr-x 1 jack jack  3754864 Apr  8 00:33 qemu-ga
-rwxr-xr-x 1 jack jack 10605752 Apr  8 00:33 qemu-img
-rwxr-xr-x 1 jack jack 10402296 Apr  8 00:33 qemu-io
-rwxr-xr-x 1 jack jack 11433864 Apr  8 00:33 qemu-nbd
-rwxr-xr-x 1 jack jack  4050608 Apr  8 00:33 qemu-pr-helper
-rwxr-xr-x 1 jack jack 16419640 Apr  8 00:33 qemu-riscv64
-rwxr-xr-x 1 jack jack 13466608 Apr  8 00:33 qemu-storage-daemon
-rwxr-xr-x 1 jack jack 57615088 Apr  8 00:33 qemu-system-riscv64
```

### 增加Tab补全

最后，将 `bin`文件夹添加到 `PATH`环境变量中，以实现 `Tab`补全支持。

- `bash`：

```shell
printf 'PATH=$PATH:%s' $PREFIX/bin >> ~/.bashrc
```

- `zsh`

```shell
printf 'PATH=$PATH:%s' $PREFIX/bin >> ~/.zshrc
```

重新加载 `Shell`配置文件即可实现 `Tab`补全

- `bash`：

```shell
source ~/.bashrc
```

- `zsh`：

```shell
source ~/.zshrc
```

## GDB

针对不同的平台，`GDB`有不同的版本。

### Debian/Ubuntu

使用 `apt`直接安装

```shell
sudo apt-get install gdb-multiarch
```

而后输入下述即命令可使用针对 `RISC-V`的 `GDB`

```shell
gdb-multiarch
```

### MacOS

使用 `brew`安装

```shell
brew tap riscv/riscv
brew install riscv-tools
```

而后输入下述即命令可使用针对 `RISC-V`的 `GDB`

```shell
riscv64-unknown-elf-gdb
```
