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
    // 禁止M模式下的时钟中断, 而后开启S模式下的timer pending中断, 即中断注入, 相当于手动触发S模式的时钟中断
    clear_csr(mie, MIE_M_TIMER_INTERRUPT);
    set_csr(mip, MIP_S_TIMER_INTERRUPT);
    return 0;
}


void clint_timer_event_start(uint64_t next_ticks){
    write_64_bits(CLINT_TIMER_MTIMECMP_0_ADDR, next_ticks);
    // 清除S模式下的timer pending终端, 而后打开M模式下的时钟中断
    clear_csr(mip, MIP_S_TIMER_INTERRUPT);
    set_csr(mie, MIE_M_TIMER_INTERRUPT);
}