/**
 * @file sinit.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `sinit.c`对`SBI`进行初始化
 * @version 0.1
 * @date 2023-04-14
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "sbi/sinit.h"


void sinit_all(void){
    uart_init();
}