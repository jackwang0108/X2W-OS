/**
 * @file string.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `string.h`提供了一系列内存和字符串操作函数. 
 * @version 0.1
 * @date 2023-04-08
 * 
 * @todo 
 *  1. 需要进行测试, 以测试函数正确性
 *  2. 实现debug.h后需要给每个指针添加 NULL 检查
 * 
 * @warning 为了避免编译时编译器使用系统的`string.h`库, 编译时需要为`GCC`指定`-nostdinc`选项, 详见根目录下的`Makefile`
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STRING_H
#define __INCLUDE_STRING_H

#include "types.h"


/**
 * @brief `memset`将`det_`起始的`size`个字节的内存单元的值设置为`value`
 * 
 * @param dst 起始地址
 * @param value 设置的值
 * @param size 要设置多少个字节
 * 
 * @return size_t 成功设置的字节数
 */
size_t memset(void *dst, byte value, size_t size);

/**
 * @brief `strmset是针对字符串复制的安全实现，避免了字符串复制时可能存在的缓存区溢出，`将`det_`起始的`size`个字节的内存单元的值设置为`value`
 * 
 * @param dst 起始地址
 * @param value 设置的值
 * @param size 要设置多少个字节
 * 
 * @return size_t 成功设置的字节数
 */
size_t strmset(void *dst, char value, size_t size);


/**
 * @brief `memcpy`将`src`起始的`size`个内存单元的值(以字节计算)复制到`dst`起始的`size`个内存单元中
 * 
 * @param dst 被复制的内存区域的起始地址
 * @param src 复制到的内存区域的起始地址
 * @param size 复制的字节数
 * 
 * @return size_t 复制的字节数
 * 
 * @warning `memcpy`不会进行长度检查, 注意`src`和`dst`的长度, 以避免溢出
 */
size_t memcpy(void *dst, const void* src, size_t size);

/**
 * @brief memmove - Copy a block of memory, handling overlaps
 * 
 * @param dst Destination buffer
 * @param src Source buffer
 * @param size Size of buffer
 * @return size_t return the copied char num
 * @note 与 memcpy() 不同，memmove() 处理重叠缓冲区。
 * 复制发生的过程就像 src 中的字节首先被复制到
 * 不与 src 或 dest 和字节重叠的临时缓冲区
 * 然后从临时缓冲区复制到目标。
 * @note 此函数用于代替 memcpy() 在可能重叠的内存块之间进行复制。
 */
size_t memmove(void* dst, const void *src, size_t size);

/**
 * @brief `memcmp`用于比较两个分别以`a`和`b`为起始地址的的`size`个字节的内存区域的值
 * 
 * @param a 要比较的第一个内存区域的地址
 * @param b 要比较的第二个内存区域的地址
 * @param size 要比较的字节数
 * 
 * @return int8_t 比较的结果:
 *  - 若`a`>`b`, 则返回`1`
 *  - 若`a`=`b`, 则返回`0`
 *  - 若`a`<`b`, 则返回`-1`
 * 
 */
int8_t memcmp(const void *a, const void *b, size_t size);

/**
 * @brief `strcpy`复制`src`指向的以'\0'结尾的字符串到`dst`指向的字符串去中去
 * 
 * @param dst 将要复制到的内存地址
 * @param src 被复制的字符串
 * 
 * @return char* 返回dst的首地址
 * 
 * @warning `strcpy`不会进行长度检查, 注意`src`和`dst`的长度, 以避免溢出
 */
char* strcpy(char *dst, const char *src);


/**
 * @brief `strlen`返回以'\0'结尾的字符串的长度
 * 
 * @param str 要获取长度的字符串
 * 
 * @return size_t 字符串的长度
 */
size_t strlen(const char* str);


/**
 * @brief `strcmp`比较两个字符串
 * 
 * @param a 要比较的第一个字符串
 * @param b 要比较的第二个字符串
 * 
 * @return int8_t 返回比较的结果
 *  - 若`a`>`b`, 则返回`1`
 *  - 若`a`=`b`, 则返回`0`
 *  - 若`a`<`b`, 则返回`-1`
 */
int8_t strcmp(const char* a, const char* b);


/**
 * @brief `strcat`用于将字符串`src`拼接到`dst`后
 * 
 * @param dst 将被拼接到的字符串
 * @param src 将被拼接的字符串 
 * 
 * @return char* 返回dst的首地址
 * 
 * @warning `strcat`不会进行长度检查, 注意`src`和`dst`的长度, 以避免溢出
 */
char* strcat(char *dst, const char* src);


/**
 * @brief `strchr`返回字符串从左往右第一次出现字符`ch`的地址
 * 
 * @param str 被搜索的字符串
 * @param ch 要搜索的字符
 * 
 * @return char* 指向字符的指针, 
 * - 若字符`ch`在字符串`str`中, 则返回字符串第一个`ch`的地址
 * - 若字符`ch`不在字符串`str`中, 则返回 `NULL`
 * 
 * @note `strchr`相当于返回字符`ch`第一次出现的地址
 */
char* strchr(const char* str, const uint8_t ch);


/**
 * @brief `strrchr`返回反向字符串第一次出现字符`ch`的地址
 * 
 * @param str 被搜索的字符串
 * @param ch 要搜索的字符
 * 
 * @return char* 
 * - 若字符`ch`在字符串`str`中, 则返回反向字符串第一个`ch`的地址
 * - 若字符`ch`不在字符串`str`中, 则返回 `NULL`
 * 
 * @note `strrchr`相当于返回字符`ch`最后一次出现的地址
 */
char* strrchr(const char* str, const uint8_t ch);

/**
 * @brief `strchrs`返回字符串`str`中字符`ch`出现的次数
 * 
 * @param str 要被搜索的字符串
 * @param ch 要查询的字符
 * 
 * @return size_t 字符`ch`在字符串`str`中出现的次数 
 */
size_t strchrs(const char* str, const uint8_t ch);

/**
 * @brief 'strslice'从src字符串中截取从start的len个字节写入start中；同时检查是否越界，避免缓存区溢出；
 * 
 * @param str 要被截取的字符串
 * @param dst 要被写入的字符串
 * @param start 开始的下标
 * @param len 要截取的长度
 * @return size_t 成功截取的长度
 */
size_t strslice(const char* src, char* dst, int64_t start, int64_t len);

/**
 * @brief ‘encrypt_decrypt’函数通过使用一个异或操作对字符串进行加密，由于异或具有自反的性质没所以该函数同样可以用于解密！
 * @param str 要被加密的字符串
 * @param len 需要被加密的长度
 * @param key 加密KEY
 * @return void return
*/
void encrypt_decrypt(char *str, int len, char key);

int regu_match(const char *str, const char *pattern, int match_pos[]);


/**
 * @brief ‘regu_replac’函数通过使用正则表达式对字符串中特定字符串进行一个替换；
 * @param str 需要被操作的字符串
 * @param pattern 正则匹配模式
 * @param replacement 替换为的字符
 * @param max_len 最大处理长度
 * @return void return
*/
void regu_replace(char *str, const char *pattern, const char *replacement, int max_len);




#endif