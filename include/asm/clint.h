/**
 * @file clint.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `clint.h`定义了`RISC-V`的`CLINT`本地中断控制器的相关内容
 * @version 0.1
 * @date 2023-05-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_ASM_CLINT_H
#define __INCLUDE_ASM_CLINT_H

/**
 * @brief `CLINT`中断控制器的寄存器`MMIO`后的占用内存字节数
 * 
 * @note 该值参考`SiFive FU740`手册`P80`的`Memory Map`
 */
#define CLINT_MMIO_SIZE                 0x10000

/**
 * @brief `CLINT`中断控制器的寄存器`MMIO`后的起始地址
 * 
 * @note 该值参考`SiFive FU740`手册`P80`的`Memory Map`
 */
#define CLINT_BASE_ADDR                 0x2000000

/**
 * @brief `CLINT`中断控制器的寄存器`MMIO`后的结束地址
 * 
 * @note 该值参考`SiFive FU740`手册`P80`的`Memory Map`
 */
#define CLINT_END_ADDR                  (CLINT_BASE_ADDR + CLINT_MMIO_SIZE)

/**
 * @brief `CLINT`中断控制器的`mtime`寄存器的地址, 所有的`HART`共享这个寄存器
 * 
 * @note 该值参考`SiFive FU740`手册`P176`的`CLINT Memory Map`
 */
#define CLINT_TIMER_MTIME_ADDR          (CLINT_BASE_ADDR + 0xBFF8)

/**
 * @brief `CLINT`中断控制器的`mtimecmp0`寄存器的地址, 每个`HART`都有自己的`mtimecmp`寄存器, 这个地址是`HART0`的`mtimecmp`寄存器的地址
 * 
 * @note 该值参考`SiFive FU740`手册`P176`的`CLINT Memory Map`
 */
#define CLINT_TIMER_MTIMECMP_0_ADDR       (CLINT_BASE_ADDR + 0x4000)


#endif