/**
 * @file stdfmt.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdfmt.c`是`stdfmt`库的实现, 实现了标准输入输出函数
 * @version 0.2
 * @date 2023-04-09
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "stdfmt.h"

// flag字段的标识
#define FLAG_ZEROPAD             0b0000001      // 0填充
#define FLAG_PLUS                0b0000010      // 正数是否加符号
#define FLAG_SPACE               0b0000100      // 空格填充
#define FLAG_LEFT                0b0001000      // 左对齐
#define FLAG_SUFFIX              0b0010000      // 0b, 0x, 0前缀

// type字段的标识
#define TYPE_SIGN                0b0100000      // 正数和负数的标识
#define TYPE_CAPITAL             0b1000000      // x和X的标识


// `is_digit`宏用于判断字符`ch`是否是ascii数字
#define is_digit(ch)  ('0' <= (ch) && (ch) <= '9')


/**
 * @brief `do_div`用于对`num`进行整除, 并返回余数
 * 
 * @param num 被整除的数字
 * @param base 整除的除数
 * 
 * @return 余数
 */
#define do_div(num, base) ({                            \
    unsigned int __base = (base);                       \
    unsigned int __remain;                              \
    __remain = ((unsigned long)(num)) % __base;         \
    (num) = ((unsigned long)(num)) / __base;            \
    __remain;                                           \
})


const char* _scan_number(const char *str, int* num){
    int tmp = 0;
    while (is_digit(*str)){
        tmp *= 10;
        tmp += *(str++) - '0';
    }
    *num = tmp;
    return str;
}


char *copy_number(char *str, unsigned long num, int base, int width, int precision, int flag){
    char pad_char;
    char sign = '\0';
    char char_num[128];

    // 处理大小写
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (flag & TYPE_CAPITAL)
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // 处理进制, 因为只有26个字母, 所以输出的时候最大只有36进制
    if (base < 2 || base > 36)  
        return NULL;
    // 处理对齐和填充字符
    if (flag & FLAG_LEFT)
        // 如果左对齐, 则清除0填充
        flag &= ~FLAG_ZEROPAD;
    // 处理填充字符
    pad_char = (flag & FLAG_ZEROPAD) ? '0' : ' ';
    // 处理符号
    if (flag & TYPE_SIGN){
        if ((long) num < 0){            // 处理负数
            sign = '-';
            num = -(long) num;
        } else if (flag & FLAG_PLUS){   // 正数前+号
            sign = '+';
            width--;
        } else if (flag & FLAG_SPACE){  // 空格填充
            sign = ' ';
            width--;
        }
    }
    // 处理前缀
    if (flag & FLAG_SUFFIX){
        if (base == 2 || base == 16)
            width -= 2;
        else if (base == 8)
            width -= 1;
    }

    // int数字转为字符串数字
    int i = 0;
    if (num == 0)
        char_num[i++] = '0';
    else
        while (num != 0)
            char_num[i++] = digits[do_div(num, base)];
    
    // 计算输出的精度
    precision = i > precision ? i : precision;
    width -= precision;

    // 空格填充
    if (!(flag & (FLAG_ZEROPAD | FLAG_LEFT)))
        while(width -- > 0)
            *str++ = ' ';
    
    // 输出符号
    if (sign)
        *str++ = sign;
    
    // 输出前缀
    if (flag & FLAG_SUFFIX){
        if (base == 2){
            *str++ = '0';
            *str++ = 'b';
        } else if (base == 8)
            *str++ = '0';
        else if (base == 16){
            *str++ = '0';
            *str++ = 'x';
        }
    }

    // 右对齐填充
    if (!(flag & FLAG_LEFT))
        while (width-- > 0)
            *str++ = pad_char;
    
    // 输出数字
    while (i < precision--)
        *str++ = '0';
    while (i-- > 0)
        *str++ = char_num[i];
    while (width-- > 0)
        *str++ = ' ';
    
    return str;
}


