/**
 * @file csr.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `csr.h`提供了读写`RISC-V`体系结构中的`CSR`寄存器(`Control and Status`)的函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @note 参考RISC-V架构手册: 
 * 1. https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf
 * 2. https://github.com/riscv/riscv-isa-manual/releases/download/Priv-v1.12/riscv-privileged-20211203.pdf
 * 
 * @note 一些有用的博客:
 * 1. https://dingfen.github.io/risc-v/2020/08/05/riscv-privileged.html#%E4%B8%AD%E6%96%AD%E4%B8%8E%E5%BC%82%E5%B8%B8
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_ASM_CSR_H
#define __INCLUDE_ASM_CSR_H

#include "types.h"

/* ------------------------------ 宏定义 ------------------------------ */

/* ----- 特权级定义 ----- */
/// 用户态特权级
#define PRIVILEGE_U     0UL
/// 监管态特权级
#define PRIVILEGE_S     1UL
/// 机器态特权级
#define PRIVILEGE_M     3UL

/* ----- 中断使能 ----- */
/// 中断开启
#define INTR_DS         0UL
/// 中断关闭
#define INTR_EN         1UL


/* ----- mstatus寄存器 ----- */

// MPP域
/// `MPP`域在`mstatus`寄存器的偏移
#define MSTATUS_MPP_SHIFT           11
/// `MPP`域选择子
#define MSTATUS_MPP                 (3UL << MSTATUS_MPP_SHIFT)
/// `MPP = 0b00`, 中断发生前的特权级为U
#define MSTATUS_MPP_U               (PRIVILEGE_U << MSTATUS_MPP_SHIFT)
/// `MPP = 0b01`, 中断发生前的特权级为S
#define MSTATUS_MPP_S               (PRIVILEGE_S << MSTATUS_MPP_SHIFT)
/// `MPP = 0b11`, 中断发生前的特权级为M
#define MSTATUS_MPP_M               (PRIVILEGE_M << MSTATUS_MPP_SHIFT)


// MPIE域
/// `MPIE`域在`mstatus`寄存器的偏移
#define MSTATUS_MPIE_SHIFT          7
/// `MPIE`域选择子
#define MSTATUS_MPIE                (1UL << MSTATUS_MPIE_SHIFT)
/// `MPIE = 0`, 中断发生前中断关闭
#define MSTATUS_MPIE_DS             (INTR_DS << MSTATUS_MPIE_SHIFT)
/// `MPIE = 1`, 中断发生前中断开启
#define MSTATUS_MPIE_EN             (INTR_EN << MSTATUS_MPIE_SHIFT)


/* ----- mtvec寄存器 ----- */

/// `MTVEC_TRAP_DIRECT`宏表示`mtvec`寄存器处于直接模式
#define MTVEC_TRAP_DIRECT                 (0b00)
/// `MTVEC_TRAP_INDIRECT`宏表示`mtvec`寄存器处于间接模式
#define MTVEC_TRAP_INDIRECT               (0b01)


/* ----- mcause寄存器 ----- */

#define MCAUSE_INTERRUPT_FLAG               (1UL << 63)
#define MCAUSE_MISALIGNED_FETCH             0x00
#define MCAUSE_FETCH_ACCESS                 0x01
#define MCAUSE_ILLEGAL_INSTRUCTION          0x02
#define MCAUSE_BREAKPOINT                   0x03
#define MCAUSE_MISALIGNED_LOAD              0x04
#define MCAUSE_LOAD_ACCESS                  0x05
#define MCAUSE_MISALIGNED_STORE             0x06
#define MCAUSE_STORE_ACCESS                 0x07
#define MCAUSE_USER_ECALL                   0x08
#define MCAUSE_SUPERVISOR_ECALL             0x09
#define MCAUSE_VIRTUAL_SUPERVISOR_ECALL     0x0A
#define MCAUSE_MACHINE_ECALL                0x0B
#define MCAUSE_FETCH_PAGE_FAULT             0x0C
#define MCAUSE_LOAD_PAGE_FAULT              0x0D
#define MCAUSE_STORE_PAGE_FAULT             0x0F
#define MCAUSE_FETCH_GUEST_PAGE_FAULT       0x14
#define MCAUSE_LOAD_GUEST_PAGE_FAULT        0x15
#define MCAUSE_VIRTUAL_INST_FAULT           0x16
#define MCAUSE_STORE_GUEST_PAGE_FAULT       0x17
/* ----- mtvec寄存器 ----- */

