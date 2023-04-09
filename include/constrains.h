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


/// 内核的跳转地址, 在`kernel.ld`中定义
#define KERNEL_JUMP_ADDR 0x80200000

/// `kprintf`和`uprintf`可以输出的字符串最大长度
#define PRINTF_STRING_SIZE          1024

/// `UART`设备的波特率
#define UART_BAUD_RATE              115200


#endif