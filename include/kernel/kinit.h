/**
 * @file init.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `kinit.h`是内核的初始化模块, 对内核的各个模块进行了初始化
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_KINIT_H
#define __INCLUDE_KERNEL_KINIT_H

#include "types.h"
#include "constrains.h"


/**
 * @brief `kinit_all`函数对内核的各个模块进行初始化
 * 
 * @note `kinit_all`的初始化顺序:
 *  1. `uart_init`: 初始化`uart`设备. 已经交给`SBI`来初始化了
 */
void kinit_all(void);


#endif