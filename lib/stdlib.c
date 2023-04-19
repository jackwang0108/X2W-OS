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
#define INT_MAX 2147483647
#define INT_MIN -2147483648

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

int64_t atoi(char* str){
    int sign=1,num=0;

    //跳过字符串开头处的空格
    while (*str == ' ')
    {
        str++;
    }

    //处理符号位
    if(*str == '-'){
        sign = -1;
        str++;
    }

    //逐个字符解析数字
    while(*str >='0' && *str <='9'){
        num = num*10 + (*str-'0');
        str++;
        if (num > INT_MAX / 10 || (num == INT_MAX / 10 && (*str - '0') > INT_MAX % 10)) {
            return sign == -1 ? INT_MIN : INT_MAX;  // 溢出处理
        }
    }
    
    return num*sign;
}
