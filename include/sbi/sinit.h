/**
 * @file sinit.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kinit.h`是`SBI`的初始化模块, 对`SBI`的各个模块进行了初始化
 * @version 0.1
 * @date 2023-04-14
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_SBI_SINIT_H
#define __INCLUDE_SBI_SINIT_H

#include "types.h"
#include "constrains.h"

/**
 * @brief `sinit_all`函数对`SBI`的各个模块进行初始化
 * 
 * @note `sinit_all`的初始化顺序:
 *  1. 初始化 SBI 异常/中断处理模块
 *  2. 初始化 SBI Ecall 异常处理模块
 */
void sinit_all(void);

#endif