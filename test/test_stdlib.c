/**
 * @file test_stdlib.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_stdlib.c`是`stdlib`的测试文件
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "kernel/kstdio.h"
#include "test/test_stdlib.h"


// 声明外部符号, 避免include
extern void uart_puts(const char *);

//_test_itoa用于测试itoa函数
void _test_itoa(void);

//_test_atoi用于测试atoi函数
void _test_atoi(void);

int test_stdlib(void){
    uart_puts("=> "), uart_puts(__func__), uart_puts(": \n");
    _test_itoa();
    _test_atoi();

    return 0;
}

void _test_itoa(void){
        // test itoa
    char itoa_buf[64] = {0};
    for (int base = 2; base <= 128; base *= 2){
        char *str = itoa_buf;
        itoa(base, &str, 2);
        uart_puts(itoa_buf), uart_puts("\n");
    }
}

void _test_atoi(void){
    char* test="123456";
    char sprintf_buffer[100] = {0};
    sprintf(sprintf_buffer,"Testing atoi,output :%d\n",atoi(test));
    uart_puts(sprintf_buffer);
}