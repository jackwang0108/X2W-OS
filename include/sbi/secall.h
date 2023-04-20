/**
 * @file secall.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `secall.h`提供了`sbi`调用的实现
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_SBI_SECALL_H
#define __INCLUDE_SBI_SECALL_H

#include "uart.h"
#include "asm/csr.h"
#include "sbi/sbi.h"
#include "sbi/strap.h"


/**
 * @brief `secall_init`用于初始化`SBI`调用
 */
void secall_init(void);

/**
 * @brief `sup_ecall_handler`是`Supervisor`模式下`ecall`指令的处理函数
 * 
 * @param stf_ptr 陷入帧, 调用ecall的程序主动触发`Supervisor Environment Call`异常时在`strap_enter`(定义在`strap_entry.S`中)中构建
 * @return int64_t 处理结果, 若为0则表示处理正常, -1表示处理失败
 */
int64_t sup_ecall_handler(strapframe_t *stf_ptr);

#endif