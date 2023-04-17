/**
 * @file sbi.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `sbi.c`是`SBI`调用的实现
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "sbi/sbi.h"


void sbi_settimer(uint64_t value){
    _SBICALL1(SBICALL_SETTIMER, value);
}

void sbi_putc(char c){
    _SBICALL1(SBICALL_CONSOLE_PUTCHAR, c);
}

void sbi_puts(char *str){
    _SBICALL1(SBICALL_CONSOLE_PUTSTR, str);
}