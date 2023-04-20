/**
 * @file trapframe.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `trapframe.c`是`trapframe`库的实现函数
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "trap/trapframe.h"


void print_gtrapframe(gtrapframe_t *gtf_ptr, printf_t print_func){
    printf_t printf = print_func;
	printf(" gp : %#016lX tp : %#016lX t0 : %#016lX\n", gtf_ptr->gp, gtf_ptr->tp, gtf_ptr->t0);
	printf(" t1 : %#016lX t2 : %#016lX t3 : %#016lX\n", gtf_ptr->t1, gtf_ptr->t2, gtf_ptr->s0);
	printf(" s1 : %#016lX a0 : %#016lX a1 : %#016lX\n", gtf_ptr->s1, gtf_ptr->a0, gtf_ptr->a1);
	printf(" a2 : %#016lX a3 : %#016lX a4 : %#016lX\n", gtf_ptr->a2, gtf_ptr->a3, gtf_ptr->a4);
	printf(" a5 : %#016lX a6 : %#016lX a7 : %#016lX\n", gtf_ptr->a5, gtf_ptr->a6, gtf_ptr->a7);
	printf(" s2 : %#016lX s3 : %#016lX s4 : %#016lX\n", gtf_ptr->s2, gtf_ptr->s3, gtf_ptr->s4);
	printf(" s5 : %#016lX s6 : %#016lX s7 : %#016lX\n", gtf_ptr->s5, gtf_ptr->s6, gtf_ptr->s7);
	printf(" s8 : %#016lX s9 : %#016lX s10: %#016lX\n", gtf_ptr->s8, gtf_ptr->s9, gtf_ptr->s10);
	printf(" s11: %#016lX t3 : %#016lX t4 : %#016lX\n", gtf_ptr->s11, gtf_ptr->t3, gtf_ptr->t4);
	printf(" t5 : %#016lX t6 : %#016lX sp : %#016lX\n", gtf_ptr->t5, gtf_ptr->t6, gtf_ptr->sp);
	printf(" ra : %#016lX\n", gtf_ptr->ra);
}


void print_strapframe(strapframe_t *stf_ptr, printf_t print_func){
    printf_t printf = print_func;
    printf("Print SBI Trap Frame at: %#016X\n", (void*)stf_ptr);
    printf("mepc: %#016lX                  mstatus : %#016lX\n", stf_ptr->mepc, stf_ptr->mstatus);
	print_gtrapframe(&stf_ptr->gregisters, printf);
}


void print_ktrapframe(ktrapframe_t *ktf_ptr, printf_t print_func){
    printf_t printf = print_func;
    printf("Print Kernel Trap Frame at: %#016X\n", (void*)ktf_ptr);
	printf("origin_a0: %#016lX\n", ktf_ptr->origin_a0);
    printf("sepc: %#016lX                  sstatus : %#016lX\n", ktf_ptr->sepc, ktf_ptr->sstatus);
    printf("scause: %#016lX               sbadaddr : %#016lX\n", ktf_ptr->scause, ktf_ptr->sbadaddr);
	print_gtrapframe(&ktf_ptr->gregisters, printf);
}