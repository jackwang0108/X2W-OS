/**
 * @file uart.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief uart.h提供了包括初始化、输出字符串等函数在内的uart功能实现
 * @version 0.1
 * @date 2023-04-08
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_UART_H
#define __INCLUDE_UART_H


void uart_init(void);
void uart_get(void);
void uart_put(char chr);
void uart_puts(char *string);


#endif
