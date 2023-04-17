/**
 * @file test_kstdio.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_kstdio.c`是`kstdio`的测试文件
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test/test_kstdio.h"


// 声明外部符号, 避免include
extern size_t kprintf(const char* format, ...);

int test_kstdio(void){
    kprintf("=> %s:\n", __func__);
    for (int i = -2; i < 2; i++)
        kprintf("\tHello World: %d\n", i);
    return 0;
}