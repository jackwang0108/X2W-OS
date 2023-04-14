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
#include "kernel/kinit.h"

/// `kernel_main`是内核的入口函数
void kernel_main(void);         // make gcc happy :)

void kernel_main(void){
    kinit_all();

    test_all();

    while (1);
}