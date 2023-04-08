/**
 * @file ustdio.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ustdio`库的实现
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning `ustdio`为用户程序库, 仅供用户程序使用, 内核请使用`kstdio`库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "ustdio.h"

size_t kprintf(const char* format, ...){
    va_list args;
    va_start(args, format);
    char buf[PRINTF_STRING_SIZE] = {0};
    vsprintf(buf, format, args);
    va_end(args);
    // uart_puts(buf);
    return strlen(buf);
}