/**
 * @file test_strap.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_strap.c`是`strap.c`的测试文件
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "test/test_strap.h"
#include "kernel/kstdio.h"

extern NO_RETURN void trigger_load_access_fault(void);

NO_RETURN int test_starp(void){
    kprintf("=> %s:\n", __func__);
    trigger_load_access_fault();
}