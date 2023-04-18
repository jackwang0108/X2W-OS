/**
 * @file sstdio.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `sstdio`库的实现
 * @version 0.1
 * @date 2023-04-18
 * 
 * @warning `sstdio`为SBI库, 仅供SBI使用, 内核程序请使用`sstdio`库, 用户程序请使用`ustdio`库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "sbi/sstdio.h"


size_t bprintf(const char* format, ...){
    va_list args;
    va_start(args, format);
    char buf[PRINTF_STRING_SIZE] = {0};
    vsprintf(buf, format, args);
    va_end(args);
    uart_puts((const char*)buf);
    return strlen(buf);
}
