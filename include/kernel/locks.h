/**
 * @file locks.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `locks.h`为操作系统提供了锁的实现
 * @version 0.1
 * @date 2023-05-15
 * 
 * @note 目前版本的自旋锁/互斥锁未来需要改成信号量
 * 
 * @warning 可能存在bug, 需要在完成内核线程后进行测试
 * 
 * @todo 实现信号量
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_LOCKS_H
#define __INCLUDE_KERNEL_LOCKS_H

#include "types.h"

/**
 * @brief `spinlock_t`是自旋锁类型, 是互斥锁的一种, 用于保证变量的唯一访问
 * 
 * @note 目前版本的自旋锁会浪费CPU, 未来实现信号量之后需要修改为信号量让当前线程主动让出CPU
*/
typedef struct __spinlock_t {
    /// @brief `locked`用于表示自旋锁是否被持有
    Bool locked;
    /// @brief 自旋锁的名字
    char *name;
} spinlock_t;


/**
 * @brief `spinlock_init`用于初始化自旋锁`lock`
 * 
 * @param lock 需要被初始化的自旋锁
 * @param name 自旋锁的名字
 */
void spinlock_init(spinlock_t *lock, char *name);


/**
 * @brief `spinlock_acquire`用于获得自旋锁`lock`, 若未能获得自旋锁, 则当前线程将循环在此
 * 
 * @param lock 需要获取的自旋锁
 * 
 * @note 目前版本使用`GCC`内置的`__sync_lock_test_and_set`获取锁, 避免手写`LR/SC`指令
 */
void spinlock_acquire(spinlock_t *lock);


/**
 * @brief `spinlock_release`用于释放自旋锁`lock`
 * 
 * @param lock 需要释放的自旋锁
 * 
 * @note 目前版本的使用`GCC`内置的`__sync_lock_release`释放锁, 避免手写`LR/SC`指令
 */
void spinlock_release(spinlock_t *lock);


#endif