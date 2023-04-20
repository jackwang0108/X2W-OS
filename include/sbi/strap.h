/**
 * @file strap.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `strap.h`是`SBI`的异常/中断管理模块
 * @version 0.1
 * @date 2023-04-15
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
 * @brief `strap_handler_t`是异常/中断处理函数的函数类型, 函数原型为: `int64_t funcname(struct strapframe_t*)` 
 * 
 * @param stf_ptr 指向被打断的程序的陷入帧, 程序的陷入帧将在`strap_enter`中构建在`M模式`的栈中
 * 
 * @note 如果被打断的程序想要传参给异常/中断处理函数, 那么必须要通过寄存器来实现, 而所有的通用寄存器在`strap_enter`函数中会被保存到陷入帧中.
 *      所以异常/中断处理函数必然可以通过陷入栈来获得被打断的程序传入的参数
 * 
 * @note RISC-V通过`a0~a7`寄存器传参, 因此在`strap_enter`中要把`strapframe`的地址保存到`a0`寄存器中
 */
typedef int64_t (*strap_handler_t)(strapframe_t* stf_ptr);


/**
 * @brief `strap_init`是`SBI`的异常/中断初始化函数
 */
void strap_init(void);


/**
 * @brief `delegate_traps`用于将`M模式`下的一些中断和异常委托至`S模式`
 * 
 * @note 目前将以下中断/异常进行了委托:
 *  1. 软件中断, 时钟中断, 外部中断
 *  2. 指令未对齐异常, 读取指令页异常, 读取数据页异常, 存储数据页异常, 读取数据异常, 存储数据异常, 断点异常, U模式下的ecall异常
 * 
 * @note 
 *  1. 委托中断和异常本质上就是读写`mideleg`和`medeleg`寄存器. 这两个寄存器每一位对应一种中断/异常
 *  2. 设置`mideleg`和`medeleg`寄存器对应的位后, 对应的中断/异常发生后就会进入到`ktrap_enter`中
 *  3. 这两个寄存器每一位具体操作的是什么中断/异常, 参考RISC-V手册: 3.1.8 Machine Trap Delegation Registers (medeleg and mideleg) 
 */
void delegate_traps(void);


/**
 * @brief `strap_dispatcher`是通用异常/中断处理函数, 将会根据`mcause`寄存器的值运行不同的中断处理函数
 * 
 * @param stf_ptr 保存被中断的程序上下文信息(所有通用寄存器和部分CSR寄存器)的陷入栈, 在`strap_enter`中构建
 */
void strap_dispatcher(strapframe_t *stf_ptr);


/**
 * @brief `regitser_strap_handler`是`SBI`异常/中断注册函数, 用于将编号为`trap_code`的异常/中断的处理函数`strap_func`注册到`SBI`的异常/中断处理函数表中
 * 
 * @param trap_code 注册的中断/异常的编号
 * @param interrupt 若为True, 则将处理函数注册为中断处理函数, 否则注册为异常处理函数
 * @param msg 中断的相关信息
 * @param strap_func 中断/异常处理函数, 需要为类型`strap_handler_t`, 即函数原型需要为`int64_t funcname(ireg_t mcause)`
 */
void regitser_strap_handler(uint64_t trap_code, Bool interrupt, const char* msg, strap_handler_t strap_func);


/**
 * @brief `general_strap_handler`是所有中断/异常的缺省处理函数, 将会在屏幕上输出中断/异常信息, 打印陷入帧, 而后挂起内核
 * 
 * @param stf_ptr 保存被中断的程序上下文信息(所有通用寄存器和部分CSR寄存器)的陷入栈, 在`strap_enter`中构建
 * @return NO_RETURN 该函数将挂起内核, 不会返回至内核继续运行
 */
NO_RETURN int64_t general_strap_handler(strapframe_t *stf_ptr);


#endif