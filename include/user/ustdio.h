/**
 * @file ustdio.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ustdio.h`为用户程序提供了标准输入输出函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning 目前库并没有完成需要在完成系统调用后完成该库
 * 
 * @warning `ustdio`库仅供用户程序使用, 内核请使用`kstdio`库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_USTDIO_H
#define __INCLUDE_USTDIO_H

#include "stdfmt.h"

/**
 * @brief uprintf用于格式化输出字符串
 * 
 * @param format 含格式控制字符的格式字符串
 * @param ... 可变参数列表
 * @return size_t 输出的字符串中的字符数
 * 
 * @note `uprintf`函数可以输出的字符串最大长度由`constrains.h`中的`PRINTF_STRING_SIZE`宏常量的值决定
 * 
 * @note 
 * 格式控制字符: `%[flag][width][precision][length]<type>`. 其中:
 *  - `type`字段必须存在, 其余字段均可选
 * 
 * 关于各个字段的说明:
 *  - `flag`字段:
 *      - `-`: 左对齐输出, 默认是右对齐
 *      - `+`: 输出正数前添加加号, 默认不添加
 *      - `#`: 当`type`为`b`, `o`, `x`, `X`时, 添加前缀`0b`, `0`或者`0x`
 *      - `0`: 使用`0`填充对齐, 默认使用空格填充对齐
 *  - `width`字段:
 *      - `数字`: 设置对齐的宽度, 例如: `printf("%5d", 100)` 将会输出 `  100`
 *      - `*:` 动态参数, 由后续的数字指定, 例如: `printf("%*d", 5, 10)` 等价于 printf("%5d", 10)`
 *  - `precision`字段:
 *      - `.数字`: 设置输出的位数, 例如: `printf("%.2d", 111)` 将会输出 `11`
 *      - `.*`: 动态参数, 由后续的数字指定, 例如: `printf("%.*d", 2, 111)` 等价于 printf("%.2d", 111)`
 *  - `length`字段:
 *      - `h`: 输出一个16位宽度的整数, 即: `short`, `unsigned short`, `int16_t`, `uint16_t`
 *      - `l`: 输出一个64位宽度的整数, 即: `long`, `unsigned long`, `int64_t`, `uint64_t`
 *      - `ll`: 输出一个64位宽度的整数, 即: `long long`, `unsigned long long`
 *      - PS: RISC-V LP64中long long和long都是64位
 *  - `type`字段:
 *      - `%`: 输出一个`%`
 *      - `p`: 输出一个指针, 需要转换为(void*)
 *      - `c`: 输出一个字符
 *      - `s`: 输出一个`\0`结尾的字符串
 *      - `b`: 以二进制输出一个整数, 无符号
 *      - `o`: 以八进制输出一个整数, 无符号
 *      - `i`: 以十进制输出一个整数, 有符号
 *      - `d`: 以十进制输出一个整数, 有符号
 *      - `u`: 以十进制输出一个整数, 无符号
 *      - `x`: 以十六进制输出一个整数, 小写字母, 无符号
 *      - `X`: 以十六进制输出一个整数, 大写字母, 无符号
 *      - `n`: 到此处已经输出的字符的数量, 不会输出文本, 而是将值保存到地址中. 参考: https://blog.csdn.net/zsx0728/article/details/118100389
 * 
 * @warning `uprintf`函数仅供用户程序使用, 内核程序请使用`kprintf`函数
 * 
 * @warning 目前`uprintf`并没有实现输出, 需要在实现系统调用后再实现该函数
 */
size_t uprintf(const char* format, ...);

#endif