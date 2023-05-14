/**
 * @file plic.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `plic.h`定义了`RISC-V`的`PLIC`全局中断控制器的相关常量和宏
 * @version 0.1
 * @date 2023-05-13
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_ASM_PLIC_H
#define __INCLUDE_ASM_PLIC_H


#include "types.h"
#include "constrains.h"


/**
 * @brief `PLIC`中断控制器的寄存器`MMIO`后的内存地址起始部分
 * 
 * @note 该值参考`SiFive FU740`手册`P80`的`Memory Map`
 */
#define PLIC_BASE_ADDR                          0xC000000UL

/**
 * @brief `PLIC`中断控制器的寄存器`MMIO`后的内存地址终止部分
 * 
 * @note 该值参考`SiFive FU740`手册`P80`的`Memory Map`
 */
#define PLIC_END_ADDR                           0x10000000UL


/**
 * @brief `plic_priority_addr`用于计算给定中断号的优先级寄存器的`MMIO`地址
 * 
 * @param hwiid 硬件中断号, hardware interrupt id
 * @return addr_t 给定中断号的优先级寄存器的`MMIO`地址
 * 
 * @note 该值参考`SiFive FU740`手册`P180`的`PLIC Memory Map`
 */
static inline addr_t plic_priority_addr(uint32_t hwiid){
    return (addr_t) (PLIC_BASE_ADDR + hwiid * 4);
}

/**
 * @brief `plic_pending_addr`用于计算给定硬件中断号的中断待定寄存器的`MMIO`地址
 * 
 * @param hwiid 硬件中断号, hardware interrupt id
 * @return addr_t 给定中断号的中断待定器的`MMIO`地址
 * 
 * @note 该值参考`SiFive FU740`手册`P180`的`PLIC Memory Map`
 */
static inline addr_t plic_pending_addr(uint32_t hwiid){
    return (addr_t) (PLIC_BASE_ADDR + 0x1000 + (hwiid / 32) * 4);
}

/**
 * @brief `plic_enable_addr`用于计算给定`hart`的中断使能寄存器的`MMIO`地址
 * 
 * @param hid 核心的编号, 即hart id
 * @param m_mode 核心的模式, True/False
 * @return addr_t 给定核心指定模式的中断使能寄存器的`MMIO`地址
 * 
 * @note 该值参考`SiFive FU740`手册`P180`的`PLIC Memory Map`
 * @note 每个核心都有对应的中断使能寄存器, S模式需要加0x20的偏移
 */
static inline addr_t plic_enable_addr(uint64_t hid, Bool m_mode){
    offset_t offset = (m_mode == True) ? 0x2000 : 0x2080;
    return (addr_t) (PLIC_BASE_ADDR + offset + hid * 0x80);
}

/**
 * @brief `plic_threshold_addr`用于计算给定`hart`的中断优先级阈值寄存器的`MMIO`地址
 * 
 * @param hid 核心的编号, 即hart id
 * @param m_mode 核心的模式, True/False
 * @return addr_t 给定核心指定模式的中断优先级阈值寄存器的`MMIO`地址
 * 
 * @note 该值参考`SiFive FU740`手册`P180`的`PLIC Memory Map`
 * @note 每个核心都有对应的中断优先级阈值寄存器, S模式需要加0x1000的偏移
 */
static inline addr_t plic_threshold_addr(uint64_t hid, Bool m_mode){
    offset_t offset = (m_mode == True) ? 0x200000 : 0x201000;
    return (addr_t) (PLIC_BASE_ADDR + offset + hid * 0x1000);
}

/**
 * @brief `plic_claim_addr`用于计算给定`hart`的中断请求寄存器的`MMIO`地址
 * 
 * @param hid 核心的编号, 即hart id
 * @param m_mode 核心的模式, True/False
 * @return addr_t 给定核心指定模式的中断请求寄存器的`MMIO`地址
 * 
 * @note 该值参考`SiFive FU740`手册`P180`的`PLIC Memory Map`
 * @note 每个核心都有对应的中断请求寄存器, S模式需要加0x1000的偏移
 */
static inline addr_t plic_claim_addr(uint64_t hid, Bool m_mode){
    return plic_threshold_addr(hid, m_mode) + 0x04;
}

/**
 * @brief `plic_complete_addr`用于计算给定`hart`的中断完成寄存器的`MMIO`地址
 * 
 * @param hid 核心的编号, 即hart id
 * @param m_mode 核心的模式, True/False
 * @return addr_t 给定核心指定模式的中断完成寄存器的`MMIO`地址
 * 
 * @note 该值参考`SiFive FU740`手册`P180`的`PLIC Memory Map`
 * @note 每个核心都有对应的中断完成寄存器, S模式需要加0x1000的偏移
 * @note 中断完成寄存器其实就是中断请求寄存器
 */
static inline addr_t plic_complete_addr(uint64_t hid, Bool m_mode){
    return plic_claim_addr(hid, m_mode);
}


#endif