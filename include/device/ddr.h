/**
 * @file ddr.h
 * @author Zhuir Xiao (xzr3356142450@gmail.com)
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ddr.h`提供了内存相关的宏定义
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright Zhuiri Xiao, Shihong Wang (c) 2023 with GNU Public License V3.0
 */


#ifndef __INCLUDE_DEVICE_DDR_H
#define __INCLUDE_DEVICE_DDR_H

#include "constrains.h"

/// `DDR`设备在内存中映射的起始单元
#define DDR_BASE_ADDR           0x80000000UL

#define DDR_END_ADDR            (DDR_BASE_ADDR + MEMORY_TOTAL)

#endif