/**
 * @file ktimer.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ktimer.h`是时钟功能模块的内核部分代码
 * @version 0.1
 * @date 2023-05-10
 * 
 * @note 目前的时钟中断会产生两个中断, 第一个是时钟中断, 第二个是设置mtimecmp寄存器的ecall异常, 这个会造成性能损失, 未来需要改进到直接处理
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KTIMER_H
#define __INCLUDE_KTIMER_H

#include "io.h"
#include "types.h"
#include "asm/clint.h"
#include "constrains.h"
#include "kernel/ktrap.h"


#if RDTIME_SUPPORT == 1
/**
 * @brief `get_cycle`函数将通过`rdtime`指令以获取系统当前的时钟周期数
 * 
 * @note 由于该函数会被频繁的调用, 因此最好声明为内联函数
 * @note `get_cycle`函数的行为由`constrains.h`中的`RDTIME_SUPPORT`宏决定
 * 
 * @return uint64_t 当前系统的时钟周期数
 */
static inline uint64_t get_cycle(void){
    uint64_t cycles = 0;
    asm volatile (
        "rdtime %0"
        : "=r"(cycles)
    );
    return cycles;
}
#else 
/**
 * @brief `get_cycle`函数将读取`CLINT`中断控制器的`mtime`寄存器以获取系统当前时钟周期数
 * 
 * @note `mtime`寄存器中的值由硬件改变, 软件只需要读取即可
 * @note 由于该函数会被频繁的调用, 因此最好声明为内联函数
 * @note `get_cycle`函数的行为由`constrains.h`中的`RDTIME_SUPPORT`宏决定
 * 
 * @return uint64_t 当前系统的时钟周期数
 */
static inline uint64_t get_cycle(void){
    return read_64_bits(CLINT_TIMER_MTIME_ADDR);
}
#endif

/**
 * @brief `reset_timer`函数用于在初始化/时钟中断发生后重新设置下一次时钟中断发生的时钟周期
 * 
 * @note `RISC-V`的时钟中断由`mtime`寄存器和`mtimecmp`寄存器完成. 
 *  `mtime`寄存器保存系统上电后一直到现在的时钟周期数, 值由硬件自动增加,
 *  `mtimecmp`寄存器保存一个常数值, 值由用户通过读写`mtimecmp`寄存器完成.
 *  当`mtime`寄存器的值大于`mtimecmp`寄存器的值之后, 就会通知当前HART发生了一个时钟中断.
 *  因此, `reset_timer`寄存器其实就是重新设置`mtimecmp`寄存器的值
 * 
 * @note 假设驱动`mtime`的时钟频率是10GHz（目前还没有哪个CPU主频能到10GHz的吧），那么要让64位的mtime溢出，那么需要
 *  `0x10000000000000000 / 10000000000 / 60 / 60 / 24 / 365`年，也就是58年。
 *  `HiFive1`开发板驱动`mtime`的时钟频率是`32768Hz`，那么需要17851025年`mtime`才会溢出
 */
void reset_timer(void);

/// @brief `ktimer_init`是时钟的初始化函数
void ktimer_init(void);

/**
 * @brief `ktimer_interrupt_handler`是S模式下的时钟中断处理函数
 * 
 * @param ktf_ptr 陷入帧, S模式下中断触发后在`ktrap_enter`中构建
 * @return int64_t 处理结果, 若为0则表示处理正常, -1表示处理失败
 */
int64_t ktimer_interrupt_handler(ktrapframe_t *kft_ptr);

#endif