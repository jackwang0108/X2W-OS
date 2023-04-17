/**
 * @file test.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test.h`是系统的测试模块, 为各个模块提供了测试注册函数, 并在初始化时进行测试
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */


#ifndef __INCLUDE_TEST_H
#define __INCLUDE_TEST_H

#include "types.h"


/**
 * @brief `test_func_pointer_t`测试函数指针类型
 * 
 * @note 测试函数的通用原型应该是`int test_XXXX(void)`, 例如:
 * - `uart`的测试函数`int test_uart(void)`
 * @note 函数名即为函数的地址/函数第一条指令的地址, 因此函数名就是一个函数指针, 例如:
 * - 函数原型是`int test_uart(void)`, 则`test_uart`就是指向`int (void)`的函数指针
 * @note 函数类型由函数的参数和返回值决定, 因此不同参数返回值的函数的函数类型不同, 因此不能放到数组中.
 *   但是函数指针都是一个指针, 因此可以放到数组中, 所以这里声明的是函数指针
 */
typedef int (*test_func_pointer_t)(void);


/**
 * @brief `register_test_func`用于注册测试函数, 测试函数模板: int test_XXX(void)
 * 
 * @param func 测试函数指针, 测试函数名即可
 */
void register_test_func(test_func_pointer_t func);


/**
 * @brief `test_all`函数运行所有测试函数
 */
void test_all(void);

#endif