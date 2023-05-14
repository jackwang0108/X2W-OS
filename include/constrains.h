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
 * @brief 内核是否处于`DEBUG`模式, 若:
 * - `DEBUG = 0`, 不会运行测试函数
 * - `DEBUG = 1`, 运行测试函数
 */
#define DEBUG 1

/**
 * @brief 当前系统`CPU`核心数
 */
#define MAX_CPU_NUM 1

/**
 * @brief `CPU`是否具有浮点寄存器, 若
 * - `WITH_FP_REG = 0`, `CPU` 不具有浮点寄存器
 * - `WITH_FP_REG = 1`, `CPU` 具有浮点寄存器
 */
#define WITH_FP_REG 0

/**
 * @brief 当前`CPU`是否支持`rdtime`指令, 若
 * - `RDTIME_SUPPORT = 0`, `CPU` 不支持`rdtime`指令
 * - `RDTIME_SUPPORT = 1`, `CPU` 支持`rdtime`指令
 */
#define RDTIME_SUPPORT 0

/// 内核的跳转地址, 在`kernel.ld`中定义
#define KERNEL_JUMP_ADDR 0x80200000

/// `kprintf`和`uprintf`可以输出的字符串最大长度
#define PRINTF_STRING_SIZE          1024

/// `UART`设备的波特率
#define UART_BAUD_RATE              115200

/// `CLINT`中断控制器的时钟中断频率, 目前`1000次/秒`
#define CLINT_TIMER_FREQUENCY_HZ          1000

/// `CLINT`中断控制器的时钟中断的默认的时钟频率
#define CLINT_TIMER_BASE_FRQENCY          10000000

/// `PLIC`中断控制器最大支持的中断数量
#define PLIC_MAX_INTERRUPTS_NUM     53

/// 最大测试函数的数量
#define MAX_TEST_FUNCTION_NUM       20

/// 中断/异常处理函数信息最大数量
#define MAX_INTR_EXCP_INFO_NUM      64

#endif