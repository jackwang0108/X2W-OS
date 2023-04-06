# ----------------------------------------------------------
# 命令行参数
# ----------------------------------------------------------

# riscv-gcc 交叉编译器参数
#		1. -notstdlib: 链接所有可重定位文件时不链接 LD_LIBRARY_PATH 包含的库, 只链接用户指定的库
#		2. -fno_builtin: 不将与库函数同名的函数视为库函数, 即使用自己编写的库函数
#		3. -march: 设置生成(指令所构成的)二进制文件所使用的指令集(指令所属于的指令集)
#		4. -mabi: 设置C和汇编函数相互调用时候的参数传递(寄存器使用)约定
#		5. -O0: 设置gcc编译时的优化等级, 0表示禁止优化
#		6. -g: 编译时向生成的二进制文件中写入debug信息, 同时禁止gcc在编译时为了加速对一些诸如abs，strncpy等进行重定义的行为
#		7. -I: 编译时候寻找头文件的路径
# 	-W 表示和警告相关的设置
#		8. -Wall: 编译的时候输出所有警告
#		9. -Wmissing-prototypes: 对缺少函数原型的函数(非内联, 静态函数)进行警告
#		10. -Werror=strict-prototypes: 调用函数的时候严格检测参数类型和函数原型中的类型, 如果不符则报错
#		11. -Werror=incompatible-pointer-types: 禁止对指针类型进行默认转换, 必须显示进行类型转换, 否则报错
CFLAGS = \
	-nostdlib \
	-fno-builtin \
	-march=rv32ima \
	-mabi=ilp32 \
	-g \
	-O0 \
	-Wall \
	-Wmissing-prototypes \
	-Werror=strict-prototypes \
	-Werror=incompatible-pointer-types \
	-I include/


# QEMU 参数
#		1. -nographic: 不显示图形界面
#		2. -smp: 设置 CPU 数
#		3. -machine: 设置模拟的系统类型. QEMU 有 User 和 System 两种模拟方式.
#					 User 模拟模式会模拟对应的CPU(例如 qemu-riscv64 会模拟64位的 RISC-V CPU), 而后将会使用Linux进程来运行交叉编译的程序, 并且会在运行时对系统调用进行翻译
#				     Full System 模拟模式将会模拟一个完整的系统, 即虚拟机. 模拟的完整系统中包括: CPU 和其他外围设备(硬盘, 网卡)
#					 在运行 Full System 模拟的时候, 你可以使用预设的一些系统, 这些系统中已经包含了指定的设备.
#					 例如当你运行 qemu-system-aarch64 的时候, 你可以指定 -machine 参数为 raspi3b 来运行一个具有树莓派3B所有设备的虚拟机, 即模拟一个树莓派3B
#					 这里指定 -machine 参数为 virt 即模拟一个具有常用设备的 RISC-V 内核的CPU
#		4. -bios: set your bios program. None means using QEMU bios.
# 比赛运行 QEMU 的参数: 
#		0. 本次大赛的区域赛阶段评测使用QEMU虚拟环境, 提交的项目根目录中必须包含一个Makefile文件, 评测时会自动在您的项目中执行make all命令.
#		   您应该在Makefile中的all目标对操作系统进行编译, 并生成ELF格式的sbi-qemu和kernel-qemu两个文件, 即与xv6-k210运行qemu时的方式一致.
#		1. 如果你的系统使用默认SBI, 则不需要生成sbi-qemu文件, 运行QEMU时会自动设置-bios参数为default
#		2. 同时QEMU启动时还会使用-drive file参数挂载SD卡镜像, SD卡镜像为FAT32文件系统, 没有分区表. 
#		   在SD卡镜像的根目录里包含若干个预先编译好的ELF可执行文件（以下简称测试点）, 您的操作系统在启动后需要主动扫描SD卡, 并依次运行其中每一个测试点, 将其运行结果输出到串口上, 评测系统会根据您操作系统的串口输出内容进行评分.
#		   您可以根据操作系统的完成度自由选择跳过其中若干个测试点, 未被运行的测试点将不计分. 测试点的执行顺序与评分无关, 多个测试点只能串行运行, 不可同时运行多个测试点.
#		   具体测试点的数量, 内容以及编译方式将在赛题公布时同步发布.
#		3. 当您的操作系统执行完所有测试点后, 应该主动调用关机命令, 评测机会在检测到QEMU进程退出后进行打分.
# qemu-system-riscv64 -machine virt -kernel kernel-qemu -m 128M -nographic -smp 2 -bios sbi-qemu -drive file=sdcard.img,if=none,format=raw,id=x0  -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 -initrd initrd.img
QFLAGS = -nographic \
	-smp 1 \
	-machine virt \
	-bios none \
	-m 128M 



# ----------------------------------------------------------
# 工具配置
# ----------------------------------------------------------

