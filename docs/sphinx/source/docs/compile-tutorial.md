# X2W-OS Make Targets

`X2W-OS`使用 `make`来管理项目。具体来说，`X2W-OS`的 `Makefile`中所有的目标可以分为三类：

- **编译相关目标**：编译内核、编译 `SBI`、编译文档
- **运行调试相关目标**：使用 `QEMU`运行 `X2W-OS`、使用 `GDB`、`VSCode`调试 `X2W-OS`
- **Miscellaneous目标**：具有多种用途的目标，例如杀死所有 `QEMU`进程、启动 `HttpServer`阅读文档……

下面将分别介绍 `X2W-OS`中这三类目标。

> 关于 `Makefile`的源代码，请参考 `X2W-OS`项目根目录下的[Makefile文件](../../../../Makefile)

## 编译相关目标

`X2W-OS`的 `Makefile`中分别提供了编译源文件、内核、`SBI`、文档的目标

### 编译源文件：`obj`

`Makfile`中定义了 `obj`目标，该目标将会编译内核和 `SBI`的源文件、生成目标文件

```shell
make obj
```

编译得到的目标文件和生成得到的内核将输出至 `build`文件夹下

注意：

- 该目标通常用于检查内核和 `SBI`的目标文件

```shell
$ make obj
mkdir -p /Users/jack/project/OSKernel2023-X2W/build/disasms
mkdir -p /Users/jack/project/OSKernel2023-X2W/build/temps
mkdir -p  /Users/jack/project/OSKernel2023-X2W/build/lib  /Users/jack/project/OSKernel2023-X2W/build/lib/kernel  /Users/jack/project/OSKernel2023-X2W/build/lib/user  /Users/jack/project/OSKernel2023-X2W/build/kernel  /Users/jack/project/OSKernel2023-X2W/build/device
mkdir -p  /Users/jack/project/OSKernel2023-X2W/build/sbi
/* -------------------------- 编译内核和SBI -------------------------- */
当前内核目标文件:
	build/kernel/boot.o
	build/lib/stdfmt.o
	build/lib/stdlib.o
	build/lib/string.o
	build/lib/kernel/kstdio.o
	build/lib/user/ustdio.o
	build/kernel/init.o
	build/kernel/main.o
	build/kernel/test.o
	build/device/uart.o
-----------------------------------------------------------------------
当前SBI目标文件:
	build/sbi/sbi_boot.o
	build/sbi/sbi_main.o
-----------------------------------------------------------------------
```

### 编译内核：`kernel`

`Makfile`中定义了 `kernel`目标，该目标将会编译内核源文件、生成目标文件，最后通过 `kernel/kernel.ld`链接脚本指导链接器链接得到内核

```shell
make kernel
```

编译得到的目标文件和生成得到的内核将输出至 `build`文件夹下

注意：

- 在 `build`文件夹下默认生成的 `ELF`格式内核，文件名为 `os.elf`
- 为了方便后续的反汇编，`kernel`目标还会生成内核的裸二进制文件 `os.bin`
- **为了方便提交测试，`kernel`目标会将 `os.elf`复制到根目录下，并将其重命名为 `${KNAME}`**

`KNAME`变量定义在 `Makefile`中，修改该变量的值即可控制最终在根目录下的内核文件名

```makefile
# 编译生成的内核的名字，将会得到 <path-to-X2W-OS>/kernel.elf
KNAME := kernel.elf
```

### 编译SBI：`sbi`

`Makfile`中定义了 `sbi`目标，该目标将会编译 `SBI`源文件、生成目标文件，最后通过 `sbi/sbi.ld`链接脚本指导链接器链接得到 `SBI`

```shell
make sbi
```

编译得到的目标文件和生成得到的 `SBI`将输出至 `build`文件夹下

注意：

