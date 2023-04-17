/**
 * @file test_stdfmt.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "test/test_stdfmt.h"


// 声明外部符号, 避免include
extern void uart_puts(const char*);

int test_stdfmt(void){
    uart_puts("=> "), uart_puts(__func__), uart_puts(": \n");
    
    // test sprintf
    char sprintf_buffer[100] = {0};
    sprintf(
        sprintf_buffer,
        "%% - %c - %s - %b - %o - %d - %x\n",
        'J', "Shihong Wang", 0b1111, 0100, 20, 0x1234
    );
    uart_puts(sprintf_buffer);

    return 0;
}