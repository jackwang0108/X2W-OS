/**
 * @file stfoffset.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stfoffset.h`中定义了`strap.h`中的`strapframe_t`结构体各个成员变量的字节偏移数
 * @version 0.1
 * @date 2023-04-16
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_SBI_STFOFFSET_H
#define __INCLUDE_SBI_STFOFFSET_H

#include "constrains.h"

#if WITH_FP_REG == 1            // 含浮点寄存器的trap frame
    #define STF_SIZE 
#else                            // 不含浮点寄存器的trap frame
    #define STF_SIZE    264
#endif

#define STF_MEPC    0
#define STF_RA      8
#define STF_SP      16
#define STF_GP      24
#define STF_TP      32
#define STF_T0      40
#define STF_T1      48
#define STF_T2      56
// S0和FP是一个寄存器
#define STF_FP      64
#define STF_S0      64
#define STF_S1      72
#define STF_A0      80
#define STF_A1      88
#define STF_A2      96
#define STF_A3      104
#define STF_A4      112
#define STF_A5      120
#define STF_A6      128
#define STF_A7      136
#define STF_S2      144
#define STF_S3      152
#define STF_S4      160
#define STF_S5      168
#define STF_S6      176
#define STF_S7      184
#define STF_S8      192
#define STF_S9      200
#define STF_S10     208
#define STF_S11     216
#define STF_T3      224
#define STF_T4      232
#define STF_T5      240
#define STF_T6      248


#if WITH_FP_REG == 1            // 含浮点寄存器的trap frame
    // TODO: 添加保存浮点寄存器
#else
    #define STF_MSTATUS      265
#endif


#endif