size_t vsprintf(char* str, const char* format, va_list ap){
    /*
     * 如何理解下面的代码? 想象有两个字符数组, 一个是output数组, 第一个是format数组
     * 各自有一个指针记录处理的字符的位置:
     *  1. fmt_pos先动:
     *      - 发现不是格式控制字符, 则复制到out_pos, 然后两者都+1
     *      - 发现是格式控制字符:
     *          1. 则fmt_pos向后移动, 先处理完这个格式控制字符
     *          2. 然后按照格式控制字符把值复制到output数组中, 此时out_pos向后移动需要的位数
     *  2. 不断循环处理, 直到fmt_pos指向的字符为'\0'
     *  
     */
    char* out_pos = str;                // output数组中的下标
    const char* fmt_pos = format;       // format数组中的下标

    // 格式控制字符的各个字段
    int flag, width, precision, length;

    // 处理type字段的变量
    char *str_arg; unsigned long num_arg; size_t *int_ptr_arg;
    
    // type字段为数字的时候的进制
    int base;
    
    // 循环处理
    for (fmt_pos = format; *fmt_pos != '\0'; fmt_pos++){
        // 不是格式字符, 直接复制
        if (*fmt_pos != '%'){
            *out_pos++ = *fmt_pos;
            continue;
        }
        // 是格式控制字符, fmt_pos先向后移动, 处理完当前的格式控制字符
        // 格式控制字符可能的字段: %[flag][width][precision][length]<type>, 依次处理这些字段

        // 1. 开始的%
        ++fmt_pos;

        // 2. 处理flag字段
        flag = 0;
        Bool is_flag_char = True;
        while (is_flag_char){
            switch (*fmt_pos){
                case '-':
                    // 左对齐flag
                    flag |= FLAG_LEFT;
                    break;
                case '+':
                    // 正数前有加号
                    flag |= FLAG_PLUS;
                    break;
                case ' ':
                    // 空格填充
                    flag |= FLAG_SPACE;
                    break;
                case '#':
                    // 0b, 0x或者0前缀
                    flag |= FLAG_SUFFIX;
                    break;
                case '0':
                    // 0填充
                    flag |= FLAG_ZEROPAD;
                    break;
                default:
                    // 没有flag字符
                    is_flag_char = False;
            }
            fmt_pos++;      // 处理下一个flag字符
        }
        fmt_pos--;          // 最后一个字符肯定不是flag字符, 所以回退一个


        // 3. 处理width字段
        width = -1;
        if (is_digit(*fmt_pos)){
            // width字段为数字, 获取数字之后修改fmt_pos
            fmt_pos = _scan_number(fmt_pos, &width);
        } else if (*fmt_pos == '*'){
            // width字段为动态参数
            width = va_arg(ap, int);
            // 负数的动态参数当做左对齐处理
            if (width < 0){
                width = -width;
                flag |= FLAG_LEFT;
            }
            fmt_pos++;
        }
        // 没有width字符


        // 4. 处理precision字段
        precision = -1;
        if (*fmt_pos == '.'){
            // 有precision字符
            ++fmt_pos;
            if (is_digit(*fmt_pos))
                // precision字段为数字, 获取数字之后修改fmt_pos
                fmt_pos = _scan_number(fmt_pos, &precision);
            else if (*fmt_pos == '*'){
                // precision字段为动态参数
                precision = va_arg(ap, int);
                fmt_pos++;
            }
            
            // 如果precision为负数, 则视为0
            if (precision < 0)
                precision = 0;
        }
        // 没有precision字符


        // 5. 处理length字段
        length = -1;
        if (*fmt_pos == 'h' || *fmt_pos == 'l' || *fmt_pos == 'L'){
            length = *fmt_pos++;
            // 下一个length字段还是l/L
            if (length == 'l' && *fmt_pos == 'l'){
                length = 'L';
                fmt_pos++;
            }
        }

        // 6. 处理type字段(输出非整数), 同时移动out_pos
        switch (*fmt_pos){
            case '%':                               // 输出一个%
                *out_pos++ = '%';
                continue;
            case 'c':                               // 输出一个字符
                // 处理左对齐
                if (!(flag & FLAG_LEFT))
                    while (--width > 0)
                        *out_pos++ = ' ';
                *out_pos++ = (unsigned char) va_arg(ap, int);
                // 默认是右对齐
                while (--width > 0)
                    *out_pos++ = ' ';
                continue;
            case 's':                               // 输出一个字符串
                str_arg = va_arg(ap, char *);
                if (*str_arg == '\0')
                    str_arg = "<NULL>";
                int str_arg_len = (int) strlen(str_arg);
                precision = precision > 0 ? precision : str_arg_len;
                str_arg_len = str_arg_len > precision ? precision : str_arg_len;
                // 复制字符串
                // 处理左对齐
                if (!(flag & FLAG_LEFT))
                    while (str_arg_len < width--)
                        *out_pos++ = ' ';
                strcpy(out_pos, str_arg);
                out_pos += str_arg_len;
                // 默认是右对齐
                while (str_arg_len < width--)
                    *out_pos++ = ' ';
                continue;
            case 'n':                               // 保存一共输出的字符个数到传入的变量地址
                int_ptr_arg = (size_t *)va_arg(ap, size_t*);
                *int_ptr_arg = (size_t)(out_pos - str);
                continue;
            case 'p':                               // 输出一个指针
                if (width == -1){
                    width = 2 * sizeof(void *);
                    flag |= FLAG_ZEROPAD;
                }
                out_pos = copy_number(
                    out_pos, (unsigned long) va_arg(ap, void*),
                    16, width, precision, flag
                );
                continue;
            case 'b':                               // 二进制输出一个整数
                base = 2;
                break;
            case 'o':                               // 八进制输出一个整数
                base = 8;
                break;
            case 'X':                               // 十六进制输出一个整数, 大写
                flag |= TYPE_CAPITAL;
            case 'x':                               // 十六进制输出一个整数, 小写
                base = 16;
                break;
            case 'd':                               // 十进制输出一个整数, 有符号
            case 'i':                               // 十进制输出一个整数, 有符号
                flag |= TYPE_SIGN;
            case 'u':                               // 十进制输出一个整数, 无符号
                base = 10;
                break;
            
            default:                                // 非可识别的格式控制字符, 视为普通字符复制
                if (*fmt_pos)
                    *out_pos++ = *fmt_pos;
                else
                    fmt_pos--;
                break;
        }

        // 7. 处理type字段(输出整数), 同时移动out_pos
        if (length == 'L') {
            if (flag & TYPE_SIGN)
                num_arg = va_arg(ap, long);
            else
                num_arg = va_arg(ap, long);
        } else if (length == 'h') {
            if (flag & TYPE_SIGN)
                num_arg = (short) va_arg(ap, int);
            else
                num_arg = (unsigned short) va_arg(ap, unsigned int);
        } else if (length == 'l') {
            if (flag & TYPE_SIGN)
                num_arg = va_arg(ap, long);
            else
                num_arg = va_arg(ap, unsigned long);
        } else {
            if (flag & TYPE_SIGN)
                num_arg = (int) va_arg(ap, int);
            else
                num_arg = va_arg(ap, unsigned int);
        }
        // 复制数字
        out_pos = copy_number(out_pos, num_arg, base, width, precision, flag);
    }

    *out_pos = '\0';
    return out_pos - str;
}

size_t sprintf(char* buf, const char* format, ...){
    va_list args;
    size_t retval;
    va_start(args, format);
    retval = vsprintf(buf, format, args);
    va_end(args);
    return retval;
}
