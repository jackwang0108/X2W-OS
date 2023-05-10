/**
 * @file sbi.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `sbi.h`为操作系统提供了SBI调用(M模式下的功能接口)
 * @version 0.1
 * @date 2023-04-15
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_SBI_SBI_H
#define __INCLUDE_SBI_SBI_H

#include "types.h"
#include "constrains.h"

/// @brief `sbicall_id_t`内定义了`SBI`调用号
typedef enum __sbicall_id_t {
    // `SBICALL_SETTIMER = 0x00`, `SBI`提供的`timer`服务
    SBICALL_SETTIMER,
    // `SBICALL_CONSOLE_PUTCHAR = 0x01`, `SBI`提供的字符输出服务
    SBICALL_CONSOLE_PUTCHAR,
    // `SBICALL_CONSOLE_PUTSTR = 0x02`, `SBI`提供的字符串输出服务
    SBICALL_CONSOLE_PUTSTR,
    // `SBICALL_CONSOLE_GETCHAR = 0x03`, `SBI`提供的字符获取服务
    SBICALL_CONSOLE_GETCHAR
} sbicall_id_t;


/**
 * @brief `_SBICALL`宏函数将会触发`ecall`异常, 而后跳转到`strap_enter`函数中运行, 该函数定义在`strap_entry.S`中
 * 
 * @note 该宏函数需要注意的是:
 *  1. 目前最大支持三个参数, 分别使用a0, a1, a2寄存器传递
 *  2. `ecall`调用号保存在`a7`寄存器中
 *  3. 返回至保存在a0寄存器中
 */
#define _SBICALL(scall_id, arg0, arg1, arg2) ({             \
    register ireg_t a0 asm ("a0") = (ireg_t)(arg0);         \
    register ireg_t a1 asm ("a1") = (ireg_t)(arg1);         \
    register ireg_t a2 asm ("a2") = (ireg_t)(arg2);         \
    register ireg_t a7 asm ("a7") = (ireg_t)(scall_id);     \
    asm volatile (                                          \
        "ecall"                                             \
        : "+r"  (a0)                                        \
        : "r"   (a1),   "r"   (a2),   "r"   (a7)            \
        : "memory"                                          \
    );                                                      \
    a0;                                                     \
})

/// @brief `_SBICALL0`为接受0个参数的`SBI`调用
#define _SBICALL0(scall_id)                     _SBICALL(scall_id, 0, 0, 0)
/// @brief `_SBICALL1`为接受1个参数的`SBI`调用
#define _SBICALL1(scall_id, arg0)               _SBICALL(scall_id, arg0, 0, 0)
/// @brief `_SBICALL2`为接受2个参数的`SBI`调用
#define _SBICALL2(scall_id, arg0, arg1)         _SBICALL(scall_id, arg0, arg1, 0)
/// @brief `_SBICALL3`为接受3个参数的`SBI`调用
#define _SBICALL3(scall_id, arg0, arg1, arg2)   _SBICALL(scall_id, arg0, arg1, arg2)

/**
 * @brief `sbi_settimer`用于设置时钟, 即设置`mtimecmp`寄存器的值为`next_ticks`
 * 
 * @param next_ticks 时钟的值
 */
static inline void sbi_settimer(uint64_t next_ticks){
    _SBICALL1(SBICALL_SETTIMER, next_ticks);
}

/**
 * @brief `sbi_putc`用于向终端输出一个字符
 * 
 * @param c 需要输出的字符
 */
static inline void sbi_putc(char c){
    _SBICALL1(SBICALL_CONSOLE_PUTCHAR, c);
}

/**
 * @brief `sbi_puts`用于向字符输出一个字符串
 * 
 * @param str 需要输出的0-terminated字符串
 * 
 * @warning `sbi_puts`不会对str是否是以'\0'结尾进行检查
 */
static inline void sbi_puts(char *str){
    _SBICALL1(SBICALL_CONSOLE_PUTSTR, str);
}


#endif