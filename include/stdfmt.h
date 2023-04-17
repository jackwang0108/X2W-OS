/**
 * @file stdfmt.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdfmt.h`提供了格式化函数
 * @version 0.2
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
 * @brief `_scan_number`用于将存储在`str`中的字符串数字转换为`int`数字并存入`num`中
 * 
 * @param str 字符串形式的数字
 * @param num 转换后的数字将存入`num`中
 * @return const char* str第一个非字符数字
 * 
 * @note `_scan_number`将在第一个非字符数字前停下, 即:
 *  - 若`str = "123TEST"`, 则运行后`num = 123, *return = 'T'`
 *  - 若`str = "12EST456"`, 则运行后`num = 12, *return = 'E'`
 *  - 若`str = "ST456"`, 则运行后`num = 0, *return = 'S'`
 */
const char* _scan_number(const char *str, int* num);

/**
 * @brief `copy_number`用于将整形的数字`num`按照指定进制`base`转换为字符串数字, 而后按照指定的宽度`width`和精度`precision`复制到`str`中去
 * 
 * @param str 输出字符数组
 * @param num 整形数字
 * @param base 进制
 * @param width 对齐宽度
 * @param precision 输出位数
 * @param flag 格式控制字符
 * @return char* 复制后str的位置
 */
char *copy_number(char *str, unsigned long num, int base, int width, int precision, int flag);


/**
 * @brief `vsprintf`按照`format`中指定的格式将`ap`指向的可变参数列表的值复制到`str`中
 * 
 * @param str 接受输出的字符数组
 * @param format  含格式控制字符的字符串.
 * @param ap 可变参数列表
 * @return size_t 处理后的字符串(str)的长度
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
 */
size_t vsprintf(char* str, const char* format, va_list ap);

/**
 * @brief `sprintf`将格式字符串`format`的格式控制字符替换为可变参数并输出到`buf`中
 * 
 * @param buf 接受输出的字符数组
 * @param format 含格式控制字符的格式字符串
 * @param ... 可变参数列表
 * @return size_t 输出到buf字符数组中的字符个数
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
 */
size_t sprintf(char* buf, const char* format, ...);


#endif