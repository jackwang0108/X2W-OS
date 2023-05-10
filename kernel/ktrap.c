/**
 * @file ktrap.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `ktrap.c`是内核的异常/中断管理模块
 * @version 0.1
 * @date 2023-04-20
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "kernel/ktrap.h"
#include "kernel/ktimer.h"
#include "kernel/kstdio.h"


// 中断的提示信息
const char *kintr_msg[MAX_INTR_EXCP_INFO_NUM] = {
    "Undelegated Interrupt",
    "Supervisor Software Interrupt",
    "Undelegated Interrupt",
    "Undelegated Interrupt",
    "Undelegated Interrupt",
    "Supervisor Timer Interrupt",
    "Undelegated Interrupt",
    "Undelegated Interrupt",
    "Undelegated Interrupt",
    "Supervisor External Interrupt",
    "Undelegated Interrupt",
    "Undelegated Interrupt"
};

// 异常的提示信息
const char *kexcp_msg[MAX_INTR_EXCP_INFO_NUM] = {
    "Instrution Address Misaligned",
    "Undelegated Exception",
    "Undelegated Exception",
    "Breakpoint",
    "Undelegated Exception",
    "Load access Fault",
    "Undelegated Exception",
    "Store/AMO Access Fault",
    "Environment Call from U-Mode",
    "Undelegated Exception",
    "Undelegated Exception",
    "Undelegated Exception",
    "Instruction Page Fault",
    "Load Page Fault",
    "Undelegated Exception",
    "Store/AMO Page Fault"
};

// 中断处理函数表
ktrap_handler_t intr_handlers[MAX_INTR_EXCP_INFO_NUM];

// 异常处理函数表
ktrap_handler_t excp_handlers[MAX_INTR_EXCP_INFO_NUM];


void ktrap_init(void){
    // 设置中断向量地址, 设置为直接模式
    write_csr(stvec, ((addr_t)ktrap_enter & (~((addr_t)TVEC_TRAP_DIRECT))));
    // 开启所有的中断
    write_csr(sie, -1);
    // 为所有的异常和中断注册处理函数
    for (size_t i = 0; i < MAX_INTR_EXCP_INFO_NUM; i++)
        register_ktrap_handler(i, False, NULL, general_ktrap_handler);
    for (size_t i = 0; i < MAX_INTR_EXCP_INFO_NUM; i++)
        register_ktrap_handler(i, True, NULL, general_ktrap_handler);
}


void ktrap_dispatcher(ktrapframe_t *ktf_ptr){
    ireg_t scause = read_csr(scause);

    Bool is_interrupt = ((scause & CAUSE_INTERRUPT_FLAG) != 0) ? 1 : 0;
    uint64_t trap_code = scause & ~(CAUSE_INTERRUPT_FLAG);
    int64_t rtval UNUSED = (is_interrupt ? intr_handlers : excp_handlers)[trap_code](ktf_ptr);
}


NO_RETURN int64_t general_ktrap_handler(ktrapframe_t *ktf_ptr){
    ireg_t scause = read_csr(scause);
    Bool is_interrupt = ((scause & CAUSE_INTERRUPT_FLAG) != 0) ? 1 : 0;
    uint64_t trap_code = scause & ~(CAUSE_INTERRUPT_FLAG);
    const char **msg_source = (is_interrupt ? kintr_msg : kexcp_msg);
    const char *msg = msg_source[trap_code];
    const char *s = is_interrupt ? "Interrupt" : "Exception";
    kprintf("==================================================================\n");
    kprintf("Message from Kernel General Trap Handler:\n");
    kprintf("%s Happened, %s ID: %#X, scause register: %#X\n", s, s, trap_code, scause);
    kprintf("Detailed Message: %s\n", msg);
    kprintf("No %s handler register for this %s, with ID: %#X\n", s, s, trap_code);
    kprintf("Running %s instead\n", __func__);
    kprintf("Printing Trap Frame:\n");
    print_ktrapframe(ktf_ptr, kprintf);
    kprintf("Please register handler to avoid hanging!\n");
    kprintf("Hanging HART Here\n");
    while (1);
    UNREACHABLE;
}


void register_ktrap_handler(uint64_t trap_code, Bool interrupt, const char* msg, ktrap_handler_t trap_func){
    if (msg != NULL)
        (interrupt ? kintr_msg : kexcp_msg)[trap_code] = msg;
    (interrupt ? intr_handlers : excp_handlers)[trap_code] = trap_func;
}