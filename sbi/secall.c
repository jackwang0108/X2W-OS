/**
 * @file secall.c
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `secall.c`是`SBI`调用的实现
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */


#include "sbi/secall.h"


void ecall_init(void){
    regitser_trap_handler(MCAUSE_SUPERVISOR_ECALL, False, NULL, sup_ecall_handler);
}

int64_t sup_ecall_handler(strapframe_t *stf_ptr){
    // ecall id 保存在 a7 寄存器中
    ireg_t arg0 UNUSED = stf_ptr->a0;
    ireg_t arg1 UNUSED = stf_ptr->a1;
    ireg_t arg2 UNUSED = stf_ptr->a2;
    ireg_t ecall_id = stf_ptr->a7;

    int64_t ret = -1;
    switch (ecall_id) {
        case SBICALL_CONSOLE_PUTCHAR:
            uart_put((char)arg0);
            ret = 0;
            break;
        case SBICALL_CONSOLE_PUTSTR:
            uart_puts((char*)arg0);
            break;
        default:
            uart_puts("Ecall Error: Non-supported ecall id!\n");
            while(1);
    }

    // ecall 为异常, 因此返回时候需要把pc的值 +4, 即指向ecall的下一条指令
    if (ret == 0)
        stf_ptr->mepc += 4;

    return ret;
}
