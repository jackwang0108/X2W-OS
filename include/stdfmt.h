/**
 * @file stdfmt.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdfmt.h`提供了格式化函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STDFMT_H
#define __INCLUDE_STDFMT_H

#include "types.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"


/**
 * @brief `vsprintf`按照`format`中指定的格式将`ap`指向的可变参数列表的值复制到`str`中
 * 
 * @param str 接受输出的字符数组
 * @param format  含格式控制字符的字符串. 目前支持的格式控制字符
 * - `%%`: 输出一个`%`
 * - `%c`: 输出一个字符
 * - `%s`: 输出一个'\0'结尾的字符串
 * - `%b`: 二进制输出一个正整数
 * - `%o`: 八进制输出一个正整数
 * - `%d`: 十进制输出一个整数
 * - `%x`: 十六进制输出一个正整数
 * @param ap 可变参数列表
 * @return size_t 处理后的字符串(str)的长度
 */
size_t vsprintf(char* str, const char* format, va_list ap);

/**
 * @brief `sprintf`将格式字符串`format`的格式控制字符替换为可变参数并输出到`buf`中
 * 
 * @param buf 接受输出的字符数组
 * @param format 含格式控制字符的格式字符串
 * @param ... 可变参数列表
 * @return size_t 输出到buf字符数组中的字符个数
 */
size_t sprintf(char* buf, const char* format, ...);


#endif