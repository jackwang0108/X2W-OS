/**
 * @file stimer.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stimer.h`是时钟功能模块的SBI部分代码
 * @version 0.1
 * @date 2023-05-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STIMER_H
#define __INCLUDE_STIMER_H


#include "types.h"
#include "sbi/strap.h"
#include "constrains.h"

/**
 * @brief `stimer_init`是`stimer`的初始化函数
 * 
 */
void stimer_init(void);

/**
 * @brief `stimer_interrupt_handler`是M模式下的时钟中断处理函数
 */
int64_t stimer_interrupt_handler(strapframe_t *stf_ptr);

/**
 * @brief `clint_timer_event_start`函数用于设置`mtimecmp`寄存器
 * 
 * @param next_ticks 设置到`mtimecmp`寄存器的值
 */
void clint_timer_event_start(uint64_t next_ticks);

#endif