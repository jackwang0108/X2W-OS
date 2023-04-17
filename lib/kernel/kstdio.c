/**
 * @file kstdio.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kstdio`库的实现
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning `kstdio`为内核库, 仅供内核使用, 用户程序请使用`ustdio`库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "kernel/kstdio.h"

size_t kprintf(const char* format, ...){
    va_list args;
    va_start(args, format);
    char buf[PRINTF_STRING_SIZE] = {0};
    vsprintf(buf, format, args);
    va_end(args);
    uart_puts((const char*)buf);
    return strlen(buf);
}
