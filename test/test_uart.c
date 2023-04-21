/**
 * @file test_uart.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_uart.c`是uart的测试文件
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test/test_uart.h"


// 声明外部符号, 避免include
extern void uart_put(char);
extern char uart_get(void);
extern void uart_puts(const char*);

int test_uart(void){
    // 测试 uart_put
    uart_puts("=> test_uart:\n");
    char charset[] = "01234567890ABCDEFGHI./?\"'";
    for (int i = 0; i < sizeof(charset)/sizeof(char); i++)
        uart_put(charset[i]);
    uart_put('\n');

    // 测试 uart_puts
    char *str = "Hello UART!\n";
    uart_puts(str);

    // char *prompt0 = "Please enter a <Enter>: ";
    // char *prompt1 = "\nI get an <Enter>!\n";
    // char ch;
    // uart_puts(prompt0);
    // while ((ch = uart_get()) != '\r');
    // uart_puts(prompt1);

    return 0;
}