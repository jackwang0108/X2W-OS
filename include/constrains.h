/**
 * @file constrains.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `constrain.h`定义了内核的一系列常数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */


#ifndef __INCLUDE_CONSTRAINS_H
#define __INCLUDE_CONSTRAINS_H

/**
 * @brief 内核是否处于DEBUG模式, 若:
 * - DEBUG = 0, 不会运行测试函数
 * - DEBUG = 1, 运行测试函数
 */
#define DEBUG 1

/**
 * @brief CPU是否具有浮点寄存器, 若
 * - WITH_FP_REG = 0, CPU 不具有浮点寄存器
 * - WITH_FP_REG = 1, CPU 具有浮点寄存器
 */
#define WITH_FP_REG 0

/// 内核的跳转地址, 在`kernel.ld`中定义
#define KERNEL_JUMP_ADDR 0x80200000

/// `kprintf`和`uprintf`可以输出的字符串最大长度
#define PRINTF_STRING_SIZE          1024

/// `UART`设备的波特率
#define UART_BAUD_RATE              115200

/// 最大测试函数的数量
#define MAX_TEST_FUNCTION_NUM       20

/// 中断/异常处理函数信息最大数量
#define MAX_INTR_EXCP_INFO_NUM      64

#endif