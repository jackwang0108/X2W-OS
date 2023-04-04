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

`X2W-OS`提供两种方式的文档：`网络文档 `以及 `本地文档`

### 网络文档

`X2W-OS`的文档由 `Sphinx`构建，目前已经托管至 `ReadTheDocs`，访问链接即可在线阅读文档：https://x2w-os.readthedocs.io/en/latest/Git

### 本地文档

如果你更喜欢在本地阅读文档，或者访问 `ReadTheDocs`存在困难，也可以自己手动构建文档。

> 以下内容为如何在本地生成文档，开发人员维护 `X2W-OS`的文档则请参考 `doc`目录中的[README](docs/README.md "README")

#### Step 1：安装Doxygen

`Doxygen`是 `C/C++`以及类 `C/C++`语言的标准文档构建系统，`X2W-OS`使用 `C`语言进行开发，因此 `X2W-OS`天然的使用 `Doxygen`作为底层文档系统

```shell
sudo apt-get install doxygen
```

#### Step 2：安装Sphinx

`Sphinx`是 `Python`的官方文档构建工具，因为其所构建的文档具有良好的显示效果，所以 `X2W-OS`使用 `Sphinx`构建最终显示的文档

```shell
python -m pip install sphinx sphinx_intl recommonmark pydata-sphinx-theme myst-parser -i https://pypi.tuna.tsinghua.edu.cn/simple
```

#### Step 3：安装Breathe

为了将 `Doxygen`生成的文档导出到 `Sphinx`中，我们还需要安装 `Breathe`

```shell
python -m pip install breathe -i https://pypi.tuna.tsinghua.edu.cn/simple
```

#### Step 4：生成文档

安装完所有依赖后，你就可以在本地构建 `X2W-OS`的文档了

```shell
cd <path-to-X2W-OS>/docs/sphinx
make html
```

编译后，在本地你就可以看到在 `<docs/sphinx>`目录下存在 `html`文件夹，文件夹内即为 `X2W-OS`的文档，`index.html`为入口

```shell
cd <path-to-X2W-OS>/docs
tree -d -L 3 sphinx
```

上述命令的输出如下：

```shell
sphinx
├── build
│   ├── doctrees
│   └── html
│       ├── _sources
│       └── _static
└── source
    ├── _static
    └── _templates

9 directories
```

#### Step5：阅读文档

你可以使用 `Python`或者 `Node.js`在本地快速启动一个 `HTTP`服务器，然后在浏览器中阅读文档

```shell

# Python3
cd <path-X2W-OS>/docs/sphinx/build/html && python -m http.server || exit 1
# Python2
cd <path-X2W-OS>/docs/sphinx/build/html && python -m SimpleHTTPServer|| exit 1
# Node.js
cd <path-X2W-OS>/docs/sphinx/build/html && http-server
```

上述命令将在本地启动一个 `HTTP`服务器，并绑定到对应的端口，访问 `localhost:<port`>即可看到文档

如果你更喜欢 `Doxygen`生成的文档，也可以使用下述命令查看 `Doxygen`生成的文档

```shell
# Python3
cd <path-X2W-OS>/docs/doxygen/build/html && python -m http.server || exit 1
# Python2
cd <path-X2W-OS>/docs/doxygen/build/html && python -m SimpleHTTPServer|| exit 1
# Node.js
cd <path-X2W-OS>/docs/doxygen/build/html && http-server
```
