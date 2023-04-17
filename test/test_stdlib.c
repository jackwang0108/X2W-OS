/**
 * @file test_stdlib.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_stdlib.c`是`stdlib`的测试文件
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test/test_stdlib.h"


extern void uart_puts(const char *);

int test_stdlib(void){
    uart_puts("=> "), uart_puts(__func__), uart_puts(": \n");
    // test itoa
    char itoa_buf[64] = {0};
    for (int base = 2; base <= 128; base *= 2){
        char *str = itoa_buf;
        itoa(base, &str, 2);
        uart_puts(itoa_buf), uart_puts("\n");
    }

    return 0;
}