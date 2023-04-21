/**
 * @file test_strap.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_strap.c`是`strap.c`的测试文件
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test/test_strap.h"
#include "kernel/kstdio.h"

// 使用汇编指令触发异常, 定义在test_strap-asm.S中
extern void trigger_breakpoint(void);                         // 触发breakpoint异常
extern void trigger_ecall(void);                              // 触发U-ecall异常
DEPRECATED extern void trigger_load_access_fault(void);       // 触发Load Access Fault异常, 已弃用

int test_exception(void){
    kprintf("=> %s:\n", __func__);
    // 由于开启了PMP为全地址, 因此不会触发该异常, 已经弃用
    // trigger_load_access_fault();
    // trigger_breakpoint();
    trigger_ecall();
}