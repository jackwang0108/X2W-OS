/**
 * @file stimer.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stimer.c`是时钟功能模块的SBI部分代码
 * @version 0.1
 * @date 2023-05-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "io.h"
#include "asm/csr.h"
#include "asm/clint.h"
#include "sbi/strap.h"
#include "sbi/stimer.h"
#include "sbi/sstdio.h"


void stimer_init(void){
    regitser_strap_handler(CAUSE_INTERRUPT_M_TIMER_INTERRUPT, True, "Machine Timer Interrupt", stimer_interrupt_handler);
}

int64_t stimer_interrupt_handler(strapframe_t *stf_ptr){
    // bprintf("Machine Timer Interrupt Happened!\n");
    // 禁止M模式下的时钟中断, 而后开启S模式下的timer pending中断
    /*
     * 一个中断只有在以下情况下才会进入到M模式处理:
     *      1. 处于M模式下, 然后发生了中断且mstatus寄存器中的MIE位设置了, mip和mie寄存器中的对应位设置了 
     *      2. 处于其他模式下, 而后发生了中断, 且mstatus寄存器中的MIE位设置了, mip和mie寄存器中的对应位设置了, 并且没有设置mideleg寄存器
     * 所以这里通过设置mie寄存器, 直接关闭M模式下的时钟中断.
     * 而S模式下的时钟中断由于已经设置了mideleg寄存器委托出去了, 所以只能设置MIP的STIP位来控制时钟中断
    */
    clear_csr(mie, MIE_M_TIMER_INTERRUPT);
    set_csr(mip, MIP_S_TIMER_INTERRUPT);
    return 0;
}


void clint_timer_event_start(uint64_t next_ticks){
    write_64_bits(CLINT_TIMER_MTIMECMP_ADDR, next_ticks);
    // 清除S模式下的timer pending终端, 而后打开M模式下的时钟中断
    clear_csr(mip, MIP_S_TIMER_INTERRUPT);
    set_csr(mie, MIE_M_TIMER_INTERRUPT);
}