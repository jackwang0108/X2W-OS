/**
 * @file init.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `init.h`是内核的初始化模块, 对系统的各个模块进行了初始化
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_INIT_H
#define __INCLUDE_INIT_H

#include "uart.h"
#include "types.h"
#include "constrains.h"


/**
 * @brief `init_all`函数对内核的各个模块进行初始化
 * 
 * @note `init_all`的初始化顺序:
 *  1. `uart_init`: 初始化`uart`设备
 */
void init_all(void);


#endif