/**
 * @file stdio.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdfmt.c`是`stdfmt`库的实现, 实现了标准输入输出函数
 * @version 0.1
 * @date 2023-04-09
 * 
 * @warning 为了避免编译时编译器使用系统的`stdio.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "stdfmt.h"

size_t vsprintf(char* str, const char* format, va_list ap){
    char* buf_ptr = str;
    const char* index_ptr = format;
    char index_char = *index_ptr;

    int64_t arg_int;
    char *arg_str;
    while (index_char){
        // 不是%, 则将字符复制到str中, 而后继续扫描
        if (index_char != '%'){
            *(buf_ptr++) = index_char;
            index_char = *(++index_ptr);
            continue;
        }
        // 获%后面具体的控制字符
        index_char = *(++index_ptr);
        switch (index_char){
            // 输出字符串
            case 's':
                arg_str = va_arg(ap, char*);
                // 字符串以'\0'结尾, 直接复制即可
                strcpy(buf_ptr, arg_str);
                buf_ptr += strlen(arg_str);
                index_char = *(++index_ptr);
                break;
            // 输出单个字符
            case 'c':
                *(buf_ptr++) = (unsigned char)va_arg(ap, int);
                index_char = *(++index_ptr);
                break;
            // 输出二进制的整数
            case 'b':
                arg_int = va_arg(ap, int);
                itoa(arg_int, &buf_ptr, 2);
                index_char = *(++index_ptr);
                break;
            // 输出八进制的整数
            case 'o':
                arg_int = va_arg(ap, int);
                itoa(arg_int, &buf_ptr, 8);
                index_char = *(++index_ptr);
                break;
            // 输出十进制的整数
            case 'd':
                arg_int = va_arg(ap, int);
                if (arg_int < 0){
                    arg_int = 0 - arg_int;
                    *buf_ptr++ = '-';
                }
                itoa(arg_int, &buf_ptr, 10);
                index_char = *(++index_ptr);
                break;
            // 输出十六进制的整数
            case 'x':
                arg_int = va_arg(ap, int);      // 将ap指向的参数转为int
                itoa(arg_int, &buf_ptr, 16);
                index_char = *(++index_ptr);
                break;
            // 单独输出一个%
            case '%':
                *(buf_ptr++) = '%';
                index_char = *(++index_ptr);
            default:
                break;
        }
    }
    return strlen(str);
}

size_t sprintf(char* buf, const char* format, ...){
    va_list args;
    size_t retval;
    va_start(args, format);
    retval = vsprintf(buf, format, args);
    va_end(args);
    return retval;
}

#if DEBUG == 1
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
#endif