/**
 * @file init.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `init.c`对系统进行初始化
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "init.h"


void init_all(void){
    uart_init();
}