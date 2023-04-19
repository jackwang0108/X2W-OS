/**
 * @file stdlib.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdlib.h`提供一系列帮助函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning 为了避免编译时编译器使用系统的`stdlib.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STDLIB_H
#define __INCLUDE_STDLIB_H

#include "types.h"


/**
 * @brief `offset`宏用于获得结构体`struct_type`中名为`member_name`成员的偏移量(以字节计算)
 * 
 * @param struct_type 结构体名
 * @param member_name 结构体中的成员名
 * 
 * 
 * 举例:
 * ```c
 * struct person_t {
 *      uint8_t age;
 *      uint32_t id;
 *      char name[10];
 * };
 * offset(person_t, age);        # return 0
 * offset(person_t, id);         # return 1
 * offset(person_t, name);       # return 5
 * ```
 * 
 */
#define offset(struct_type, member_name) \
    (size_t) (&((struct_type*)0)->member_name)

/**
 * @brief `member2struct`宏用于将指向结构体`struct_type`中的名为`member_name`的成员的指针转为指向`struct_type`的指针
 * 
 * @param struct_type 结构体名
 * @param member_name 结构体中的成员名
 * @param elem_ptr 指向结构体中`member_name`成员的指针
 * 
 * @return struct_type* 指向成员所在的结构体的指针
 * 
 * 举例:
 * ```c
 * struct person_t {
 *      uint8_t age;
 *      uint32_t id;
 *      char name[10];
 * };
 * 
 * person_t jack = {22, 2196113760, "Shihong Wang"};
 * uint32_t *id_ptr = &jack.id;
 * # 将指向成员的指针转换为指向结构体的指针
 * person_t *person_ptr = member2struct(person_t, id, id_ptr);
 * &jack == person_ptr ? true : false;          # return true
 * ```
 */
#define member2struct(struct_type, member_name, elem_ptr) \
    (struct_type*)((uint64_t)elem_ptr - offset(struct_type, member_name))


/**
 * @brief `itoa` (integer to ascii) 用于将`uint64_t`数字转换为`char*`数字, 并将其存入`*buf_ptr_addr`指向的内存中
 * 
 * @param integer 需要转为字符串形式的整数
 * @param buf_ptr_addr 指向接受输出的字符串的指针, 即字符数组名的地址
 * @param base 转换进制, 可以任意指定. 例如:
 * - `base = 2`: 将`integer`转换为二进制形式的字符串数字, 例如 `6 = "110"`
 * - `base = 8`: 将`integer`转换为二进制形式的字符串数字, 例如 `23 = "27"`
 * - `base = 16`: 将`integer`转换为二进制形式的字符串数字, 例如 `47 = "2F"`
 * 
 * @note 因为是使用递归完成的转换, 因此多次递归需要共享字符数组, 因此需要字符数组的二级指针
 */
void itoa(uint64_t integer, char** buf_ptr_addr, uint8_t base);

/**
 * @brief 'atoi函数实现字符串转整形数字
 * @param  str 需要转换的字符串
 * @return int64_t 转换得到的整形数字
 */
int64_t atoi(char* str);

#endif