# QEMU
QEMU = qemu-system-riscv64

# GDB
GDB = gdb-multiarch

# cross-compiler prefix
CROSS_COMPILE = riscv64-unknown-elf-

# GCC
CC = ${CROSS_COMPILE}gcc

# OBJCOPY
OBJCOPY = ${CROSS_COMPILE}objcopy

# OBJDUMP
OBJDUMP = ${CROSS_COMPILE}objdump


# ----------------------------------------------------------
# 源文件设置
# ----------------------------------------------------------

# source files
SRCS_ASM = \
	start.S \
	mem.S \
	entry.S \

SRCS_C = \
	kernel.c \
	uart.c \
	page.c \
	printf.c \
	sched.c \


# ----------------------------------------------------------
# 宏设置
# ----------------------------------------------------------

# 编译生成的内核的名字
KNAME := os.elf
# 获取当前正在执行的makefile的绝对路径
MKP := $(abspath $(lastword $(MAKEFILE_LIST)))
# 根文件夹
RDIR=$(shell dirname ${MKP})
# 编译文件夹
BDIR=$(shell dirname ${MKP})/build/

# 目标文件
OBJS = $(SRCS_ASM:.S=.o)	# 将 SRC_ASM 中的所有.S结尾的文件转换为.o结尾的文件, 并赋值给 OBJS 变量
OBJS += $(SRCS_C:.c=.o)		# 将 SRC_ASM 中的所有.c结尾的文件转换为.o结尾的文件, 并添加给 OBJS 变量
OBJS := $(addprefix ${BDIR}, ${OBJS})	# OBJS 变量中的所有值添加 DIR 前缀


# ----------------------------------------------------------
# 隐式目标
# ----------------------------------------------------------

# compile all .c file into .o
${BDIR}%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

# compile all .S file into .o
${BDIR}%.o: %.S
	${CC} ${CFLAGS} -c -o $@ $<



# ----------------------------------------------------------
# 编译目标
# ----------------------------------------------------------

# all 是默认目标, 将会:
#		1. 创建 build 编译文件夹
#		2. 编译得到内核 os.elf
#		3. 反汇编内核, 得到内核的汇编代码和内核的符号表
#		4. 生成文档
.DEFAULT_GOAL := all
all: mkdir kernel disasm documentation 

# mkdir 伪目标将会:
#		1. 创建 build 编译文件夹
.PHONY : mkdir
mkdir:
	@mkdir -p ${BDIR}disasms

# kernel 目标依赖于 OBJ 目标, 具体来说会:
#		1. 根据 os.ld 链接脚本链接所有的目标文件以生产二进制内核文件
#		2. 将二进制内核文件转换为裸二进制文件 os.bin 以删除ELF文件中没有用到的段, 为后续的反汇编进行准备
kernel: ${OBJS}
	@${CC} ${CFLAGS} ${SRC} -T os.ld -o ${BDIR}${KNAME} $^
	@${OBJCOPY} -O binary ${BDIR}${KNAME} ${DIR}os.bin

# disasm 目标依赖于 kernel 目标, 具体来说会:
#		1. 反汇编得到内核的汇编代码
#		2. 输出内核的二进制内容
#		3. 输出内核的符号表
disasm: kenel
	@echo "内核的反汇编代码可以在文件" ${BDIR}diasms/os.code "查看"
	@${OBJDUMP} -S ${BDIR}${KNAME} > ${DIR}disasms/os.disasm
	@echo "内核的二进制内容可以在文件" ${BDIR}diasms/os.machine "查看"
	@hexdump -C ${BDIR}os.bin > ${DIR}disasms/os.machine
	@echo "内核的符号表可以在文件" ${BDIR}disasms/os.symbol "查看"
	@echo '内核文件: 符号地址 (符号长度) 符号类型 符号名 符号定义处' > ${BDIR}disasms/os.symbol
	@echo -e '常见符号类型 (更多符号类型请参考 man nm):' >> ${BDIR}disasms/os.symbol
	@echo -e '\tA:\t该符号的值在今后的链接中将不再改变' >> ${BDIR}disasms/os.symbol
	@echo -e '\tB:\t该符号位与BSS段中, 通常是未初始化的全局变量' >> ${BDIR}disasms/os.symbol
	@echo -e '\tb:\t该符号位与BSS段中, 通常是未初始化的静态全局变量, 例如: static int test' >> ${BDIR}disasms/os.symbol
	@echo -e '\tD:\t该符号位与普通的数据段中, 通常是已经初始化的全局变量' >> ${BDIR}disasms/os.symbol
	@echo -e '\tt/T:\t该符号位与代码段中, 通常是全局非静态函数' >> ${BDIR}disasms/os.symbol
	@echo -e '\tR:\t该符号位与只读数据区' >> ${BDIR}disasms/os.symbol
	@echo -e '\tU:\t该符号未在当前文件中定义过, 需要自其他对象文件中链接进来' >> ${BDIR}disasms/os.symbol
	@echo -e '\tW:\t未明确指定的弱链接符号, 若链接的其他对象文件中有它的定义就链接, 否则就用一个系统指定的默认值, 通常是动态链接库' >> ${BDIR}disasms/os.symbol
	@nm -l -n -S -A --print-armap ${BDIR}${KNAME} >> ${DIR}disasms/os.symbol


