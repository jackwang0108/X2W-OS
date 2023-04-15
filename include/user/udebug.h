/**
 * @file udebug.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `udebug.h`提供用户态态的断言函数
 * @version 0.1
 * @date 2023-04-13
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */
#ifndef __INCLUDE_UDEBUG_H
#define __INCLUDE_UDEBUG_H

// 设置使用用户态的输出函数
#undef KERNEL_LIB
#define USER_LIB

#include "assert.h"

#endif