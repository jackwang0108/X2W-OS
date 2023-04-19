.section ".text.boot"

.global sbi_start
sbi_start:
    // 设置SBI栈, 目前是4KB大小
    la sp, sstacks_start
    li t0, 4096
    add sp, sp, t0

    // 设置mscratch寄存器的值为sbi的栈顶指针, 未来在strap_enter函数(定义在strap_entry.S中)中可以进行栈的切换
	csrw mscratch, sp

    // 跳转到sbi的main函数, 定义在sbi_main中
    tail sbi_main

.section .data
.align 12
.global sstacks_start
sstacks_start:
    .skip 4096