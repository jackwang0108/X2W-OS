.section ".text.boot"

.global sbi_start
sbi_start:
    // 设置SBI栈, 目前是4KB大小
    la sp, stacks_start
    li t0, 4096
    add sp, sp, t0

    // 跳转到sbi的main函数, 定义在sbi_main中
    tail sbi_main

.section .data
.align 12
.global stacks_start
stacks_start:
    .skip 4096