- 在 `build`文件夹下默认生成的 `ELF`格式 `SBI`，文件名为 `sbi.elf`
- 为了方便后续的反汇编，`sbi`目标还会生成 `SBI`的裸二进制文件 `os.bin`
- 为了方便提交测试，`kernel`目标会将 `sbi.elf`复制到根目录下，并将其重命名为 `${SNAME}`

`SNAME`变量定义在 `Makefile`中，修改该变量的值即可控制最终在根目录下的 `SBI`文件名

```makefile
# 编译生成的内核的名字，将会得到 <path-to-X2W-OS>/sbi.elf
SNAME := sbi.elf
```

### 编译文档：`documentation`

`Makfile`中定义了 `documentation`目标，该目标将会通过 `Doxygen`和 `Sphinx`编译文档系统

```shell
make documentation
```

编译得到的 `Doxygen`文档和 `Sphinx`文档将分别输出至：

- `<path-to-X2W-OS>/docs/doxygen/build`
- `<path-to-X2W-OS>/docs/sphinx/build`

注意：

- `Sphinx`将会输出 `html`格式的文档
- `Doxygen`将会输出 `html`、`latex`和 `xml`三种格式的文档，你可以修改 `<path-to-X2W-OS>/docs/doxygen/Doxyfile`来改变这一行为
- 你可以使用 `pdflatex`等其它工具生成诸如 `PDF`等其他格式的文档

> 关于如何在本地阅读 `HTML`格式的文档，请参考

### 编译内核和SBI：`all`

`Makfile`中定义了 `all`目标，该目标将会：

- 编译内核
- 编译 `SBI`

```shell
make all
```

### 编译内核、SBI和文档：`full`

`Makfile`中定义了 `full`目标，该目标将会：

- 编译内核
- 编译 `SBI`
- 编译文档

```shell
make full
```

## 运行和调试相关目标

`X2W-OS`的 `Makefile`中提供了直接运行内核、使用 `GDB`调试内核、使用 `VSCode`调试内核的目标

### 直接运行内核：`run`

`Makefile`提供了 `run`目标，该目标将启动 `QEMU`运行内核。`QEMU`的输出信息将输出在终端上。

```shell
make run
```

> 警告：
>
> - 当前仅在 `Ubuntu`上进行了测试，其他类 `Unix/Linux`平台没有进行测试
> - 无法在 `MacOS`上运行，因为：
>   - 通过 `brew`安装的 `qemu-system-riscv64`没有通过所有测试，无法将诸如 `mret`等指令翻译为本地指令（`JIT`）
>   - 通过 `port`安装依赖库后从源码编译 `QEMU-7.2.0`时会因为导入头文件问题报错

### GDB调试：`debug-gdb`

`QEMU`支持通过 `GDB`连接到 `QEMU`中进行调试，因此 `Makefile`提供了 `debug-gdb`目标，以通过 `GDB`连接 `QEMU`进行调试。

```shell
make debug-gdb
```

运行该命令后，`GDB`将在前台运行，而 `QEMU`将在后台运行。退出时首先按下 `Ctrl+C`以终止 `QEMU`，而后输入 `quit`后按下回车以退出 `GDB`。

> 注意：
>
> - `Makefile`将搜索 `gdb-multiarch`或者 `riscv64-unknown-elf-gdb`，你可以通过包管理安装或者手动编译安装
> - 如果 `Makefile`无法找到两个 `GDB`中的任意一个，那么就会报错，而后终止运行

### VSCode调试：`debug-vscode`

`VSCode`内嵌入了 `UI`形式的 `GDB`。因此诸如函数调用栈、全局变量、寄存器等调试信息非常易于观察，因此 `Makefile`提供了 `debug-vscode`实现以通过 `VSCode`连接 `QEMU`进行调试

```shell
make debug-vscode
```

此时，`QEMU`将在前台运行，`QEMU`的输出信息将输出到屏幕上，并将 `GDB`端口暴露到本机的 `1234`端口。

接下来，按照下述步骤配置 `VSCode`，以能够在 `VSCode`中调试：

