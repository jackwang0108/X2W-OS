/**
 * @file trapframe.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `trapframe.h`中定义了通用的陷入帧`gtrapframe_t`
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_TRAP_TRAPFRAME_H
#define __INCLUDE_TRAP_TRAPFRAME_H

#include "types.h"
#include "constrains.h"


/**
 * @brief `gtrapframe_t`是通用陷入帧(General Trap Frame), 主要用于陷入中断/异常保存通用寄存器, 以方便在退出中断/异常的时候恢复现场
 * 
 * @note 
 *  1. `gtrapframe_t`是否包含浮点寄存器取决于`constrain.h`的`WITH_FP_REG`宏:
 *      - `WITH_FP_REG = 0`, 不包含浮点寄存器, 此时保存: `x1~x31` 寄存器. 一共 31 个寄存器, 共 248 字节
 *      - `WITH_FP_REG = 1`, 包含浮点寄存器, 此时保存: `x1~x31`, `f0~f31` 一共 31 + 32 个寄存器, 共 248 + 128 字节
 *  2. 寄存器命名规则参考: RISC-V Assembly Programmer’s Handbook
 * 
 * @warning 目前不支持浮点寄存器, 支持浮点寄存器的话, 需要额外修改trap_entry.S里的汇编入口函数来保存
 */
typedef struct __gtrapframe_t {
    /// @brief `ra`寄存器(`Return Address`)用于保存函数调用时的返回地址
    union { ireg_t _x1; ireg_t ra; };
    /// @brief `sp`寄存器(`Stack Pointer`)用于保存当前线程的栈底指针
    union { ireg_t _x2; ireg_t sp; };
    /// @brief `gp`寄存器(`Golbal Pointer`)用于保存全局指针
    union { ireg_t _x3; ireg_t gp; };
    /// @brief `tp`寄存器(`Thread Pointer`)用于保存当前线程帧的指针
    union { ireg_t _x4; ireg_t tp; };
    /// @brief `t0`寄存器(`Temporary 0`)是临时寄存器或者备用`ra`寄存器
    union { ireg_t _x5; ireg_t t0; };
    /// @brief `t1`寄存器(`Temporary 1`)是临时寄存器
    union { ireg_t _x6; ireg_t t1; };
    /// @brief `t2`寄存器(`Temporary 2`)是临时寄存器
    union { ireg_t _x7; ireg_t t2; };
    /// @brief `s0`寄存器(`Saved Register 0`)是保存寄存器/帧寄存器
    union { ireg_t _x8; ireg_t s0; ireg_t fp; };
    /// @brief `s1`寄存器(`Saved Register 1`)是保存寄存器
    union { ireg_t _x9; ireg_t s1; };
    /// @brief `a0`寄存器(`Argument 0`)是函数调用参数寄存器/返回值寄存器
    union { ireg_t _x10; ireg_t a0; };
    /// @brief `a1`寄存器(`Argument 1`)是函数调用参数寄存器
    union { ireg_t _x11; ireg_t a1; };
    /// @brief `a2`寄存器(`Argument 2`)是函数调用参数寄存器
    union { ireg_t _x12; ireg_t a2; };
    /// @brief `a3`寄存器(`Argument 3`)是函数调用参数寄存器
    union { ireg_t _x13; ireg_t a3; };
    /// @brief `a4`寄存器(`Argument 4`)是函数调用参数寄存器
    union { ireg_t _x14; ireg_t a4; };
    /// @brief `a5`寄存器(`Argument 5`)是函数调用参数寄存器
    union { ireg_t _x15; ireg_t a5; };
    /// @brief `a6`寄存器(`Argument 6`)是函数调用参数寄存器
    union { ireg_t _x16; ireg_t a6; };
    /// @brief `a7`寄存器(`Argument 7`)是函数调用参数寄存器
    union { ireg_t _x17; ireg_t a7; };
    /// @brief `s2`寄存器(`Saved Register 2`)是保存寄存器
    union { ireg_t _x18; ireg_t s2; };
    /// @brief `s3`寄存器(`Saved Register 3`)是保存寄存器
    union { ireg_t _x19; ireg_t s3; };
    /// @brief `s4`寄存器(`Saved Register 4`)是保存寄存器
    union { ireg_t _x20; ireg_t s4; };
    /// @brief `s5`寄存器(`Saved Register 5`)是保存寄存器
    union { ireg_t _x21; ireg_t s5; };
    /// @brief `s6`寄存器(`Saved Register 6`)是保存寄存器
    union { ireg_t _x22; ireg_t s6; };
    /// @brief `s7`寄存器(`Saved Register 7`)是保存寄存器
    union { ireg_t _x23; ireg_t s7; };
    /// @brief `s8`寄存器(`Saved Register 8`)是保存寄存器
    union { ireg_t _x24; ireg_t s8; };
    /// @brief `s9`寄存器(`Saved Register 9`)是保存寄存器
    union { ireg_t _x25; ireg_t s9; };
    /// @brief `s10`寄存器(`Saved Register 10`)是保存寄存器
    union { ireg_t _x26; ireg_t s10; };
    /// @brief `s11`寄存器(`Saved Register 11`)是保存寄存器
    union { ireg_t _x27; ireg_t s11; };
    /// @brief `t3`寄存器(`Temporary 3`)是临时寄存器
    union { ireg_t _x28; ireg_t t3; };
    /// @brief `t4`寄存器(`Temporary 4`)是临时寄存器
    union { ireg_t _x29; ireg_t t4; };
    /// @brief `t5`寄存器(`Temporary 5`)是临时寄存器
    union { ireg_t _x30; ireg_t t5; };
    /// @brief `t6`寄存器(`Temporary 6`)是临时寄存器
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
} gtrapframe_t;


