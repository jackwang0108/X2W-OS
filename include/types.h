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

/// 分隔符
#define DELIMITER "============================================================\n"

/// `NULL` 被定义为指向内存`0x0000000000000000`处的`void`指针, 由于在`QEMU`的`Memory Map`中该地址不在内存中, 因此访问该地址最终将会导致报错
#define NULL ((void *)0)

// 无符号常量定义, 由`gcc -mabi`参数设定, 参考`Makefile`中的`GCC`编译选项`CFALSG`, https://www.ibm.com/docs/en/zos/2.3.0?topic=environments-ilp32-lp64-data-models-data-type-sizes
typedef unsigned char       uint8_t;            ///< `LP64`中一个 `char` 宽度为 `8     bit`
typedef unsigned short      uint16_t;           ///< `LP64`中一个 `short`宽度为 `16    bit`
typedef unsigned int        uint32_t;           ///< `LP64`中一个 `int`  宽度为 `32    bit`
typedef unsigned long       uint64_t;           ///< `LP64`中一个 `long` 宽度为 `64    bit`

// 符号常量定义, 由`gcc -mabi`参数设定, 参考`Makefile`中的`GCC`编译选项`CFLAGS`, https://www.ibm.com/docs/en/zos/2.3.0?topic=environments-ilp32-lp64-data-models-data-type-sizes
typedef char       int8_t;            ///< `LP64`中一个 `char` 宽度为 `8     bit`
typedef short      int16_t;           ///< `LP64`中一个 `short`宽度为 `16    bit`
typedef int        int32_t;           ///< `LP64`中一个 `int`  宽度为 `32    bit`
typedef long       int64_t;           ///< `LP64`中一个 `long` 宽度为 `64    bit`

/// 布尔常量定义
typedef unsigned char Bool;
enum {
    False   = 0,            ///< 等价于`unsigned int false = 0`
    True    = 1             ///< 等价于`unsigned int true = 1`
};

/// 计数类型定义
typedef uint64_t size_t;

/// 字节类型定义
typedef uint8_t byte;

/// 偏移类型定义
typedef int64_t offset_t;

/// 地址类型
typedef uint64_t addr_t;

/// 整型寄存器类型
typedef uint64_t ireg_t;

/// 浮点寄存器类型
typedef uint64_t freg_t;

/// `ARRAY_SIZE`宏函数用于计算数组`array`中的元素个数, 返回值类型为`size_t`
#define ARRAY_SIZE(array)  ((size_t)(sizeof(array) / sizeof((array)[0])))


/**
 * @brief `UNUSED`宏用于通知`GCC`/`Clang`寄存器被修饰的变量可能没有被使用, 从而在编译的时候静默警告
 * 
 * @note `UNUSED`宏本质上是借助了`GCC`/`Clang`的`__attribute__`关键词实现的, 使用的标记(annotation)是: `unused`
 * 
 * @note 参考: https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Variable-Attributes.html
 */
#define UNUSED __attribute__((unused))

/**
 * @brief `NO_RETURN`宏用于通知`GCC`/`Clang`编译器被修饰的函数不会返回, 帮助编译器静态分析用
 * 
 * @note `NO_RETURN`宏本质上是借助了`GCC`/`Clang`的`__attribute__`关键词实现的, 使用的标记(annotation)是: `noreturn`
 * 
 * @note 参考: https://stackoverflow.com/questions/25408795/tell-gcc-that-a-function-call-will-not-return
 */
#define NO_RETURN __attribute__((noreturn))

/**
 * @brief `DEPRECATED`宏用于通知`GCC`/`Clang`编译器被修饰的对象已经弃用, 帮助编译器静态分析用
 * 
 * @note `NO_RETURN`宏本质上是借助了`GCC`/`Clang`的`__attribute__`关键词实现的, 使用的标记(annotation)是: `deprecated`
 * 
 * @note 参考: https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/GCC/__attribute__/deprecated/index
 */
#define DEPRECATED __attribute__((deprecated))

/**
 * @brief `UNREACHABLE`宏用于通知`GCC`/`Clang`编译器不会运行到此处, 帮助编译器静态分析用
 * 
 * @note `NO_RETURN`宏本质上是借助了`GCC`/`Clang`内建的`__builtin_unreachable()`函数实现的
 * 
 * @note 参考: https://stackoverflow.com/questions/25408795/tell-gcc-that-a-function-call-will-not-return
 */
#define UNREACHABLE __builtin_unreachable()

#endif