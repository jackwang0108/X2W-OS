/**
 * @file kdebug.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kdebug.h`提供内核态的断言函数
 * @version 0.1
 * @date 2023-04-13
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */
#ifndef __INCLUDE_KDEBUG_H
#define __INCLUDE_KDEBUG_H

// 设置使用内核态的输出函数
#undef USER_LIB
#define KERNEL_LIB

#include "assert.h"

#endif