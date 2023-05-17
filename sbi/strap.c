/**
 * @file strap.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `strap.c`是`SBI`的异常/中断管理模块
 * @version 0.1
 * @date 2023-04-16
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "string.h"
#include "asm/csr.h"
#include "sbi/strap.h"
#include "sbi/stimer.h"
#include "sbi/sstdio.h"
#include "device/uart.h"

// 中断的提示信息
const char *sintr_msg[MAX_INTR_EXCP_INFO_NUM] = {
    "User Software Interrupt",
    "Supervisor Software Interrupt",
    "Reserved for Future Standard Use",
    "Machine Software Interrupt",
    "User Timer Interrupt",
    "Supervisor Timer Interrupt",
    "Reserved for Future Standard Use",
    "Machine Timer Interrupt",
    "User External Interrupt",
    "Supervisor External Interrupt",
    "Reserved for Future Standard Use",
    "Machine External Interrupt"
};

// 异常的提示信息
const char *sexcp_msg[MAX_INTR_EXCP_INFO_NUM] = {
    "Instrution Address Misaligned",
    "Instrution Access Fault",
    "Illegal Instruction",
    "Breakpoint",
    "Load address Misaligned",
    "Load access Fault",
    "Store/AMO address Misaligned",
    "Store/AMO Access Fault",
    "Environment Call from U-Mode",
    "Environment Call from S-Mode",
    "Reserved",
    "Environment Call from M-Mode",
    "Instruction Page Fault",
    "Load Page Fault",
    "Reserved for Future Standard Use",
    "Store/AMO Page Fault"
};

// 中断处理函数表
strap_handler_t intr_handlers[MAX_INTR_EXCP_INFO_NUM];

// 异常处理函数表
strap_handler_t excp_handlers[MAX_INTR_EXCP_INFO_NUM];

void strap_init(void){
    // 设置中断向量地址, 设置为直接模式
    write_csr(mtvec, ((addr_t)strap_enter & (~((addr_t)TVEC_TRAP_DIRECT))));
    // 关闭所有的中断
    write_csr(mie, 0);
    // 为所有的异常和中断注册处理函数
    for (size_t i = 0; i < MAX_INTR_EXCP_INFO_NUM; i++)
        regitser_strap_handler(i, False, NULL, general_strap_handler);
    for (size_t i = 0; i < MAX_INTR_EXCP_INFO_NUM; i++)
        regitser_strap_handler(i, True, NULL, general_strap_handler);
    // 注册M模式下的时钟中断处理函数
    regitser_strap_handler(CAUSE_INTERRUPT_M_TIMER_INTERRUPT, True, "Machine Timer Interrupt", stimer_interrupt_handler);
}


void delegate_traps(void){
    // 将S模式下的发生的:
    //      1. 软件中断
    //      2. 时钟中断
    //      3. 外部中断
    // 委托给S模式, 即在S模式下发生了这些中断, 则进入到S模式的陷入入口(中断处理程序)
    ireg_t delegated_interrupts = \
                                MIP_S_SOFTWARE_INTERRUPT    | 
                                MIP_S_TIMER_INTERRUPT       |
                                MIP_S_EXTERNAL_INTERRUPT;
    // 将S模式下的发生的:
    //      1. 指令未对齐异常
    //      2. 读取指令页异常
    //      3. 读取数据页异常
    //      4. 存储数据页异常
    //      5. 读取数据异常
    //      6. 存储数据异常
    //      7. 断点异常
    //      8. 非法指令
    //      9. U模式下的ecall异常
    // 委托给S模式, 即在S模式下发生了这些异常, 则进入到S模式的陷入入口(异常处理程序)
    ireg_t delegated_exceptions = \
                                (1UL << CAUSE_EXCEPTION_MISALIGNED_FETCH)                   |
                                (1UL << CAUSE_EXCEPTION_FETCH_PAGE_FAULT)                   |
                                (1UL << CAUSE_EXCEPTION_LOAD_PAGE_FAULT)                    |
                                (1UL << CAUSE_EXCEPTION_STORE_PAGE_FAULT)                   |
                                (1UL << CAUSE_EXCEPTION_LOAD_ACCESS)                        |
                                (1UL << CAUSE_EXCEPTION_STORE_ACCESS)                       |
                                (1UL << CAUSE_EXCEPTION_BREAKPOINT)                         |
                                (1UL << CAUSE_EXCEPTION_ILLEGAL_INSTRUCTION)                |
                                (1UL << CAUSE_EXCEPTION_USER_ECALL);
    write_csr(mideleg, delegated_interrupts);
    write_csr(medeleg, delegated_exceptions);
}


void strap_dispatcher(strapframe_t *stf_ptr){
    ireg_t mcause = read_csr(mcause);

    Bool is_interrupt = ((mcause & CAUSE_INTERRUPT_FLAG) != 0) ? 1 : 0;
    uint64_t trap_code = mcause & ~(CAUSE_INTERRUPT_FLAG);
    int64_t rtval UNUSED = (is_interrupt ? intr_handlers : excp_handlers)[trap_code](stf_ptr);
}


NO_RETURN int64_t general_strap_handler(strapframe_t *stf_ptr){
    ireg_t mcause = read_csr(mcause);
    Bool is_interrupt = ((mcause & CAUSE_INTERRUPT_FLAG) != 0) ? 1 : 0;
    uint64_t trap_code = mcause & ~(CAUSE_INTERRUPT_FLAG);
    const char **msg_source = (is_interrupt ? sintr_msg : sexcp_msg);
    const char *msg = msg_source[trap_code];
    const char *s = is_interrupt ? "Interrupt" : "Exception";
    bprintf("==================================================================\n");
    bprintf("Message from SBI General Trap Handler:\n");
    bprintf("%s Happened, %s ID: %#X, mcause register: %#X\n", s, s, trap_code, mcause);
    bprintf("Detailed Message: %s\n", msg);
    bprintf("No %s handler register for this %s, with ID: %#X\n", s, s, trap_code);
    bprintf("Running %s instead\n", __func__);
    bprintf("Printing Trap Frame:\n");
    print_strapframe(stf_ptr, bprintf);
    bprintf("Please register handler to avoid hanging!\n");
    bprintf("Hanging HART Here\n");
    while (1);
    UNREACHABLE;
}


void regitser_strap_handler(uint64_t trap_code, Bool interrupt, const char* msg, strap_handler_t trap_func){
    if (msg != NULL)
        (interrupt ? sintr_msg : sexcp_msg)[trap_code] = msg;
    (interrupt ? intr_handlers : excp_handlers)[trap_code] = trap_func;
}