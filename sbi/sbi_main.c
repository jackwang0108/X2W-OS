/**
 * @file sbi_main.c
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `sbi_main.c`是`SBI`的主文件
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */


#include "types.h"
#include "asm/csr.h"
#include "constrains.h"

/**
 * @brief `sbi_main`是`sbi`的主函数, 运行在M模式
 * 
 * @note 
 * 1. 其通过伪装`S`模式中断返回的模式跳转到内核的主函数
 * 
 * @note `sbi_main`跳转到内核前准备工作
 * 1. 伪装从S模式中断进入M模式
 * 2. 设置M模式的中断返回地址
 * 3. 设置S模式中断向量表入口
 * 4. 关闭S模式的中断
 * 5. 关闭S模式的页表转换
 */
void sbi_main(void);

void sbi_main(void){

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

    // 伪装中断返回, 返回到S模式
    asm volatile("mret");
}