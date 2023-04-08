/**
 * @file stdlib.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdlib.h`提供一系列帮助函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @todo
 * 1. 需要测试itoa函数
 * 
 * @warning 为了避免编译时编译器使用系统的`stdlib.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STDLIB_H
#define __INCLUDE_STDLIB_H

#include "types.h"

/**
 * @brief itoa (integer to ascii) 用于将`uint64_t`数字转换为`char*`数字, 并将其存入`*buf_ptr_addr`指向的内存中
 * 
 * @param integer 需要转为字符串形式的整数
 * @param buf_ptr_addr **字符串指针/指向字符数组的指针**的地址
 * @param base 转换进制, 可以任意指定. 例如:
 * - `base = 2`: 将`integer`转换为二进制形式的字符串数字, 例如 6 = "110"
 * - `base = 8`: 将`integer`转换为二进制形式的字符串数字, 例如 23 = "27"
 * - `base = 16`: 将`integer`转换为二进制形式的字符串数字, 例如 47 = "2F"
 * 
 * @note 因为是使用递归完成的转换, 因此多次递归需要共享字符数组, 因此需要字符数组的二级指针
 */
void itoa(uint64_t integer, char** buf_ptr_addr, uint8_t base);

#endif