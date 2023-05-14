/**
 * @file kplic.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kplic.h`是的`PLIC`全局中断控制器初始化内核部分的代码
 * @version 0.1
 * @date 2023-05-13
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_KPLIC_H
#define __INCLUDE_KERNEL_KPLIC_H

#include "types.h"
#include "asm/plic.h"
#include "constrains.h"
#include "trap/trapframe.h"

/**
 * @brief `kplic_init`是`PLIC`中断控制器的初始化函数
 * 
 * @note 目前只对一个核心进行了初始化, 未来多核心要考虑这个问题
 */
void kplic_init(void);


/**
 * @brief `kplic_set_priority`用于设置
 * 
 * @param hwiid 硬件中断号, hardware interrupt id
 * @param priority 优先级, 0~7, 0表示关闭, 7最高
 */
void kplic_set_priority(uint32_t hwiid, uint32_t priority);

/**
 * @brief `kplic_enable_interrupt`用于设置指定CPU的指定硬件中断使能状态
 * 
 * @param cid CPU id
 * @param hwiid 硬件中断号, hardware interrupt id
 * @param enable True, 使能; False, 不使能
 */
void kplic_enable_interrupt(uint64_t cid, uint32_t hwiid, Bool enable, Bool m_mode);


/**
 * @brief `kplic_interrupt_handler`是S模式下的时钟中断处理函数
 * 
 * @param ktf_ptr 陷入帧, S模式下中断触发后在`ktrap_enter`中构建
 * @return int64_t 处理结果, 若为0则表示处理正常, -1表示处理失败
 * 
 * @note 目前所有中断都是CPU0处理的, 未来多核设计的时候需要修改一个版本
 */
int64_t kplic_interrupt_handler(ktrapframe_t *ktf_ptr);

#endif