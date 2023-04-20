/**
 * @file trap_entry.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `trap_entry.h`中定义了各种模式下的陷入入口函数和陷入出口函数
 * @version 0.1
 * @date 2023-04-20
 * 
 * @note 所有的陷入入口函数和陷入出口函数都定义在`trap_entry.S`中
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_TRAP_TRAP_ENTRY_H
#define __INCLUDE_TRAP_TRAP_ENTRY_H


/**
 * @brief `strap_enter`是`SBI`的陷入入口函数, 由汇编实现, 定义在 `strap_entry.S`中
 * 
 * @note 该函数会:
 *      1. 在`M模式`的栈中构建被打断程序的陷入帧
 *      2. 将被打断的程序的陷入帧作为参数调用`strap_dispatcher`函数
 */
extern void strap_enter(void);


/**
 * @brief `strap_exit`是`SBI`的陷入出口函数, 由汇编实现, 定义在 `strap_entry.S`中
 * 
 * @note 该函数会:
 *      1. 将`M模式`的栈中构建的被打断程序的陷入帧恢复到寄存器中
 */
extern void strap_exit(void);


/**
 * @brief `ktrap_enter`是内核的陷入入口函数, 由汇编实现, 定义在 `ktrap_entry.S`中
 * 
 * @note 该函数会:
 *      1. 在`S模式`的栈中构建被打断程序的陷入帧
 *      2. 将被打断的程序的陷入帧作为参数调用`ktrap_dispatcher`函数
 */
extern void ktrap_enter(void);


/**
 * @brief `ktrap_exit`是内核的陷入出口函数, 由汇编实现, 定义在 `ktrap_entry.S`中
 * 
 * @note 该函数会:
 *      1. 将`S模式`的栈中构建的被打断程序的陷入帧恢复到寄存器中
 */
extern void ktrap_exit(void);


#endif