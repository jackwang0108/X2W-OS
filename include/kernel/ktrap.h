/**
 * @file ktrap.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ktrap.h`是内核的异常/中断管理模块
 * @version 0.1
 * @date 2023-04-20
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_KTRAP_H
#define __INCLUDE_KERNEL_KTRAP_H

#include "types.h"
#include "asm/csr.h"
#include "constrains.h"
#include "trap/trapframe.h"
#include "trap/trap_entry.h"


/**
 * @brief `ktrap_handler_t`是异常/中断处理函数的函数类型, 函数原型为: `int64_t funcname(struct ktrapframe_t*)` 
 * 
 * @param ktf_ptr 指向被打断的程序的陷入帧, 程序的陷入帧将在`ktrap_enter`中构建在`S模式`的栈中
 * 
 * @note 如果被打断的程序想要传参给异常/中断处理函数, 那么必须要通过寄存器来实现, 而所有的通用寄存器在`ktrap_enter`函数中会被保存到陷入帧中.
 *      所以异常/中断处理函数必然可以通过陷入栈来获得被打断的程序传入的参数
 * 
 * @note RISC-V通过`a0~a7`寄存器传参, 因此在`ktrap_enter`中要把`ktrapframe`的地址保存到`a0`寄存器中
 */
typedef int64_t (*ktrap_handler_t)(ktrapframe_t *ktf_ptr);


/**
 * @brief `supervisor_interrupt_enable`用于打开S模式下的中断, 即设置`sstatus`寄存器的`SIE`位为1
 */
static inline void supervisor_interrupt_enable(void){
    set_csr(sstatus, SSTATUS_SIE);
}

/**
 * @brief `supervisor_interrupt_disable`用于关闭S模式下的中断, 即设置`sstatus`寄存器的`SIE`位为0
 */
static inline void supervisor_interrupt_disable(void){
    clear_csr(sstatus, SSTATUS_SIE);
}

/**
 * @brief `ktrap_init`是内核的异常/中断初始化函数, 主要:
 *      1. 设置了`sstvec`寄存器
 *      2. 设置了`sie`寄存器
 *      3. 将所有异常和中断处理函数设置为通用异常/中断处理函数
 */
void ktrap_init(void);


/**
 * @brief `ktrap_dispatcher`是通用异常/中断处理函数, 将会根据`scause`寄存器的值运行不同的中断处理函数
 * 
 * @param ktf_ptr 保存被中断的程序上下文信息(所有通用寄存器和部分CSR寄存器)的陷入栈, 在`ktrap_enter`中构建
 */
void ktrap_dispatcher(ktrapframe_t *ktf_ptr);


/**
 * @brief `register_ktrap_handler`是`SBI`异常/中断注册函数, 用于将编号为`trap_code`的异常/中断的处理函数`ktrap_func`注册到`SBI`的异常/中断处理函数表中
 * 
 * @param trap_code 注册的中断/异常的编号
 * @param interrupt 若为True, 则将处理函数注册为中断处理函数, 否则注册为异常处理函数
 * @param msg 中断的相关信息
 * @param ktrap_func 中断/异常处理函数, 需要为类型`ktrap_handler_t`, 即函数原型需要为`int64_t funcname(ireg_t kcause)`
 */
void register_ktrap_handler(uint64_t trap_code, Bool interrupt, const char* msg, ktrap_handler_t ktrap_func);


/**
 * @brief `general_ktrap_handler`是所有中断/异常的缺省处理函数, 将会在屏幕上输出中断/异常信息, 打印陷入帧, 而后挂起内核
 * 
 * @param ktf_ptr 保存被中断的程序上下文信息(所有通用寄存器和部分CSR寄存器)的陷入栈, 在`ktrap_enter`中构建
 * @return NO_RETURN 该函数将挂起内核, 不会返回至内核继续运行
 */
NO_RETURN int64_t general_ktrap_handler(ktrapframe_t *ktf_ptr);


#endif