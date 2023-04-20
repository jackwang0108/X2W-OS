/**
 * @file strap.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `strap.h`是`SBI`的异常/中断管理模块
 * @version 0.1
 * @date 2023-04-15
 * 
 * @todo
 *  1. 添加浮点寄存器支持
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_SBI_STRAP_H
#define __INCLUDE_SBI_STRAP_H

#include "types.h"
#include "constrains.h"
#include "trap/trapframe.h"
#include "trap/trap_entry.h"


/**
 * @brief `trap_handler_t`是异常/中断处理函数的函数类型, 函数原型为: `int64_t funcname(struct strapframe_t*)` 
 * 
 * @param stf_ptr 指向被打断的程序的陷入帧, 程序的陷入帧将在`trap_exter`中构建在`M模式`的栈中
 * 
 * @note 如果被打断的程序想要传参给异常/中断处理函数, 那么必须要通过寄存器来实现, 而寄存器在`trap_enter`函数中会被保存到陷入帧中.
 *      所以异常/中断处理函数必然可以通过陷入栈来获得被打断的程序传入的参数
 */
typedef int64_t (*trap_handler_t)(strapframe_t* stf_ptr);


/**
 * @brief `strap_init`是`SBI`的异常/中断初始化函数
 */
void strap_init(void);


/**
 * @brief `strap_dispatcher`是通用异常/中断处理函数, 将会根据`mcause`寄存器的值运行不同的中断处理函数
 * 
 * @param stf_ptr 保存被中断的程序上下文信息(所有通用寄存器和部分CSR寄存器)的陷入栈, 在`strap_enter`中构建
 */
void strap_dispatcher(strapframe_t *stf_ptr);


/**
 * @brief `regitser_trap_handler`是异常/中断注册函数, 用于将编号为`trap_code`的异常/中断的处理函数`trap_func`注册到异常/中断处理函数表中
 * 
 * @param trap_code 注册的中断/异常的编号
 * @param interrupt 若为True, 则将处理函数注册为中断处理函数, 否则注册为异常处理函数
 * @param msg 中断的相关信息
 * @param trap_func 中断/异常处理函数, 需要为类型`trap_handler_t`, 即`int64_t funcname(ireg_t mcause)`
 */
void regitser_trap_handler(uint64_t trap_code, Bool interrupt, const char* msg, trap_handler_t trap_func);


/**
 * @brief `general_trap_handler`是所有中断/异常的缺省处理函数, 将会在屏幕上输出中断/异常信息, 打印陷入帧, 而后挂起内核
 * 
 * @param mcause mcasue寄存器的值
 * @return NO_RETURN 该函数将挂起内核, 不会返回至内核继续运行
 */
NO_RETURN int64_t general_trap_handler(strapframe_t *stf_ptr);


#endif