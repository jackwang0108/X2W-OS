/**
 * @file smain.c
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `sbi_main.c`是`SBI`的主文件
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "uart.h"
#include "types.h"
#include "constrains.h"
#include "asm/csr.h"
#include "sbi/sinit.h"
#include "sbi/smain.h"


NO_RETURN void sbi_main(void){
    // 初始化 UART 设备
    uart_init();
    // 输出 SBI Banner
    uart_puts(X2WSBI_BANNER);
    uart_puts(DELIMITER);
    // 输出 SBI 提示信息
    uart_puts("Enter SBI!\n");
    // 初始化 SBI 其余各个组件
    sinit_all();
    // 跳转至内核
    jump_to_kernel();
    UNREACHABLE;
}



NO_RETURN void jump_to_kernel(){
    // 设置中断前模式
    uint64_t mval;
    mval = read_csr(mstatus);
    mval = SET_FIELD(mval, MSTATUS_MPP, PRIVILEGE_S);
    mval = SET_FIELD(mval, MSTATUS_MPIE, INTR_DS);
    write_csr(mstatus, mval);

    // 设置M模式的EPC(Exception Program Counter)寄存器, 用于mret跳转
    write_csr(mepc, KERNEL_JUMP_ADDR);
    // 设置S模式的异常向量表入口
    write_csr(stvec, KERNEL_JUMP_ADDR);
    // 关闭S模式的中断
    write_csr(sie, 0);
    // 关闭S模式的页表转换
    write_csr(satp, 0);

    // 设置物理地址保护(Physical Memory Protection)
    write_csr(pmpaddr0, (ireg_t)0x3FFFFFFFFFFFFF);
    write_csr(pmpcfg0, 0xF);

    uart_puts("Jump to kernel!\n");
    // 伪装中断返回, 返回到S模式
    asm volatile("mret");
    UNREACHABLE;
}
