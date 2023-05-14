/**
 * @file ktimer.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ktimer.h`是时钟功能模块的内核部分代码
 * @version 0.1
 * @date 2023-05-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "sbi/sbi.h"
#include "asm/csr.h"
#include "kernel/ktimer.h"
#include "kernel/kstdio.h"

uint64_t volatile ALIGN64 ticks;

void reset_timer(void){
    sbi_settimer(get_cycle() + CLINT_TIMER_BASE_FRQENCY / CLINT_TIMER_FREQUENCY_HZ);
    // 打开Supervisor模式下的时钟中断
    set_csr(sie, SIE_S_TIMER_INTERRUPT);
}

void ktimer_init(void){
    reset_timer();
}

int64_t ktimer_interrupt_handler(ktrapframe_t *kft_ptr){
    // 关闭Supervisor模式下的时钟中断, 避免S模式下的时钟中断嵌套
    clear_csr(sie, SIE_S_TIMER_INTERRUPT);
    // 重新设置mtimecmp寄存器
    reset_timer();
    ticks++;
    // kprintf("Core0 Timer Interrupt, ticks=%lu\r\n", ticks);
    return 0;
}