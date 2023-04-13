/**
 * @file kdebug.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kdebug.h`提供了内核的debug函数
 * @version 0.1
 * @date 2023-04-13
 * 
 * @warning 需要在完成中断后添加关中断
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_ASSERT_H
#define __INCLUDE_ASSERT_H

#include "constrains.h"
#include "stdarg.h"
#include "stdfmt.h"
#include "kstdio.h"
#include "ustdio.h"

#ifdef DEBUG
    #define ASSERT(CONDITION, MSG) if (CONDITION) {} else {PANIC(#CONDITION, #MSG)}
#else
    #define ASSERT(CONDITION) ((void) 0)
#endif

#define PANIC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

/**
 * @brief `panic_spin`用于在输出错误信息, 而后挂起`CPU`, 停止运行
 * 
 * @param filename 错误所在的文件
 * @param line 错误所在的行数
 * @param func 错误所在的函数
 * @param condition 导致错误发生的条件
 * @param msg 要输出的错误信息, 支持使用`stdfmt`中的格式控制字符
 * @param ... 可变参数列表, 需要和错误信息中的格式控制字符匹配
 * 
 * @note `panic_spin`会根据内核和用户态的不同而行为不同, 具体由`KERNEL_LIB`和`USER_LIB`两个宏控制
 */
static inline void panic_spin(char *filename, int line, const char* func, const char* condition, const char*msg, ...){
    size_t (*printf)(const char*, ...);
    #ifdef KERNEL_LIB
        printf = kprintf;
    #endif
    #ifdef USER_LIB
        printf = uprintf;
    #endif
    // 需要在这里关中断
    printf("===================== Error Message =====================%c", '\n');
    printf("Filename: %s\n", filename);
    printf("At line: %d\n", line);
    printf("In Function: %s\n", func);
    printf("Not Met Condition: %s\n", condition);
    printf("Messages:%c", '\n');
    char buf[PRINTF_STRING_SIZE] = {'\0'};
    va_list args;
    va_start(args, msg);
    vsprintf(buf, msg, args);
    printf(buf);
}

#endif