/**
 * @file main.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `main.c`是内核的主函数
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "uart.h"
#include "test.h"
#include "kernel/kstdio.h"
#include "kernel/kmain.h"
#include "kernel/kinit.h"

void kernel_main(void){
    uart_puts("Jump into kernel!\n");
    kprintf(DELIMITER);
    kprintf("Start kinit_all!\n");
    kinit_all();

    kprintf(DELIMITER);
    kprintf("Start testing!\n");
    test_all();

    kprintf(DELIMITER);
    kprintf("Kernel Hanging Here!\n");
    while (1);
}