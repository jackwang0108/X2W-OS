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


/**
 * @brief `reg_offset`用于获得`strapframe_t`结构体中名为`rname`的寄存器的字节偏移量
 * 
 * @param `rname`寄存器名
 */
#define reg_offset(rname) (size_t)(&(((strapframe_t *)0)->rname))



/**
 * @brief `strapframe_t`是`SBI`的异常/中断的陷入帧(Trap Frame), 其保存了中断/异常发生时候被打断的程序的上下文信息 (全部的通用寄存器(x1-x31)和部分CSR寄存器(mstatus, mepc)).
 *  
 * 
 * @note
 *  1. `strapframe_t`是否包含浮点寄存器取决于`constrain.h`的`WITH_FP_REG`宏:
 *      - `WITH_FP_REG = 0`, 不包含浮点寄存器, 此时保存: `mepc`, `x1-x31`, `mstatus`寄存器. 一共33个寄存器, 共 264 字节
 *      - `WITH_FP_REG = 1`, 包含浮点寄存器, 此时保存: `mepc`, `x1-x31, `f0-f31`, `mstatus`寄存器. 一共33 + 32个寄存器, 共 264 + 128 字节
 *  2. `strpframe_t`用于在陷入中断/异常的时候保存现场, 以方便退出中断/异常的时候恢复现场
 *  3. 命令规则参考: RISC-V Assembly Programmer’s Handbook
 * 
 * @warning 目前不支持浮点寄存器, 支持浮点寄存器的话, 需要额外修改strap_entry.S里的汇编入口函数来保存
 */
typedef struct __strapframe_t {
    // 31个通用寄存器, 不保存x0/zero寄存器
    ireg_t mepc;

    union { ireg_t _x1; ireg_t ra; };
    union { ireg_t _x2; ireg_t sp; };
    union { ireg_t _x3; ireg_t gp; };
    union { ireg_t _x4; ireg_t tp; };
    union { ireg_t _x5; ireg_t t0; };
    union { ireg_t _x6; ireg_t t1; };
    union { ireg_t _x7; ireg_t t2; };
    union { ireg_t _x8; ireg_t s0; ireg_t fp; };
    union { ireg_t _x9; ireg_t s1; };
    union { ireg_t _x10; ireg_t a0; };
    union { ireg_t _x11; ireg_t a1; };
    union { ireg_t _x12; ireg_t a2; };
    union { ireg_t _x13; ireg_t a3; };
    union { ireg_t _x14; ireg_t a4; };
    union { ireg_t _x15; ireg_t a5; };
    union { ireg_t _x16; ireg_t a6; };
    union { ireg_t _x17; ireg_t a7; };
    union { ireg_t _x18; ireg_t s2; };
    union { ireg_t _x19; ireg_t s3; };
    union { ireg_t _x20; ireg_t s4; };
    union { ireg_t _x21; ireg_t s5; };
    union { ireg_t _x22; ireg_t s6; };
    union { ireg_t _x23; ireg_t s7; };
    union { ireg_t _x24; ireg_t s8; };
    union { ireg_t _x25; ireg_t s9; };
    union { ireg_t _x26; ireg_t s10; };
    union { ireg_t _x27; ireg_t s11; };
    union { ireg_t _x28; ireg_t t3; };
    union { ireg_t _x29; ireg_t t4; };
    union { ireg_t _x30; ireg_t t5; };
    union { ireg_t _x31; ireg_t t6; };

#if WITH_FP_REG == 1
    // TODO: 未来可能添加浮点寄存器的支持
    union { freg_t _f0; freg_t ft0; };
    union { freg_t _f1; freg_t ft1; };
    union { freg_t _f2; freg_t ft2; };
    union { freg_t _f3; freg_t ft3; };
    union { freg_t _f4; freg_t ft4; };
    union { freg_t _f5; freg_t ft5; };
    union { freg_t _f6; freg_t ft6; };
    union { freg_t _f7; freg_t ft7; };
    union { freg_t _f8; freg_t fs0; };
    union { freg_t _f9; freg_t fs1; };
    union { freg_t _f10; freg_t fa0; };
    union { freg_t _f11; freg_t fa1; };
    union { freg_t _f12; freg_t fa2; };
    union { freg_t _f13; freg_t fa3; };
    union { freg_t _f14; freg_t fa4; };
    union { freg_t _f15; freg_t fa5; };
    union { freg_t _f16; freg_t fa6; };
    union { freg_t _f17; freg_t fa7; };
    union { freg_t _f18; freg_t fs2; };
    union { freg_t _f19; freg_t fs3; };
    union { freg_t _f20; freg_t fs4; };
    union { freg_t _f21; freg_t fs5; };
    union { freg_t _f22; freg_t fs6; };
    union { freg_t _f23; freg_t fs7; };
    union { freg_t _f24; freg_t fs8; };
    union { freg_t _f25; freg_t fs9; };
    union { freg_t _f26; freg_t fs10; };
    union { freg_t _f27; freg_t fs11; };
    union { freg_t _f28; freg_t ft8; };
    union { freg_t _f29; freg_t ft9; };
    union { freg_t _f30; freg_t ft10; };
    union { freg_t _f31; freg_t ft11; };
#endif
    
    // mstatus 寄存器
    ireg_t mstatus;
} strapframe_t;

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


/**
 * @brief `print_trapframe`用于输出陷入帧`stf_ptr`
 * 
 * @param stf_ptr 要输出的陷入帧
 */
void print_trapframe(strapframe_t *stf_ptr);


/**
 * @brief `strap_enter`是`SBI`的陷入入口函数, 由汇编实现, 定义在 `strap_enter.S`中
 * 
 * @note 该函数会:
 *      1. 在`M模式`的栈中构建被打断程序的陷入帧
 *      2. 将被打断的程序的陷入帧作为参数调用`strap_dispatcher`函数
 */
extern void strap_enter(void);

/**
 * @brief `strap_exit`是`SBI`的陷入出口函数, 由汇编实现, 定义在 `strap_enter.S`中
 * 
 * @note 该函数会:
 *      1. 将`M模式`的栈中构建的被打断程序的陷入帧恢复到寄存器中
 */
extern void strap_exit(void);

#endif