# X2W-OS编译教程

`X2W-OS`使用`make`来管理项目。具体来说，`X2W-OS`的`Makefile`支持的功能有：

- **编译内核**：编译源文件并链接生成内核
- **编译文档**：编译`X2W-OS`文档
- **其他功能**：反汇编内核代码、生成内核符号表、本地阅读文档、`GDB`调试、`VSCode`调试等功能

关于`Makefile`的源代码，请参考`X2W-OS`项目根目录下的[Makefile文件](../../../../Makefile)



## 编译内核

`Makfile`中定义了`kernel`目标，该目标将会编译所有的源文件、生成目标文件，最后通过`os.ld`链接脚本指导链接器链接得到内核。

```shell
make kernel
```

此外，生成得到的内核的名称由`KNAME`变量控制，并且编译得到的目标文件和生成得到的内核将输出至`build`文件夹下。

```makefile
# 编译生成的内核的名字，将会得到 <path-to-X2W-OS>/build/os.elf
KNAME := os.elf
```



## 反汇编内核

`Makfile`中定义了`disams`目标，该目标将会对内核进行反汇编，从而得到内核的符号表、指令序列以及二进制内容。

```shell
make disasm
```

得到的反汇编文件将输出至`build/disasms`文件夹下，其中：

- 反汇编指令序列为：`build/disasms/os.disasm`
- 内核符号表为：`build/disasms/os.symbol`
- 二进制内容为：`build/disasms/os.machine`



## 编译文档

`Makfile`中定义了`documentation`目标，该目标将会编译`X2W-OS`的文档。

```shell
make documentation
```

将会得到的`Doxygen`文档和`Sphinx`文档，其中：

- `Doxygen`文档将输出至：`docs/doxygen/build`文件夹下。其中存在：
  - `html`格式文档：你可以在本地运行一个`HttpServer`来阅读文档
  - `latex`格式文档：你可以在本地使用`pdflatex`等编译器编译`latex`源码从而得到`PDF`格式的文档
  - `xml`格式文档
- `Sphinx`文档将输出值：`docs/sphinx/build/html`文件夹下，你可以在本地运行`HttpServer`来阅读文档





## 阅读文档

为了提升文档阅读体验，`Makefile`中定义了`read`目标，该目标将在本地启动`HttpServer`来阅读`Sphinx`文档。

```shell
make read
```

将会在后台运行`Python`的`http.server`，端口号每次运行时随机指定，按下`Ctrl + C`即可终止`HttpServer`

`make read`默认会阻塞终端，你也可以使用如下命令来避免终端被阻塞。

```shell
make read &
```

> 注意，后台运行`HttpServer`时，`HttpServer`的日志信息不回被重定向，会输出到终端上。需要用户手动进行重定向。





## 运行内核

`Makefile`提供了`run`目标，该目标将启动`QEMU`运行内核。`QEMU`的输出信息将输出在终端上。

```shell
make run
```





## GDB调试

`QEMU`支持通过`GDB`连接到`QEMU`中进行调试，因此`Makefile`提供了`debug-gdb`目标，以通过`GDB`连接`QEMU`进行调试。

```shell
make debug-gdb
```

此时，`GDB`程序将在前台运行，而`QEMU`将在后台运行。退出时首先按下`Ctrl+C`以终止`QEMU`，而后输入`quit`后按下回车以退出`GDB`。



## VSCode调试

`VSCode`内嵌入了`UI`形式的`GDB`。因此诸如函数调用栈、全局变量、寄存器等调试信息非常易于观察，因此`Makefile`提供了`debug-vscode`实现以通过`VSCode`连接`QEMU`进行调试

```shell
make debug-vscode
```

此时，`QEMU`将在前台运行，`QEMU`的输出信息将输出到屏幕上，并将`GDB`端口暴露到本机的`1234`端口。

按照下述步骤在`VSCode`开始调试：

- 在项目根目录中的`.vscode`文件夹下的`launch.json`文件的`configuration`项中添加如下配置：

```json
{
    "name": "Attach to QEMU",
    "type": "cppdbg",
    "request": "attach",
    "useExtendedRemote": true,
    "program": "${workspaceFolder}/build/<内核文件名，即KNAME变量的值>",
    "miDebuggerServerAddress": "<本机IP地址>::1234",
    "miDebuggerPath": "/usr/bin/gdb-multiarch",
}
```

- 在`VSCode`的侧边栏中的`运行和调试`选项卡下选择`Attach to QEMU`调试配置
- 按下`F5`启动`VSCode`中的`GDB`，即可连接到本机的`1234`端口开始调试





## 杀死进程

有的时候因为一些原因可能导致`QEMU`进程没有关闭，或者`HttpServer`没能关闭，因此`Makefile`提供了`kill`命令来杀死遗留的进程

```shell
make kill
```

将会杀死所有的`QEMU`和`HttpServer`进程





## 清除中间文件

`Makefile`提供了`clean`目标以清除编译过程中产生的所有的文件

```shell
make clean
```

将会清空`build`文件夹和`docs`文件夹中的文档。
