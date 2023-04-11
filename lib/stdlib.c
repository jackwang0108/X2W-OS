/**
 * @file stdlib.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdlib.c`是`stdlib`库的实现, 实现了一系列库函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning 为了避免编译时编译器使用系统的`stdlib.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "stdlib.h"


void itoa(uint64_t integer, char** buf_ptr_addr, uint8_t base){
    // 使用递归来完成转换
    uint64_t remain = integer % base;       // 余数
    uint64_t quotient = integer / base;     // 商
    // 没有处理到最高位则继续处理
    if (quotient)
        itoa(quotient, buf_ptr_addr, base);
    if (remain < 10)
        *((*buf_ptr_addr)++) = remain + '0';
    else
        *((*buf_ptr_addr)++) = remain - 10 + 'A';
}

#if DEBUG == 1
extern void uart_puts(const char *);
int test_stdlib(void){
    uart_puts("---> "), uart_puts(__func__), uart_puts(": \n");
    // test itoa
    for (int base = 2; base <= 128; base *= 2){
        char *itoa_buf[64] = {0};
        itoa(base, (char**)&itoa_buf, base);
        uart_puts(*itoa_buf), uart_puts("\n");
    }
    return 0;
}
#endif