.section ".text.boot"

.global kernel_start
kernel_start:
    # 关闭中断
    csrw sie, zero

    # 设置栈
    la sp, kstacks_start
    li t0, 4096
    add sp, sp, t0

    # 跳转到C函数
    tail kernel_main

.section .data
.align 12
.global kstacks_start
kstacks_start:
    .skip 4096