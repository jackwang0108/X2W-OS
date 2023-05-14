/**
 * @file kdebug.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kdebug.h`提供内核态的断言函数
 * @version 0.2
 * @date 2023-05-15
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */
#ifndef __INCLUDE_KERNEL_KDEBUG_H
#define __INCLUDE_KERNEL_KDEBUG_H

#include "constrains.h"
#include "kernel/ktrap.h"
#include "kernel/kstdio.h"

/**
 * @brief `kpanic_spin`用于在输出错误信息, 而后挂起`CPU`, 停止运行
 * 
 * @param filename 错误所在的文件
 * @param line 错误所在的行数
 * @param func 错误所在的函数
 * @param condition 导致错误发生的条件
 * @param msg 要输出的错误信息, 支持使用`stdfmt`中的格式控制字符
 * @param ... 可变参数列表, 需要和错误信息中的格式控制字符匹配
 * 
 */
static inline void kpanic_spin(char *filename, int line, const char* func, const char* condition, const char*msg, ...){
    supervisor_interrupt_disable();
    kprintf("===================== Error Message =====================%c", '\n');
    kprintf("filename: %s, at line %d\n", filename, line);
    kprintf("unsatisfied condition: %s\n", condition);
    kprintf("assert check failed in function: %s\n", func);
    char buf[PRINTF_STRING_SIZE] = {0};
    va_list args;
    va_start(args, msg);
    vsprintf(buf, msg, args);
    kprintf("info message: %s\n", buf);
    while (1);
}


#ifdef DEBUG
    /**
     * @brief `ASSERT`将确保`CONDITION`成立, 若不成立则将挂起内核并输出提示信息`MSG`
     * 
     */
    #define ASSERT(CONDITION, MSG, ...) if (CONDITION) {} else {PANIC(#CONDITION, #MSG);}
#else
    #define ASSERT(CONDITION, MSG, ...) ((void) 0)
#endif

#define PANIC(...) kpanic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#endif