- 在项目根目录中的 `.vscode`文件夹下的 `launch.json`文件的 `configuration`项中添加如下配置：

  ```shell
  {
      "name": "GDB Debug",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/os.elf",
      "externalConsole": false,
      "stopAtConnect": true,
      "MIMode": "gdb",
      "miDebuggerArgs": "<本机gdb-multiarch或者riscv64-unknown-elf-gcc的绝对路径",
      "miDebuggerServerAddress": "<本机IP地址>:1234",
      "miDebuggerPath": "<本机gdb-multiarch或者riscv64-unknown-elf-gcc的绝对路径>",
      "cwd": "${workspaceFolder}",
  }
  ```

  > 注意：
  >
  > - `GDB`的绝对路径可以通过`which`命令获得
  > - `<本机IP地址>`可以通过运行`ifconfig`命令查看
  > - `<本机IP地址>`最好写成本机在局域网下的`IP地址`，不要写成`本地回环地址`。例如`192.168.0.16`，而非`127.0.0.1`：
  >   - 因为`GDB Server`可能运行在同一局域网下另外的机器，而`VSCode`运行在当前机器上
  >   - 若`<本机IP地址>`的值设定为`127.0.0.1`，则`VSCode`会尝试访问当前机器上的`GDB Server`（事实上并没有运行），而非同一局域网下的另外的机器
  >   - 此时就无法成功运行

- 在 `VSCode`的侧边栏中的 `运行和调试`选项卡下选择 `GDB Debug`调试配置
- 按下 `F5`启动 `VSCode`中的 `GDB`，即可连接到本机的 `1234`端口开始调试

## Miscellaneous目标

`X2W-OS`的 `Makefile`中提供了其他诸多目标以帮助更好的进行开发

### 反汇编：`disasm`

`Makfile`中定义了 `disams`目标，该目标将会对 `ELF`格式的内核和 `SBI`进行反汇编，从而得到符号表、指令序列以及裸二进制内容。

```shell
make disasm
```

得到的反汇编内容将输出至 `build/disasms`文件夹下，其中：

- 反汇编指令序列为：`build/disasms/{os,sbi}.disasm`
- 内核符号表为：`build/disasms/{os,sbi}.symbol`
- 二进制内容为：`build/disasms/{os,sbi}.machine`

### 阅读文档：`read`

为了提升文档阅读体验，`Makefile`中定义了 `read`目标，该目标将在本地启动 `HttpServer`来阅读 `Sphinx`文档。

```shell
make read
```

将会在后台运行 `HttpServer`，端口号每次运行时随机指定，按下 `Ctrl + C`即可终止 `HttpServer`

> 注意：
>
> - 目前 `Makefile`仅支持运行 `Python`的 `HttpServer`，如果使用诸如 `node.js`的 `HttpServer`，请自行添加
> - `Makefile`对Python版本差异进行了屏蔽：
>   - `Python2`中的 `HttpServer`由 `SimpleHTTPServer`提供
>   - `Python3`中的 `HttpServer`由 `http.server`提供

q此外，`make read`默认会阻塞终端，你也可以使用如下命令来避免终端被阻塞。

```shell
make read &
```

> 注意，`Python`，`HttpServer`的日志信息不会被重定向，因此会依旧输出到终端上，需要用户手动进行重定向。

### 杀死遗留QEMU、HttpServer进程：`kill`

很多时候可能会导致存在遗留的进程：

- 在使用 `GDB`调试的时候，可能因为错误的退出方式，导致 `QEMU`进程没有关闭
- 与此类似，可能有时候把 `HttpServer`进程挂到后台运行忘记关闭
- ……

因此 `Makefile`提供了 `kill`命令来杀死遗留的 `QEMU`、`HttpServer`和 `GDB`进程

```shell
make kill
```

### 清除中间文件：`clean`

`Makefile`提供了 `clean`目标以清除编译过程中产生的所有的文件

```shell
make clean
```

将会清空 `build`文件夹和 `docs`文件夹中的文档。
