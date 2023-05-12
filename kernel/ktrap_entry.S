#include "trap/tfoffset.h"

/**
 * @brief `ktrap_enter`是内核的的陷入入口函数, C语言描述为`void ktrap_enter(void)`
 * 
 * @note `ktrap_enter`函数的作用:
 *  1. 保存现场: 将通用寄存器和一些csr寄存器保存到陷入栈中
 *  2. 读取中断/异常号, 以作为参数调用`ktrap_dispatcher`函数处理异常
 * 
 * @note 
 *  1. `ktrap_enter`的地址将保存到`stvec`寄存器中, 具体由`ktrap.c`的`ktrap_init`函数实现
 *  2. `stvec`的直接模式`mode = 0`要求`ktrap_enter`的地址要4字节对齐, 这里是8字节对齐
 *  3. 在`ktrap_init`将`stvec`的中断向量值设置为`ktrap_enter`的地址后, 未来发生了异常就会进入到该`ktrap_enter`函数中运行
 *  4. 但是`RISC-V`的异常默认是在`M模式`下处理的, 异常处理涉及到的寄存器只能在`M模式`下设置和访问;
 *     如果`S模式`想要使用某个功能，需要再`M模式`下先对指定的中断和异常委托到`S模式`, 而当指定的中断和异常发生时, 就会进入到`S模式`中进行处理
 *     从而实现了`S模式`调用`M模式`提供的功能
 */
.align 3
.global ktrap_enter
ktrap_enter:
	addi sp, sp, -(KTF_SIZE)

	sd x1,  KTF_RA(sp)
	sd x3,  KTF_GP(sp)
	sd x5,  KTF_T0(sp)
	sd x6,  KTF_T1(sp)
	sd x7,  KTF_T2(sp)
	sd x8,  KTF_S0(sp)
	sd x9,  KTF_S1(sp)
	sd x10, KTF_A0(sp)
	sd x11, KTF_A1(sp)
	sd x12, KTF_A2(sp)
	sd x13, KTF_A3(sp)
	sd x14, KTF_A4(sp)
	sd x15, KTF_A5(sp)
	sd x16, KTF_A6(sp)
	sd x17, KTF_A7(sp)
	sd x18, KTF_S2(sp)
	sd x19, KTF_S3(sp)
	sd x20, KTF_S4(sp)
	sd x21, KTF_S5(sp)
	sd x22, KTF_S6(sp)
	sd x23, KTF_S7(sp)
	sd x24, KTF_S8(sp)
	sd x25, KTF_S9(sp)
	sd x26, KTF_S10(sp)
	sd x27, KTF_S11(sp)
	sd x28, KTF_T3(sp)
	sd x29, KTF_T4(sp)
	sd x30, KTF_T5(sp)
	sd x31, KTF_T6(sp)

	csrr s1, sstatus
	sd s1, KTF_SSTATUS(sp)

	/*保存sepc*/
	csrr s2, sepc
	sd s2, KTF_SEPC(sp)
	
	/*保存sbadaddr*/
	csrr s3, sbadaddr
	sd s3, KTF_SBADADDR(sp)

	/*保存scause*/
	csrr s4, scause
	sd s4, KTF_SCAUSE(sp)

	/*保存ssratch*/
	csrr s5, sscratch
	sd s5, KTF_TP(sp)

	/*保存SP*/
	addi s0, sp, KTF_SIZE 
	sd s0, KTF_SP(sp)

	csrw sscratch, x0

	la ra, ktrap_exit

	mv a0, sp /* pt_regs */
	mv a1, s4
	tail ktrap_dispatcher


/**
 * @brief `ktrap_exit`是`SBI`的陷入入口函数, C语言描述为`void ktrap_exit(void)`
 * 
 * @note `ktrap_exit`函数的作用:
 *  1. 恢复现场: 将`ktrap_enter`中得到的到陷入栈中的通用寄存器和一些csr寄存器的值加载到对应的寄存器中
 */
.global ktrap_exit
ktrap_exit:
	ld a0, KTF_SSTATUS(sp)
	csrw sstatus, a0

	ld a2, KTF_SEPC(sp)
	csrw sepc, a2

	ld x1,  KTF_RA(sp)
	ld x3,  KTF_GP(sp)
	ld x4,  KTF_TP(sp)
	ld x5,  KTF_T0(sp)
	ld x6,  KTF_T1(sp)
	ld x7,  KTF_T2(sp)
	ld x8,  KTF_S0(sp)
	ld x9,  KTF_S1(sp)
	ld x10, KTF_A0(sp)
	ld x11, KTF_A1(sp)
	ld x12, KTF_A2(sp)
	ld x13, KTF_A3(sp)
	ld x14, KTF_A4(sp)
	ld x15, KTF_A5(sp)
	ld x16, KTF_A6(sp)
	ld x17, KTF_A7(sp)
	ld x18, KTF_S2(sp)
	ld x19, KTF_S3(sp)
	ld x20, KTF_S4(sp)
	ld x21, KTF_S5(sp)
	ld x22, KTF_S6(sp)
	ld x23, KTF_S7(sp)
	ld x24, KTF_S8(sp)
	ld x25, KTF_S9(sp)
	ld x26, KTF_S10(sp)
	ld x27, KTF_S11(sp)
	ld x28, KTF_T3(sp)
	ld x29, KTF_T4(sp)
	ld x30, KTF_T5(sp)
	ld x31, KTF_T6(sp)

	ld x2,  KTF_SP(sp)
	sret
