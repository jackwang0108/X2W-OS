/**
 * @file types.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `types.h`中定义了`X2W-OS`的内建类型
 * @version 0.1
 * @date 2023-04-08
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_TYPES_H
#define __INCLUDE_TYPES_H

#include "constrains.h"

/// `NULL` 被定义为指向内存`0x0000000000000000`处的`void`指针, 由于在`QEMU`的`Memory Map`中该地址不在内存中, 因此访问该地址最终将会导致报错
#define NULL ((void *)0)

// 无符号常量定义, 由`gcc -mabi`参数设定, 参考`Makefile`中的`GCC`编译选项`CFALSG`
typedef unsigned char       uint8_t;            ///< `LP64`中一个 `char` 宽度为 `8     bit`
typedef unsigned short      uint16_t;           ///< `LP64`中一个 `short`宽度为 `16    bit`
typedef unsigned int        uint32_t;           ///< `LP64`中一个 `int`  宽度为 `32    bit`
typedef unsigned long       uint64_t;           ///< `LP64`中一个 `long` 宽度为 `64    bit`

// 符号常量定义, 由`gcc -mabi`参数设定, 参考`Makefile`中的`GCC`编译选项`CFLAGS`
typedef char       int8_t;            ///< `LP64`中一个 `char` 宽度为 `8     bit`
typedef short      int16_t;           ///< `LP64`中一个 `short`宽度为 `16    bit`
typedef int        int32_t;           ///< `LP64`中一个 `int`  宽度为 `32    bit`
typedef long       int64_t;           ///< `LP64`中一个 `long` 宽度为 `64    bit`

/// 布尔常量定义
typedef unsigned char Bool;
enum {
    False   = 0,            ///< 等价于`unsigned int False = 0`
    True    = 1             ///< 等价于`unsigned int True = 1`
};

/// 计数常量定义
typedef unsigned long size_t;

/// `ARRAY_SIZE`宏函数用于计算数组`array`中的元素个数, 返回值类型为`size_t`
#define ARRAY_SIZE(array)  ((size_t)(sizeof(array) / sizeof((array)[0])))

#endif