/// `TRAP_DIRECT`宏将`mtvec`寄存器的`mode`字段设置为0, 即直接模式
#define TRAP_DIRECT         (0b00)
/// `TRAP_INDIRECT`宏将`mtvec`寄存器的`mode`字段设置为1, 即间接模式
#define TRAP_INDIRECT       (0b01)


/* ----- mcause寄存器 ----- */
/// `IS_INTERRUPT`宏用于判断当前陷入是否为中断
#define IS_INTERRUPT        (1 << 63)
/// `IS_EXCEPTION`宏用于判断当前陷入是否为异常
#define IS_EXCEPTION        (0 << 64)


/* ------------------------------ 宏函数 ------------------------------ */

/**
 * @brief `SET_FIELD`宏用于将`origin`中`which`所指定的`field`的值设置为`fieldval`
 * 
 * @param origin 初始值
 * @param which 设置的`field`
 * @param fieldval `field`将被设置的值
 * 
 * @note 需要注意的点:
 * - `x & ~mask`: 将x的非`mask`清0
 * - `x & ~(x - 1)`: 用于一个数的二进制表示中最右边的非零位（也称作最低位的1），并将其它位都置为0
 * - 而后利用这个最右边的非零位表示需要左移多少位, 这里是用乘法实现的左移
 * - 最后在和mask对应位清0之后的原值与就能得到最终需要的值
 * 
 * 一个详细的例子`origin = 0b1001_0001`, `which = 0b0011_0000`, `fieldval = 0b11`则:
 * - `which & ~(which - 1) = 0b0001_0000`
 * - `fieldval * (which & ~(which - 1)) = 0b0011_0000`
 * - `origin & !(which) = 0b1011_0001`
 * - `... | ... = 0b1000_0000`
 */
#define SET_FIELD(origin, which, fieldval) \
    (((origin) & ~(which)) | ((fieldval) * ((which) & ~((which) - 1))))

/**
 * @brief `read_csr`用于读取的`csr`寄存器的值并返回
 * 
 * @param csr 要读取的`CSR`寄存器名
 * 
 * @return uint64_t 读取得到的`csr`寄存器的值
 * 
 * @note 通常情况下，在内联汇编代码块的最后一条语句中将需要返回的值存储在一个特定的变量中，然后该变量将被作为代码块的返回值返回给调用者。在GCC扩展中，这个变量通常被命名为__v（两个下划线 + v），并且在代码块的最后一行写上__v;语句来表示将其作为返回值返回给调用者。
 * @note 写入到`CSR`寄存器的变量必须要存在通用寄存器中
 * @note `read_csr`中使用`#`将`csr`转为字符串, 而后利用`C`语言的字符串拼接得到最终的汇编语句
 */
#define read_csr(csr) ({                            \
    register unsigned long __v;                     \
    asm volatile(                                   \
        "csrr %0, " #csr                            \
        : "=r" (__v)                                \
        :                                           \
        : "memory"                                  \
    );                                              \
    __v;                                            \
})

/**
 * @brief `write_csr`用于将`csr`寄存器的值设置为`value`
 * 
 * @param csr 要设置的`CSR`寄存器名
 * @param value 要设置的值
 * 
 * @note `rK`约束参考, `r`是简单约束, `K`是`RISC-V`的机器限定约束:
 * 1. https://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html
 * 2. https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
 */
#define write_csr(csr, value) ({                    \
    unsigned long __v = (unsigned long) (value);    \
    __asm__ __volatile__(                           \
        "csrw " #csr ", %0"                         \
        :                                           \
        : "rK" (__v)                                \
        : "memory"                                  \
    );                                              \
})







#endif