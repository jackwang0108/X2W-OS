/**
 * @file string.c
 * @author Shihong Wang  (jack4shihong@gmail.com)
 * @brief `string.c`是`string`库的实现, 实现了一系列内存和字符串操作函数
 * @version 0.1
 * @date 2023-04-08
 * 
 * @warning 为了避免编译时编译器使用系统的`string.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "string.h"

size_t memset(void* dst, byte value, size_t size){
    size_t num = 0;
    uint8_t* dst_ = (uint8_t *)dst;
    while(size-- > 0)
        *dst_++ = value, num++;
    return num;
}

size_t strmset(void *dst, char value, size_t size){
    size_t num = 0;
    size_t maxl=strlen((char*)dst);
    char* dst_ = (char *)dst;
    while(size-- > 0&& maxl-->0)
        *dst_++ = value, num++;
    return num;
}

size_t memcpy(void* dst, const void* src, size_t size){
    size_t num = 0;
    uint8_t* dst_ = (uint8_t*) dst;
    const uint8_t* src_ = (uint8_t*) src;
    while (size-- > 0)
        *dst_++ = *src_++, num++;
    return num;
}


int8_t memcmp(const void* a, const void* b, size_t size){
    const char* a_ = a;
    const char* b_ = b;
    while (size-- > 0){
        if (*a_ != *b_)
            return (int8_t)(*a_ > *b_ ? 1 : -1);
        a_++, b_++;
    }
    return (int8_t)0;
}


char* strcpy(char* dst, const char* src){
    char* r = dst;
    // C语言传参是值复制, 因此dst++和src++不会修改原指针指向的地址
    while ((*dst++ = *src++));
    return r;
}


size_t strlen(const char* str){
    const char* p = str;
    while(*p++);
    // p在最后会指向'\0'
    return (size_t)((p - 1) - str);
}


int8_t strcmp(const char* a, const char* b){
    while(*a != 0 && *a == *b)
        // C语言传参是值复制, 因此a++和b++不会修改原指针指向的地址
        a++, b++;
    return *a < *b ? -1 : *a > *b;
}


char *strcat(char *dst, const char* src){
    char *str = dst;
    // 先让str移动到最后
    while (*str++);
    --str;
    while((*str++ = *src++));
    return dst;
}


char* strchr(const char* str, const uint8_t ch){
    while (*str != 0){
        if(*str == ch)
            return (char*) str;
        // C语言传参是值复制, 因此str++不会修改原指针指向的地址
        str++;
    }
    return NULL;
}


char* strrchr(const char* str, const uint8_t ch){
    const char* last_char = NULL;
    while (*str != 0){
        if (*str == ch)
            last_char = str;
        // C语言传参是值复制, 因此str++不会修改原指针指向的地址
        str++;
    }
    return (char*) last_char;
}


size_t strchrs(const char* str, uint8_t ch){
    size_t ch_cnt = 0;
    const char* p = str;
    while (*p != 0){
        if (*p == ch)
            ch_cnt++;
        p++;
    }
    return ch_cnt;
}

size_t strslice(const char* src, char* dst, int64_t start, int64_t len){
    char* sour=(char*)src+start;
    int64_t maxl=strlen(sour);
    size_t num=0;
    if(len>maxl)len=maxl;
    while(len-->0){
        num++;
        *(dst++)=*(sour++);
    }
    *dst='\0';
    return num;
}

void encrypt_decrypt(char *str, int len, char key)
{
    int i;
    for (i = 0; i < len; i++) {
        str[i] ^= key;
    }
}


int regu_match(const char *str, const char *pattern, int match_pos[])
{
    int i, j, k;
    int len_str = strlen(str);
    int len_pat = strlen(pattern);
    int num_match = 0;
    for (i = 0; i < len_str; i++) {
        if (str[i] == pattern[0]) {
            k = i;
            for (j = 0; j < len_pat; j++) {
                if (k >= len_str || str[k] != pattern[j]) {
                    break;
                }
                k++;
            }
            if (j == len_pat) {
                match_pos[num_match++] = i;
                i = k - 1;
            }
        }
    }
    return num_match;
}

void regu_replace(char *str, const char *pattern, const char *replacement, int max_len)
{
    int i, j, k;
    int len_str = strlen(str);
    int len_pat = strlen(pattern);
    int len_rep = strlen(replacement);
    int match_pos[2048];
    int num_match = regu_match(str, pattern, match_pos);
    if (num_match == 0) {
        return;
    }
    int new_len = len_str + num_match * (len_rep - len_pat);
    if (new_len >= max_len) {
        return;
    }
    k = 0;
    j = 0;
    for (i = 0; i < len_str; i++) {
        if (i == match_pos[j]) {
            memcpy(&str[k], replacement, len_rep);
            k += len_rep;
            i += len_pat - 1;
            j++;
        } else {
            str[k++] = str[i];
        }
    }
    str[k] = '\0';
}