# documentation 目标将会:
#		1. 构建文件系统
documentation: ${RDIR}/docs/sphinx/Makefile
	@echo "正在构建文件系统...."
	make -C ${RDIR}/docs/sphinx html


# clean 伪目标将会:
#		1. 清除中间文件
.PHONY: clean
clean:
	rm -rf ${BDIR}*
	rm -rf ${RDIR}/docs/doxygen/build/*
	rm -rf ${RDIR}/docs/sphinx/build/*



# ----------------------------------------------------------
# 命令行工具
# ----------------------------------------------------------

# read 伪目标将会:
#		1. 在本地启动HttpServer以阅读文档
PORT := $(shell python -c "from random import randint; print(randint(8000, 65536))")
.PHONY: read
read:
	@echo "正在启动 HttpServer ..."
	@echo "本地文档链接: http://127.0.0.1:${PORT}"
	@echo "在线文档链接: https://x2w-os.readthedocs.io/en/latest/"
	@echo 'HttpServer日志将会打印在下方, 按下 Ctrl+C 或运行 `make kill` 以终止本地 HttpServer'
	@echo "-------------------------------------------------------------------------------"
	@cd ${RDIR}/docs/sphinx/build/html && (python -m http.server ${PORT} 2>&1 >/dev/null ) && cd ${RDIR}


# kill 伪目标将会:
#		1. 终止所有QEMU进程
#		2. 终止所有Python.HttpServer进程
.PHONY: kill
kill:
	@echo "正在终止所有 QEMU 进程..."
	ps aux | grep -e 'qemu' | grep -v 'grep' | awk '{print $$2}' | xargs --no-run-if-empty kill -9
	@echo "正在终止所有 Python.HttpServer 进程..."
	ps aux | grep -e 'http.server' | grep -v 'grep' | awk '{print $$2}' | xargs --no-run-if-empty kill -9

# run 目标依赖于 all 目标, 将会:
#		1. 使用 QEMU 运行内核
.PHONY: run
run: all
	@echo '运行内核: ${KNAME}, 所有输出将在下方显示'
	@echo "按下 Ctrl+A, 然后按下 X 键以退出 QEMU"
	@echo '你可以运行 `make debug-gdb` 以使用 GDB 调试内核'
	@echo '或者运行 `make debug-vscode` 以使用 VSCode 链接 QEMU 调试内核'
	@echo "----------------------------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${BDIR}${KNAME}

# debug-gdb 目标依赖于 all 目标, 将会使用 gdb 调试内核:
#		1. qemu -kernel: 指定要调试的ELF格式二进制内核文件
#		2. qemu -s: 使用 QEMU 的 GDB Server 调试内核, 默认端口是 1234
#		3. qemu -S: 启动 QEMU 后停止 CPU 运行, 直到 GDB 客户端连接到 QEMU GDB Server
#		4. qemu &: 在后台运行 QEMU, 并在前台运行 GDB
#		5. GDB -q: 取消 GDB 链接时的一些输出信息
#		6. GDB -x: 设置 GDB 启动后运行的命令, 这里包含: 链接到 GDB Server, 在 _start 处设置断点
.PHONY : debug-gdb
debug-gdb: all
	@echo "使用 GDB 调试内核, 所有输出将在下方显示"
	@echo "按下 Ctrl+C 以终止QEMU, 然后输入 quit 退出 GDB"
	@echo '你可以运行 `make run` 以使用 QEMU 直接运行内核'
	@echo '或者运行 `make debug-vscode` 以使用 VSCode 链接 QEMU 调试内核'
	@echo "------------------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${BDIR}${KNAME} -gdb tcp::1234 -S &
	@${GDB} ${BDIR}${KNAME} -q -x ./gdbinit

# debug-server 目标依赖于 all 目标, 将会使用 vs-code 调试内核:
.PHONY : debug-vscode
debug-vscode:
	@echo "使用 VSCode 调试内核, 正在等待 VSCode 链接..."
	@echo "VSCode 终止调试后 QEMU 将自动停止运行"
	@echo '你可以运行 `make run` 以使用 QEMU 直接运行内核'
	@echo '你可以运行 `make debug-gdb` 以使用 GDB 调试内核'
	@echo "------------------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${BDIR}os.elf -gdb tcp::1234 -S

