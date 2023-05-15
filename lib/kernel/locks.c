/**
 * @file locks.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `locks.h`为操作系统提供了锁的实现
 * @version 0.1
 * @date 2023-05-15
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "kernel/ktrap.h"
#include "kernel/locks.h"


void spinlock_init(spinlock_t *lock, char *name){
    lock->locked = False;
    lock->name = name;
}

void spinlock_acquire(spinlock_t *lock){
    // 关闭中断以避免死锁
    supervisor_interrupt_disable();
    // 如果没有获得锁则循环在此
    while (__sync_lock_test_and_set(&lock->locked, True) != 0);
    /*
     * 由于RISC-V是弱内存模型, 因此内存访问指令在CPU内部是乱序执行的.
     * 所以这里可能会导致一个问题: 可能在获得锁之前就运行了临界区内的内存访问指令
     * 这里借助GCC的__sync_synchronize手动触发一个内存屏障指令(fence)
     * 内存屏障指令通知`CPU`在执行阶段后续的临界区内的内存访问指令不会在下面这个函数运行前执行
     * 从而确保临界区内的内存访问指令一定在获得锁之后执行
     * 
     * PS: RISC-V的内存模型是RVWMO, 所以其实是存在多种内存屏障的
     *     而GCC实现的__sync_synchronize是一个双向内存屏障
     */
    __sync_synchronize();
}

void spinlock_release(spinlock_t *lock){
    /*
     * 由于RISC-V是弱内存模型, 因此内存访问指令在CPU内部是乱序执行的.
     * 所以这里可能会导致一个问题: 可能在临界区内的内存访问指令在释放锁之后运行
     * 这里借助GCC的__sync_synchronize手动触发一个内存屏障指令(fence)
     * 内存屏障指令通知`CPU`在执行阶段先前临界区内的内存访问指令不会在下面这个函数运行后执行
     * 从而确保临界区内的内存访问指令一定在释放锁前运行
     * 
     * PS: RISC-V的内存模型是RVWMO, 所以其实是存在多种内存屏障的
     *     而GCC实现的__sync_synchronize是一个双向内存屏障
     */
    __sync_synchronize();
    // 释放锁
    __sync_lock_release(&lock->locked);
    // 开中断
    supervisor_interrupt_enable();
}