/// @brief `strapframe_t`是`SBI`陷入帧(SBI Trap Frame), 在保存通用寄存器的基础上, 额外保存了`empc`寄存器和`mstatus`寄存器
typedef struct __strapframe_t {
    /// @brief `mepc`保存了`mepc`寄存器的值
    ireg_t mepc;
    /// @brief `gregisters`寄存器保存了通用寄存器的值
    gtrapframe_t gregisters;
    /// @brief `mstatus`保存了`mstatus`寄存器的值
    ireg_t mstatus;
} strapframe_t;


/// @brief `ktrapframe_t`是`Kernel`陷入帧(Kernel Trap Frame), 在保存通用寄存器的基础上, 额外保存了`sepc`, `sstatus`, `sbadaddr`, `scause`等寄存器
typedef struct __ktrapframe_t {
    /// @brief `spec`保存了`spec`寄存器的值
    ireg_t sepc;
    /// @brief `gregisters`寄存器保存了通用寄存器的值
    gtrapframe_t gregisters;
    /// @brief `sstatus`保存了`sstatus`寄存器的值
    ireg_t sstatus;
    /// @brief `sbadaddr`保存了`sbadaddr`寄存器的值
    ireg_t sbadaddr;
    /// @brief `scause`保存了`scause`寄存器的值
    ireg_t scause;
    /// @brief `origin_a0`保存了系统调用前的`a0`寄存器的值
    ireg_t origin_a0;
} ktrapframe_t;


/// @brief `printf_t`是输出函数类型
typedef size_t (*printf_t)(const char* format, ...);


/**
 * @brief `print_gtrapframe`用于输出通用陷入帧`gtf_ptr`
 * 
 * @param gtf_ptr 指向要输出的通用陷入帧的指针
 * @param print_func printf函数, 可以是: `bprintf`, `kprintf`, `uprintf`
 */
void print_gtrapframe(gtrapframe_t *gtf_ptr, printf_t print_func);


/**
 * @brief `print_strapframe`用于输出`SBI`陷入帧`stf_ptr`
 * 
 * @param stf_ptr 指向要输出的`SBI`陷入帧的指针
 * @param print_func printf函数, 可以是: `bprintf`, `kprintf`, `uprintf`
 */
void print_strapframe(strapframe_t *stf_ptr, printf_t print_func);


/**
 * @brief `print_ktrapframe`用于输出内核陷入帧`ktf_ptr`
 * 
 * @param ktf_ptr 指向要输出的内核陷入帧的指针
 * @param print_func printf函数, 可以是: `bprintf`, `kprintf`, `uprintf`
 */
void print_ktrapframe(ktrapframe_t *ktf_ptr, printf_t print_func);


#endif