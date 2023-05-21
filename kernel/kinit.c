/**
 * @file init.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kinit.c`对内核进行初始化
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "device/ddr.h"
#include "device/uart.h"
#include "kernel/mm.h"
#include "kernel/ktrap.h"
#include "kernel/kinit.h"
#include "kernel/kplic.h"
#include "kernel/ktimer.h"
#include "kernel/kstdio.h"
#include "kernel/paging.h"

// _e_kernel是内存中的内核映像结束地址
extern char _e_kernel[];

#define INIT_DONE   kprintf("\tDone!\n");

void kinit_all(void){
    kprintf("=> ktrap_init\n");
    ktrap_init();
    INIT_DONE;
    kprintf("=> kplic_init\n");
    kplic_init();
    INIT_DONE;
    kprintf("=> ktimer_init\n");
    ktimer_init();
    INIT_DONE;
    kprintf("=> memory_init\n");
    memory_init((addr_t) _e_kernel, DDR_END_ADDR);
    INIT_DONE;
    kprintf("=> paging_init\n");
    paging_init();
    INIT_DONE;
}