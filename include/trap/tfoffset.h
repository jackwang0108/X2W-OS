/**
 * @file tfoffset.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `tfoffset.h`中定义了`trapframe.h`中的`gtrapframe_t`结构体各个成员变量的字节偏移数
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_TRAP_TFOFFSET_H
#define __INCLUDE_TRAP_TFOFFSET_H


#include "constrains.h"


#if WITH_FP_REG == 1            // 含浮点寄存器的trap frame
    /// `TF_SIZE`宏是`gtrapframe_t`结构体的总字节长度
    #define TF_SIZE 
#else                            // 不含浮点寄存器的trap frame
    /// `TF_SIZE`宏是`gtrapframe_t`结构体的总字节长度
    #define TF_SIZE    264
#endif

/// `TF_MEPC`宏是`mepc`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_MEPC    0
/// `TF_RA`宏是`ra`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_RA      8
/// `TF_SP`宏是`sp`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_SP      16
/// `TF_GP`宏是`gp`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_GP      24
/// `TF_TP`宏是`tp`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_TP      32
/// `TF_T0`宏是`t0`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T0      40
/// `TF_T1`宏是`t1`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T1      48
/// `TF_T2`宏是`t2`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T2      56
// S0和FP是一个寄存器
/// `TF_FP`宏是`fp`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_FP      64
/// `TF_S0`宏是`s0`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S0      64
/// `TF_S1`宏是`s1`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S1      72
/// `TF_A0`宏是`a0`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A0      80
/// `TF_A1`宏是`a1`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A1      88
/// `TF_A2`宏是`a2`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A2      96
/// `TF_A3`宏是`a3`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A3      104
/// `TF_A4`宏是`a4`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A4      112
/// `TF_A5`宏是`a5`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A5      120
/// `TF_A6`宏是`a6`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A6      128
/// `TF_A7`宏是`a7`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_A7      136
/// `TF_S2`宏是`s2`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S2      144
/// `TF_S3`宏是`s3`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S3      152
/// `TF_S4`宏是`s4`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S4      160
/// `TF_S5`宏是`s5`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S5      168
/// `TF_S6`宏是`s6`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S6      176
/// `TF_S7`宏是`s7`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S7      184
/// `TF_S8`宏是`s8`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S8      192
/// `TF_S9`宏是`s9`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S9      200
/// `TF_S10`宏是`s10`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S10     208
/// `TF_S11`宏是`s11`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_S11     216
/// `TF_T3`宏是`t3`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T3      224
/// `TF_T4`宏是`t4`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T4      232
/// `TF_t5`宏是`t5`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T5      240
/// `TF_t6`宏是`t6`寄存器在`gtrapframe_t`结构体中的字节偏移量
#define TF_T6      248


#if WITH_FP_REG == 1            // 含浮点寄存器的trap frame
    // TODO: 添加保存浮点寄存器
#else
    /// `TF_MSTATUS`宏是`mstatus`寄存器在`gtrapframe_t`结构体中的字节偏移量
    #define TF_MSTATUS      265
#endif


#endif