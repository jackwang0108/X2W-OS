/**
 * @file stdarg.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdargs.h`提供了自定义的变参函数支持
 * @version 0.1
 * @date 2023-04-09
 * 
 * @note `GCC`调用时函数时通过栈传参, 因此参数在函数调用栈帧中依次排列. 所以变参宏的本质就是以字节的形式处理在栈中的依次排列的函数参数.
 * 
 * @todo
 * 1. 需要测试va_start, va_arg, va_end是否正常工作
 * 
 * @warning 为了避免编译时编译器使用系统的`stdarg.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STRARG_H
#define __INCLUDE_STRARG_H

#include "types.h"

/// 变参宏以字节形式处理在栈中的函数参数, 因此变参列表`va_list`的本质就是字节
typedef char* va_list;

/// 
/**
 * @brief `va_start`用于将参数指针`ap`(argument pointer)指向第一个固定参数`v`
 * 
 * @param ap 参数指针
 * @param v 函数的第一个参数
 */
#define va_start(ap, v) ap = (va_list) &v

/**
 * @brief `va_arg`用于将参数指针`ap`(argument pointer)指向栈中下一个类型为`t`的参数.
 * 
 * @param ap 参数指针
 * @param t 下一个参数的类型
 * 
 * @note 由于是`64位`机器, 因此每次参数指针`ap`向后(高地址)增加8个字节
 */
#define va_arg(ap, t) *((t*)(ap += 8))

/**
 * @brief `va_end`用于清除参数指针`ap`(argument pointer)
 * 
 * @param ap 参数指针
 */
#define va_end(ap) ap = NULL

#endif