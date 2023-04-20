/**
 * @file tfoffset.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `tfoffset.h`中定义了`trapframe.h`中的`strapframe_t`和`ktrapframe_t`结构体各个成员变量的字节偏移数
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_TRAP_STFOFFSET_H
#define __INCLUDE_TRAP_STFOFFSET_H


#include "constrains.h"

/* ------------------------------  strapframe_t 相关寄存器 ------------------------------ */

#if WITH_FP_REG == 1            // 含浮点寄存器的trapframe
    /// `STF_SIZE`宏是`strapframe_t`结构体的总字节长度
    #define STF_SIZE 
#else                            // 不含浮点寄存器的trapframe
    /// `STF_SIZE`宏是`strapframe_t`结构体的总字节长度
    #define STF_SIZE    264
#endif

/// `STF_MEPC`宏是`mepc`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_MEPC    0
/// `STF_RA`宏是`ra`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_RA      8
/// `STF_SP`宏是`sp`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_SP      16
/// `STF_GP`宏是`gp`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_GP      24
/// `STF_TP`宏是`tp`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_TP      32
/// `STF_T0`宏是`t0`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T0      40
/// `STF_T1`宏是`t1`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T1      48
/// `STF_T2`宏是`t2`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T2      56
// S0和FP是一个寄存器
/// `STF_FP`宏是`fp`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_FP      64
/// `STF_S0`宏是`s0`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S0      64
/// `STF_S1`宏是`s1`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S1      72
/// `STF_A0`宏是`a0`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A0      80
/// `STF_A1`宏是`a1`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A1      88
/// `STF_A2`宏是`a2`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A2      96
/// `STF_A3`宏是`a3`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A3      104
/// `STF_A4`宏是`a4`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A4      112
/// `STF_A5`宏是`a5`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A5      120
/// `STF_A6`宏是`a6`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A6      128
/// `STF_A7`宏是`a7`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_A7      136
/// `STF_S2`宏是`s2`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S2      144
/// `STF_S3`宏是`s3`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S3      152
/// `STF_S4`宏是`s4`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S4      160
/// `STF_S5`宏是`s5`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S5      168
/// `STF_S6`宏是`s6`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S6      176
/// `STF_S7`宏是`s7`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S7      184
/// `STF_S8`宏是`s8`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S8      192
/// `STF_S9`宏是`s9`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S9      200
/// `STF_S10`宏是`s10`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S10     208
/// `STF_S11`宏是`s11`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_S11     216
/// `STF_T3`宏是`t3`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T3      224
/// `STF_T4`宏是`t4`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T4      232
/// `STF_t5`宏是`t5`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T5      240
/// `STF_t6`宏是`t6`寄存器在`strapframe_t`结构体中的字节偏移量
#define STF_T6      248

#if WITH_FP_REG == 1            // 含浮点寄存器的trapframe
    // TODO: 添加保存浮点寄存器
#else
    /// `STF_MSTATUS`宏是`mstatus`寄存器在`strapframe_t`结构体中的字节偏移量
    #define STF_MSTATUS      265
#endif




/* ------------------------------  ktrapframe_t 相关寄存器 ------------------------------ */

#if WITH_FP_REG == 1            // 含浮点寄存器的trapframe
    /// `KTF_SIZE`宏是`ktrapframe_t`结构体的总字节长度
    #define KTF_SIZE 
#else                            // 不含浮点寄存器的trapframe
    /// `KTF_SIZE`宏是`ktrapframe_t`结构体的总字节长度
    #define KTF_SIZE    288
#endif

/// `KTF_SEPC`宏是`sepc`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_SEPC    0
/// `KTF_RA`宏是`ra`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_RA      8
/// `KTF_SP`宏是`sp`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_SP      16
/// `KTF_GP`宏是`gp`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_GP      24
/// `KTF_TP`宏是`tp`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_TP      32
/// `KTF_T0`宏是`t0`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T0      40
/// `KTF_T1`宏是`t1`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T1      48
/// `KTF_T2`宏是`t2`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T2      56
// S0和FP是一个寄存器
/// `KTF_FP`宏是`fp`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_FP      64
/// `KTF_S0`宏是`s0`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S0      64
/// `KTF_S1`宏是`s1`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S1      72
/// `KTF_A0`宏是`a0`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A0      80
/// `KTF_A1`宏是`a1`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A1      88
/// `KTF_A2`宏是`a2`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A2      96
/// `KTF_A3`宏是`a3`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A3      104
/// `KTF_A4`宏是`a4`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A4      112
/// `KTF_A5`宏是`a5`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A5      120
/// `KTF_A6`宏是`a6`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A6      128
/// `KTF_A7`宏是`a7`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_A7      136
/// `KTF_S2`宏是`s2`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S2      144
/// `KTF_S3`宏是`s3`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S3      152
/// `KTF_S4`宏是`s4`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S4      160
/// `KTF_S5`宏是`s5`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S5      168
/// `KTF_S6`宏是`s6`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S6      176
/// `KTF_S7`宏是`s7`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S7      184
/// `KTF_S8`宏是`s8`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S8      192
/// `KTF_S9`宏是`s9`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S9      200
/// `KTF_S10`宏是`s10`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S10     208
/// `KTF_S11`宏是`s11`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_S11     216
/// `KTF_T3`宏是`t3`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T3      224
/// `KTF_T4`宏是`t4`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T4      232
/// `KTF_t5`宏是`t5`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T5      240
/// `KTF_t6`宏是`t6`寄存器在`ktrapframe_t`结构体中的字节偏移量
#define KTF_T6      248

#if WITH_FP_REG == 1            // 含浮点寄存器的trapframe
    // TODO: 添加保存浮点寄存器
#else
    /// `KTF_SSTATUS`宏是`sstatus`寄存器在`ktrapframe_t`结构体中的字节偏移量
    #define KTF_SSTATUS     256
    /// `KTF_SBADADDR`宏是`sbadaddr`寄存器在`ktrapframe_t`结构体中的字节偏移量
    #define KTF_SBADADDR    264
    /// `KTF_SCAUSE`宏是`scause`寄存器在`ktrapframe_t`结构体中的字节偏移量
    #define KTF_SCAUSE      272
    /// `KTF_ORIGIN_A0`宏是`origin_a0`在`ktrapframe_t`结构体中的字节偏移量
    #define KTF_ORIGIN_A0   280
#endif


#endif