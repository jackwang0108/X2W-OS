/**
 * @file stdarg.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdargs.h`提供了自定义的变参函数支持
 * @version 0.2
 * @date 2023-04-09
 * 
 * @note deprecated, 原因见下. `GCC`调用时函数时通过栈传参, 因此参数在函数调用栈帧中依次排列. 所以变参宏的本质就是以字节的形式处理在栈中的依次排列的函数参数.
 * 
 * @warning 自己原先写的`stdargs`有BUG, 因为:
 *  1. `x86-32` 是全部通过栈传参
 *  2. `RISC-V LP64`的abi不是按照栈传参的, 先用寄存器再用栈, 所以如果要自己实现的话, 需要操作寄存器和栈
 * 所以为了屏蔽不同的体系结构的`ABI`的差异, `GCC`使用了内建的`__builtin_va_list`, `__builtin_va_start()`和 `__builtin_va_end()`
 * 因此如果需要自己实现`RISV-C`的`stdargs.h`的话, 就得参考GCC源码中的`__builtin_va_list`, `__builtin_va_start()`和 `__builtin_va_end()`的`RISC-V`部分的实现
 * 所以综合考虑, 还是直接需要使用`GCC`内建的 `__builtin_va_list`, `__builtin_va_start()`和 `__builtin_va_end()`算了
 * 
 * @warning 所有我自己写的`stdargs.h`适用于`x86-32`, 已经全部加上了`_my_`的前缀
 * 
 * @note 参考: <path-to-riscv-gnu-toolchain-installation-path>/lib/gcc/riscv64-unknown-elf/12.2.0/include/stdarg.h 
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STRARG_H
#define __INCLUDE_STRARG_H

#include "types.h"

/**
 * @brief 变参列表类型, 本质就是一个指针, 因此也称为参数指针, 原理参考`GCC`编译器的传参原理
 * 
 * @note 使用`GCC`内建的`__builtin_va_list`作为变参列表类型
 */
typedef __builtin_va_list va_list;

/**
 * @brief `va_start`用于将参数指针`ap`(argument pointer)指向第一个固定参数`v`
 * 
 * @note 使用`GCC`内建的`__builtin_va_start`作为将变参列表指向变参列表的的第一个参数
 */
#define va_start(ap, v)	__builtin_va_start(ap, v)

/**
 * @brief `va_arg`用于将参数指针`ap`(argument pointer)指向栈中下一个类型为`t`的参数.
 * 
 * @note 关于类型t, 需要注意的是:
 * 1. 默认传入的最小单位是int, 因此char的调用方式为: `char varg_char = (unsigned char) va_arg(ap, int)`
 * 
 * @note 使用`GCC`内建的`__builtin_va_arg`作为将变参列表指向下一个类型为`t`的参数
 */
#define va_arg(ap, t)	__builtin_va_arg(ap, t)

/**
 * @brief `_my_va_end`用于清除参数指针`ap`(argument pointer)
 * 
 * @note 使用`GCC`内建的`__builtin_va_end`来清空变参列表
 */
#define va_end(ap)	__builtin_va_end(ap)



/* ------------------- 已经弃用的实现 -------------------- */

/**
 * @brief 变参宏以字节形式处理在栈中的函数参数, 因此变参列表`_my_va_list`的本质就是字节
 * 
 * @deprecated 自己写的`_my_va_list`只适用于`x86-32`, 因此已经弃用, 详细的原因参考文件的doc
 */
typedef char* _my_va_list;


/**
 * @brief `_my_va_start`用于将参数指针`ap`(argument pointer)指向第一个固定参数`v`
 * 
 * @param ap 参数指针
 * @param v 函数的第一个参数
 * 
 * @deprecated 自己写的`_my_va_list`只适用于`x86-32`, 因此已经弃用, 详细的原因参考文件的doc
 */
#define _my_va_start(ap, v) ap = (va_list) &v

/**
 * @brief `_my_va_arg`用于将参数指针`ap`(argument pointer)指向栈中下一个类型为`t`的参数.
 * 
 * @param ap 参数指针
 * @param t 下一个参数的类型
 * 
 * @note 由于是`64位`机器, 因此每次参数指针`ap`向后(高地址)增加8个字节
 * 
 * @deprecated 自己写的`_my_va_arg`只适用于`x86-32`, 因此已经弃用, 详细的原因参考文件的doc
 */
#define _my_va_arg(ap, t) *((t*)(ap += 8))

/**
 * @brief `_my_va_end`用于清除参数指针`ap`(argument pointer)
 * 
 * @param ap 参数指针
 * 
 * @deprecated 自己写的`_my_va_end`只适用于`x86-32`, 因此已经弃用, 详细的原因参考文件的doc
 */
#define _my_va_end(ap) ap = NULL


#endif