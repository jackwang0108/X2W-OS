/**
 * @file mm.h
 * @author Zhuir Xiao (xzr3356142450@gmail.com)
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `mm.h`是`X2WOS`的内存管理模块
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright Zhuiri Xiao, Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_MM_H
#define __INCLUDE_KERNEL_MM_H

#include "types.h"
#include "stdbitmap.h"
#include "constrains.h"
#include "asm/svxx.h"
#include "kernel/locks.h"


/**
 * @brief `ppool_t`是物理内存池(Physical Memory Pool)结构体, 内存池以页面为管理对象
 * 
 * @note 整个系统会有两个内存池, `kernel_ppool`和`user_ppool`, 由系统维护
 */
typedef struct __ppool_t {
    /// 内存池`bitmap`
    bitmap_t *btmp;
    /// 管理的物理内存起始页地址
    addr_t paddr_start;
    /// 内存池管理的总页数
    size_t size;
    /// 内存池的锁
    spinlock_t lock;
} ppool_t;


/**
 * @brief `vpool_t`是虚拟内存池(Virtual Memory Pool)结构体
 * 
 * @brief 系统中可能存在多个虚拟内存池, 一个进程一个虚拟内存池
 */
typedef struct __vpool_t {
    /// 内存池`bitmap`
    bitmap_t *btmp;
    /// 管理的虚拟内存起始页地址
    addr_t vaddr_start;
    /// 内存池的锁
    spinlock_t lock;
} vpool_t;


/**
 * @brief `get_pgd`用于获得当前正在运行的进程的全局页目录表
 * 
 * @return pgd_t* 当前正在运行进程的全局页目录表
 */
pgd_t *get_pgd(void);

/**
 * @brief `get_ppool`用于获得当前正在运行的进程的物理内存池
 * 
 * @return ppool_t* 当前正在运行进程的物理内存池
 * 
 * @note 目前仅支持内核线程, 返回用户进程的物理内存池需要在未来实现了用户线程之后实现
 */
ppool_t *get_ppool(void);


/**
 * @brief `get_vpool`用于获得当前正在运行的进程的虚拟内存池
 * 
 * @return vpool_t* 当前正在运行进程的虚拟内存池
 * 
 * @note 目前仅支持内核线程, 返回用户进程的虚拟内存池需要在未来实现了用户线程之后实现
 */
vpool_t *get_vpool(void);


/**
 * @brief `init_pools`用于初始化内存池. 具体来说, 该函数会:
 *      1. 初始化系统物理内存池
 *      2. 初始化用户物理内存池
 *      3. 初始化系统虚拟内存池
 * 
 * @note 该函数将会按照`constrain.h`中定义`MEMORY_US_RATIO`定义的用户和系统占用内存比例
 * 
 * @param start_paddr 物理页起始地址
 * @param total_pages 系统可用物理内存页数
 */
void init_pools(addr_t start_paddr, size_t total_pages);

/**
 * @brief `memory_init`是内存管理模块的初始化函数
 * 
 * @param start_paddr 内存起始物理地址
 * @param end_paddr 内存结束物理地址
 */
void memory_init(addr_t start_paddr, addr_t end_paddr);


/**
 * @brief `alloc_ppage`用于分配一个物理页
 * 
 * @param kpage 是否分配内核物理页
 * @return addr_t 物理页地址
 * 
 * @note 目前如果物理页满了的话, 那么就会警告挂起内核; 未来实现换页机制的时候这里需要修改
 * 
 * @note 由于虚拟页和物理页的映射是乱序的, 所以没必要分配连续的物理页, 一次分配一个就行
 */
addr_t alloc_ppage(Bool kpage);


/**
 * @brief `free_ppage`用于释放一个物理页
 * 
 * @param ppage 要释放的物理页
 * @param kpage 是否释放物理页
 */
void free_ppage(addr_t ppage, Bool kpage);


/**
 * @brief `alloc_vpage`用于从`vpool`指向的虚拟内存池中分配连续`cnt`个虚拟页
 * 
 * @param vpool 虚拟内存池
 * @param cnt 连续分配的虚拟页数量
 * @return addr_t 虚拟页地址
 * 
 * @note 目前没法找到连续`cnt`个虚拟页的话, 那么就会警告挂起内核; 未来实现换页机制的时候这里需要修改
 * 
 * @note 虽然虚拟页和物理页的映射是乱序的, 但是用户可能一次会`malloc`多个虚拟页, 所以需要分配虚拟页的函数需要支持一次分配多个虚拟页
 */
addr_t alloc_vpage(vpool_t *vpool, size_t cnt);


/**
 * @brief `free_vpage`用于释放虚拟页`vpage`到虚拟内存池`vpool`中
 * 
 * @param vpool 虚拟内存池
 * @param vpage 要释放的虚拟页
 */
void free_vpage(vpool_t *vpool, addr_t vpage);


/**
 * @brief `malloc_page`将从指定的内存池中分配`cnt`个连续的页
 * 
 * @param cnt 分配的连续的页面数
 * @param kpage 是否分配内核页
 * @return void* 分配得到的页地址
 * 
 * @note 该函数将会:
 *      1. 从指定的物理内存池分配一个页
 *      2. 从虚拟内存池分配一个页
 *      3. 在页目录表中完成虚拟页和物理页的映射
 */
void *malloc_page(size_t cnt, Bool kpage);


#endif