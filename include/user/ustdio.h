/**
 * @file ustdio.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ustdio.h`为用户程序提供了标准输入输出函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning 目前库并没有完成需要在完成系统调用后完成该库
 * 
 * @warning `ustdio`库仅供用户程序使用, 内核请使用`kstdio`库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_USTDIO_H
#define __INCLUDE_USTDIO_H

#include "stdfmt.h"

/**
 * @brief uprintf用于格式化输出字符串
 * 
 * @param format 含格式控制字符的格式字符串
 * @param ... 可变参数列表
 * @return size_t 输出的字符串中的字符数
 * 
 * @note `uprintf`函数可以输出的字符串最大长度由`constrains.h`中的`PRINTF_STRING_SIZE`宏常量的值决定
 * 
 * @warning `uprintf`函数仅供用户程序使用, 内核程序请使用`kprintf`函数
 * 
 * @warning 目前`uprintf`并没有实现输出, 需要在实现系统调用后再实现该函数
 */
size_t uprintf(const char* format, ...);

#endif