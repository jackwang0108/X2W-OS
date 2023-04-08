/**
 * @file kstdio.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kstdio.h`为内核提供了标准输入输出函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning `kstdio`库仅供内核使用, 用户程序请使用`ustdio`库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KSTDIO_H
#define __INCLUDE_KSTDIO_H

#include "uart.h"
#include "stdfmt.h"

/**
 * @brief kprintf用于格式化输出字符串
 * 
 * @param format 含格式控制字符的格式字符串
 * @param ... 可变参数列表
 * @return size_t 输出的字符串中的字符数
 * 
 * @note `kprintf`函数可以输出的字符串最大长度由`constrains.h`中的`PRINTF_STRING_SIZE`宏常量的值决定
 * 
 * @warning `kprintf`函数仅供内核使用, 用户程序请使用`uprintf`函数
 * 
 * @warning 目前`kprintf`最终向`uart`输出字符串
 */
size_t kprintf(const char* format, ...);

#endif