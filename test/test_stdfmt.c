/**
 * @file test_stdfmt.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_stdfmt.c`是`stdfmt`的测试文件
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test/test_stdfmt.h"

// 声明外部符号, 避免include
extern void uart_puts(const char*);

// _test_types用于测试格式控制符的type字段
void _test_types(void);

// _test_flags用于测试格式控制符的flag字段
void _test_flags(void);

// _test_width用于测试格式控制符的width字段
void _test_width(void);

// _test_precision用于测试格式控制符的precision字段
void _test_precision(void);

// _test_length用于测试格式控制符的length字段
void _test_length(void);


int test_stdfmt(void){
    uart_puts("=> "), uart_puts(__func__), uart_puts(": \n");
    
    _test_types();
    _test_flags();
    _test_width();
    _test_precision();
    _test_length();

    return 0;
}

void _test_types(void){
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a single percent sign: %%\n");
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a single char: %c\n", 'J');
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a string: %s\n", "'Hello World'");
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a pointer: %p\n", (void*)sprintf_buffer);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a binary: %b\n", 0b1111111111111111);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a octal: %o\n", 01234567);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a hex: %x\n", 0xABCDEF);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a capitcal-hex: %X\n", 0xABCDEF);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a decimal: %i\n", 123456789);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a decimal: %d\n", 123456789);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a unsigned decimal: %u\n", (unsigned int)(-1));
        uart_puts(sprintf_buffer);
    }
    {
        size_t nums = 0;
        char sprintf_buffer1[100] = {0};
        char sprintf_buffer2[100] = {0};
        sprintf(sprintf_buffer1, "Output a string: %s, char been output:%n", "'Hello World'", &nums);
        uart_puts(sprintf_buffer1);
        sprintf(sprintf_buffer2, "%d\n", (int)nums);
        uart_puts(sprintf_buffer2);
    }
}

void _test_flags(void){
    { 
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a 20-bit  left-aligned decimal: %-20d\n", 1234567890);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a 20-bit right-aligned decimal: %20d\n", 1234567890);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a 20-bit right-aligned space-padded decimal: %20d\n", 1234567890);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a 20-bit right-aligned     0-padded decimal: %020d\n", 1234567890);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a positive decimal: %d\n", 100);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a negative decimal: %d\n", -100);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a signed positive decimal: %+d\n", 100);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a signed negative decimal: %+d\n", -100);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a suffixed binary: %#b\n", 0b1111);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a suffixed octal: %#o\n", 07654321);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a suffixed hex: %#x\n", 0xabcdef);
        uart_puts(sprintf_buffer);
    }
}


void _test_width(void){
    uart_puts("                              : 01234567890123456789\n");
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a fixed-width decimal  : %20d\n", 123456);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a dynamic-width decimal: %*d\n", 15, 123456);
        uart_puts(sprintf_buffer);
    }
}


void _test_precision(void){
    uart_puts("                                  : 01234567890123456789\n");
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a fixed-precision decimal  : %.2d\n", 123456);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a fixed-precision decimal  : %.9d\n", 123456);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a dynamic-precision decimal: %.*d\n", 15, 123456);
        uart_puts(sprintf_buffer);
    }
}


void _test_length(void){
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a short: %hd\n", (short)12345);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a long: %ld\n", (long)123456789987654321);
        uart_puts(sprintf_buffer);
    }
    {
        char sprintf_buffer[100] = {0};
        sprintf(sprintf_buffer, "Output a long long: %ld\n", (long)987654321123456789);
        uart_puts(sprintf_buffer);
    }
}