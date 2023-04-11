/**
 * @file uart.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `uart.h`提供了包括初始化、输出字符串等函数在内的`uart`功能
 * @version 0.1
 * @date 2023-04-08
 * 
 * @note `QEMU`默认模拟的`UART`设备处理芯片是`AXI UART 16550`
 * @note 关于`AXI UART 16550`芯片介绍: https://wenku.baidu.com/view/6f7a176048d7c1c708a14521.html?_wkts_=1681021571718
 * @note 关于`AXI UART 16550`相关常量, 参考`AXI UART 16550`编程手册(`Programming Table`): http://byterunner.com/16550.html
 * 
 * @warning 目前使用轮询方式使用`UART`设备, 没有实现中断方式
 * @warning `uart.h`仅提供`UART`相关功能, `AXI UART 16550`相关常量的定义在`asm/uart.h`中
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_UART_H
#define __INCLUDE_UART_H

#include "io.h"
#include "types.h"
#include "asm/uart.h"
#include "constrains.h"



/**
 * @brief `uart_init`初始化`UART`设备.
 * 
 * @note `uart_init`将`UART`设备的波特率设置为`constrains.h`中`UART_BAUD_RATE`所定义的值
 * @note `QEMU`默认模拟的UART处理芯片为`AXI UART 16550`芯片
 * 
 */
void uart_init(void);

/**
 * @brief `uart_get`从`UART`设备接受输入
 * 
 * @warning 目前没有实现
 */
void uart_get(void);

/**
 * @brief `uart_put`向`UART`设备发送一个字符
 * 
 * @param chr 要发送的字符
 * 
 * @warning 目前以轮询方式发送
 */
void uart_put(char chr);

/**
 * @brief `uart_puts`向`UART`设备发送字符串
 * 
 * @param string 要发送的字符串
 * 
 * @note `uart_puts`调用`uart_put`发送
 */
void uart_puts(const char *string);


/**
 * @brief `test_uart`是`uart`模块的测试函数
 * @return int 测试正常则返回0
 */
int test_uart(void);

#endif
