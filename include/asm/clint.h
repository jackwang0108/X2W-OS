/**
 * @file clint.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `clint.h`是RISC-V本地中断控制器
 * @version 0.1
 * @date 2023-05-10
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef __INCLUDE_ASM_CLINT_H
#define __INCLUDE_ASM_CLINT_H


/**
 * @brief `CLINT`中断控制器的寄存器`MMIO`后的地址
 * 
 * @note 该值参考`SiFive`手册`P80`的`Memory Map`
 */
#define CLINT_BASE_ADDR                 0x2000000

/**
 * @brief `CLINT`中断控制器的`mtime`寄存器的地址, 所有的`HART`共享这个寄存器
 * 
 * @note 该值参考`SiFive`手册`P176`的`CLINT Memory Map`
 */
#define CLINT_TIMER_MTIME_ADDR          (CLINT_BASE_ADDR + 0xBFF8)

/**
 * @brief `CLINT`中断控制器的`mtimecmp`寄存器的地址, 每个`HART`都有自己的`mtimecmp`寄存器, 这个地址是`HART0`的`mtimecmp`寄存器的地址
 * 
 * @note 该值参考`SiFive`手册`P176`的`CLINT Memory Map`
 */
#define CLINT_TIMER_MTIMECMP_ADDR       (CLINT_BASE_ADDR + 0x4000)


#endif