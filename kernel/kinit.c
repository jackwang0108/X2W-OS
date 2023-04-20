/**
 * @file init.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kinit.c`对内核进行初始化
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "kernel/ktrap.h"
#include "kernel/kinit.h"


void kinit_all(void){
    // uart_init();
    ktrap_init();
}