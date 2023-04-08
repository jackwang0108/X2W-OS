# X2W-OS

2023全国大学生计算机系统能力大赛-操作系统赛-内核实现赛道西安交通大学X2W参赛队参赛作品

## 队伍信息

队员：

- 王仕鸿
- 王少非
- 肖追日

指导老师：

- 惠维

## 安装及运行

Coming soon....

## 文档

`X2W-OS`提供两种方式的文档：`网络文档`以及 `本地文档`

### 网络文档

`X2W-OS`的文档由 `Sphinx`构建，目前已经托管至 `ReadTheDocs`，访问链接即可在线阅读文档：https://x2w-os.readthedocs.io/en/latest/

### 本地文档

如果你更喜欢在本地阅读文档，或者访问 `ReadTheDocs`存在困难，也可以自己手动构建文档。

> 以下内容为如何在本地生成文档，开发人员维护 `X2W-OS`的文档则请参考 `doc`目录中的[README](docs/README.md "README")

#### Step 1：安装依赖

`X2W-OS`的文档系统依赖 `Doxygen`、`Sphinx`、`Breathe`等工具/库，因此首先安装这些依赖

```shell
sudo apt-get install doxygen
python -m pip install sphinx sphinx_intl recommonmark pydata-sphinx-theme myst-parser -i https://pypi.tuna.tsinghua.edu.cn/simple
```

#### Step 2：编译文档

编译文档的相关命令已经集成进 `Makefile`，运行下述命令即可编译 `X2W-OS`的文档

```shell
make documentation
```

> 注意，编译过程中产生的下述警告是正常现象，忽略即可
>
> ```shell
> Declaration is '.. cpp:function:: char * strcpy (char *dst, const char *src)'.
> /Users/jack/project/OSKernel2023-X2W/docs/sphinx/source/api/functions.rst:12: WARNING: Duplicate C++ declaration, also defined at api/files/include/string:3.
> ```

#### Step 3：阅读文档

阅读文档的相关命令已经集成进 `Makefile`，运行下述命令即可阅读 `X2W-OS`的文档

```shell
make read
```

而后访问链接即可开始阅读文档

```shell
正在启动 HttpServer ...
本地文档链接: http://127.0.0.1:45308
在线文档链接: https://x2w-os.readthedocs.io/en/latest/
HttpServer日志将会打印在下方, 按下 Ctrl+C 或运行 `make kill` 以终止本地 HttpServer
-------------------------------------------------------------------------------
```

> 注意，`HttpServer`在本地暴露的端口在每次运行 `make read`命令时会随机指定
