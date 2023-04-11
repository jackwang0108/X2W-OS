/**
 * @file test.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test.c`是测试模块的实现函数
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test.h"

/// @brief 测试函数结构体
struct {
    /// @brief 测试函数
    uint8_t regiter_idx;
    /// @brief 测试函数数组
    test_func_pointer_t test_functions[MAX_TEST_FUNCTION_NUM];
} _Test = {
    .regiter_idx = 0,
    .test_functions = {(test_func_pointer_t)NULL} 
};


void register_test_func(test_func_pointer_t func){
    #if DEBUG == 1
    if (_Test.regiter_idx < MAX_TEST_FUNCTION_NUM - 1)
        _Test.test_functions[_Test.regiter_idx++] = func;
    #endif
}

#include "uart.h"
#include "stdlib.h"
#include "stdfmt.h"
#include "kstdio.h"
#include "string.h"

void test_all(void){
    #if DEBUG == 1

    register_test_func(test_uart);
    register_test_func(test_stdlib);
    register_test_func(test_stdfmt);
    register_test_func(test_kstdio);


    for (int i = 0; i < _Test.regiter_idx; i++)
        (*_Test.test_functions[i])();
    #endif
}