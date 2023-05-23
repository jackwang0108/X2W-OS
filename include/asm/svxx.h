/**
 * @file svxx.h
 * @author Shihong Wang
 * @brief `svxx.h`中定义了`RISC-V`的分页机制所使用的宏
 * @version 0.1
 * @date 2023-05-19
 * 
 * @copyright Copyright Zhuiri Xiao, Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_ASM_SVXX_H
#define __INCLUDE_ASM_SVXX_H

#include "types.h"
#include "constrains.h"


// **********************************************************************************************************
// * 页表项属性定义
// **********************************************************************************************************

/**
 * 目前页表项的属性有:
 *      - Bit[0]     V (Valid)                     : 有效位, 该页表项是否有效
 *      - Bit[1]     R (Read)                      : 可读位, 该页表项映射的页是否可读
 *      - Bit[2]     W (Write)                     : 可写位, 该页表项映射的页是否可写
 *      - Bit[3]     X (Executable)                : 可执行位, 该页表项映射的页是否可执行
 *      - Bit[4]     U (User)                      : U模式是否可以访问该页
 *      - Bit[5]     G (Global)                    : 全局属性, 给TLB使用
 *      - Bit[6]     A (Access)                    : 访问位, 该页表是否被处理器访问过
 *      - Bit[7]     D (Dirty)                     : 脏位, 该页表是否被修改过
 *      - Bit[8:9]   RSW (Reserved for Software)   : 预留给操作系统使用
 */

/// `page_property_t`是页目录表项/页表项属性结构体
typedef struct __page_property_t {
    uint64_t val;
} page_property_t;


/**
 * @brief `PAGE_ATTR_VALID`位用于表示页表项是否有效, 即页表项表示的页表是否在内存中. `PAGE_ATTR_VALID`位值为:
 *  - 0: 页表项有效, 即所表示的页表在内存中
 *  - 1: 页表项无效, 即所表示的页表不在内存中
 */
#define PAGE_ATTR_VALID             (1 << 0)

/**
 * @brief `is_valid_page`用于判断给定的全局/中间页目录表项或页表项是否有效
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`有效, `False`无效
 */
static inline Bool is_valid_page(uint64_t val){
    return ((val & PAGE_ATTR_VALID) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_READ`位用于表示页表项表示的页表是否可读. `PAGE_ATTR_READ`位值为:
 *  - 0: 页表项所表示的页表不可读
 *  - 1: 页表项所表示的页表可读
 */
#define PAGE_ATTR_READ              (1 << 1)

/**
 * @brief `is_readable_page`用于判断给定的全局/中间页目录表项或页表项是否可读
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`可读, `False`不可读
 */
static inline Bool is_readable_page(uint64_t val){
    return ((val & PAGE_ATTR_READ) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_WRITE`位用于表示页表项表示的页表是否可写. `PAGE_ATTR_WRITE`位值为:
 *  - 0: 页表项所表示的页表不可写
 *  - 1: 页表项所表示的页表可写
 */
#define PAGE_ATTR_WRITE             (1 << 2)

/**
 * @brief `is_writable_page`用于判断给定的全局/中间页目录表项或页表项是否可写
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`可写, `False`不可写
 */
static inline Bool is_writable_page(uint64_t val){
    return ((val & PAGE_ATTR_WRITE) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_EXEC`位用于表示页表项表示的页表是否可执行. `PAGE_ATTR_EXEC`位值为:
 *  - 0: 页表项所表示的页表不可执行
 *  - 1: 页表项所表示的页表可执行
 */
#define PAGE_ATTR_EXEC              (1 << 3)

/**
 * @brief `is_executable_page`用于判断给定的全局/中间页目录表项或页表项是否可执行
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`可执行, `False`不可执行
 */
static inline Bool is_executable_page(uint64_t val){
    return ((val & PAGE_ATTR_EXEC) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_USER`位用于表示页表项表示的页表是否是用户可访问. `PAGE_ATTR_USER`位值为:
 *  - 0: 页表项所表示的页表用户不可访问
 *  - 1: 页表项所表示的页表用户可以访问
 */
#define PAGE_ATTR_USER              (1 << 4)

/**
 * @brief `is_user_page`用于判断给定的全局/中间页目录表项或页表项用户是否可访问
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`用户可访问, `False`用户不可访问
 */
static inline Bool is_user_page(uint64_t val){
    return ((val & PAGE_ATTR_USER) == 0) ? False : True;
}

#define PAGE_ATTR_GLOBAL            (1 << 5)

/**
 * @brief `is_global_page`用于判断给定的全局/中间页目录表项或页表项用户是否是全局页
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`是全局页, `False`用户不是全局页
 */
static inline Bool is_global_page(uint64_t val){
    return ((val & PAGE_ATTR_GLOBAL) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_ACCESS`位用于表示页表项表示的页表`CPU`是否访问过, 用于缺页中断的页面置换算法. `PAGE_ATTR_ACCESS`位值为:
 *  - 0: 页表项所表示的页表`CPU`先前未访问过
 *  - 1: 页表项所表示的页表`CPU`先前访问过
 */
#define PAGE_ATTR_ACCESS            (1 << 6)

/**
 * @brief `is_accessed_page`用于判断给定的全局/中间页目录表项或页表项是否倍访问过
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`被访问过, `False`未被访问过
 */
static inline Bool is_accessed_page(uint64_t val){
    return ((val & PAGE_ATTR_ACCESS) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_DIRTY`位用于表示页表项表示的页表是否被修改过, 用于缺页中断的页面置换算法. `PAGE_ATTR_DIRTY`位值为:
 *  - 0: 页表项所表示的页表`CPU`未被修改过
 *  - 1: 页表项所表示的页表`CPU`被修改过
 */
#define PAGE_ATTR_DIRTY             (1 << 7)

/**
 * @brief `is_dirty_page`用于判断给定的全局/中间页目录表项或页表项是否被修改过
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`被访问过, `False`未被访问过
 */
static inline Bool is_dirty_page(uint64_t val){
    return ((val & PAGE_ATTR_DIRTY) == 0) ? False : True;
}

/**
 * @brief `PAGE_ATTR_SOFTWARE`位预留给操作系统使用. 
 */
#define PAGE_ATTR_SOFTWARE          (3 << 8)

/**
 * @brief `PAGE_ATTR_LEAF`表示一个页是否为子页
 */
#define PAGE_ATTR_LEAF              (PAGE_ATTR_READ | PAGE_ATTR_WRITE | PAGE_ATTR_EXEC)

/**
 * @brief `is_leaf_page`用于判断给定的全局/中间页目录表项或页表项是否为子页
 * 
 * @param val 全局页目录表项/中间页目录表项/页表项的值
 * @return Bool `True`是子页, `False`不是子页
 */
static inline Bool is_leaf_page(uint64_t val){
    return ((is_valid_page(val) && (val & PAGE_ATTR_LEAF)) == 0) ? False : True;
}


#if MEMORY_SVxx == 39

// **********************************************************************************************************
// * Sv39分页机制宏定义
// **********************************************************************************************************

/* 
 * ********************************************* Sv39分页机制介绍 *********************************************
 *
 *  Sv39使用三级页表进行虚拟地址转换, 其中:
 *      1. 第2级页表, 称为全局页目录表 (Page Global Directory, PGD). 其中每一项都是PMD的地址
 *      2. 第1级页表, 称为中间页目录表 (Page Middle Directory, PMD). 其中每一项都是PT的地址
 *      3. 第0级页表, 即页表 (Page Table, PT).                      其中每一项都是页的地址
 *  
 *  注意:
 *      1. 全局页目录表整个系统(每个进程)只有一个. 全局页目录表的地址在开启Sv39分页机制后保存在satp寄存器中.
 *      2. 中间页目录表和页表整个系统(每个进程)可能有多个, 具体数量根据需要在会在程序运行中动态分配.
 *      3. 由于RV64中每个地址都是64位的, 即每个地址都是8个字节的, 所以PGD/PMD/PT中的每一个表项的都占用8个字节
 * 
 *  PS: 为了描述方便, 代码中不少地方将全局页目录表简称为页目录表
 * 
 * 
 * 
 *  全局页目录表 (Page Global Directory, PGD) 结构如下:
 *      Byte Offset                                     Index
 *              0       .-----------------------,           0
 *                      |       PGD Entry 0     |
 *              8       |-----------------------|           1
 *                      |            .          |
 *            ...       |            .          |         ...
 *                      |            .          |
 *          8 * x       |-----------------------|           x
 *                      |       PGD Entry x     |
 *    8 * (x + 1)       |-----------------------|       x + 1
 *                      |            .          |
 *            ...       |            .          |         ...
 *                      |            .          |
 *          8 * N       |-----------------------|           N
 *                      |       PGD Entry N     |
 *                      '-----------------------'
 * 
 *  全局页目录表中的表项称为全局页目录表表项(Page Global Directory Entry, PGD_Entry), 其结构如下:
 *      Bit Offset:      63                   54 53                               10 9                4  3   2   1   0
 *                       .---------------------------------------------------------------------------------------------,
 *                       |       Reserved       |              PMD PFN              |     Reserved     | X | R | W | V |
 *                       '---------------------------------------------------------------------------------------------'
 *          length:               10 Bit                        44 Bit                      6 Bit        1   1   1   1
 * 
 *  其中:
 *      1. PGD/PMD/PT 默认都占整个页, 而一个页 4096B, 即 2^12B. 故 PMD 地址低12位为0, 且高 63~54 位为保留位.
 *         因此 PMD 地址只需要记录 PMD 地址的第 53~13 位, 即只需要保存 41 位即可.
 *         事实上, 由于一个 PGD_Entry 项占用 64 个Bit, 所以实际上记录了 PMD 地址的 53~10 位, 一共 44 位.
 * 
 *  
 *  
 *  中间页目录表 (Page Middle Directory, PGD) 结构如下:
 *      Byte Offset                                     Index
 *              0       .-----------------------,           0
 *                      |       PMD Entry 0     |
 *              8       |-----------------------|           1
 *                      |            .          |
 *            ...       |            .          |         ...
 *                      |            .          |
 *          8 * x       |-----------------------|           x
 *                      |       PMD Entry x     |
 *    8 * (x + 1)       |-----------------------|       x + 1
 *                      |            .          |
 *            ...       |            .          |         ...
 *                      |            .          |
 *          8 * N       |-----------------------|           N
 *                      |       PMD Entry N     |
 *                      '-----------------------'
 * 
 *  中间页目录表中的表项称为中间页目录表表项(Page Mlobal Directory Entry, PMD_Entry), 其结构如下:
 *      Bit Offset:      63                   54 53                               10 9                4  3   2   1   0
 *                       .---------------------------------------------------------------------------------------------,
 *                       |       Reserved       |               PT PFN              |     Reserved     | X | R | W | V |
 *                       '---------------------------------------------------------------------------------------------'
 *          length:               10 Bit                        44 Bit                      6 Bit        1   1   1   1
 * 
 *  其中:
 *      1. PGD/PMD/PT 默认都占整个页, 而一个页 4096B, 即 2^12B. 故 PT 地址低12位为0, 且高 63~54 位为保留位.
 *         因此 PT 地址只需要记录 PT 地址的第 53~13 位, 即只需要保存 41 位即可.
 *         事实上, 由于一个 PMD_Entry 项占用 64 个Bit, 所以实际上记录了 PT 地址的 53~10 位, 一共 44 位.
 *  
 * 
 *  
 *  页表 (Page Table, PT) 结构如下:
 *      Byte Offset                                     Index
 *              0       .-----------------------,           0
 *                      |        PT Entry 0     |
 *              8       |-----------------------|           1
 *                      |            .          |
 *            ...       |            .          |         ...
 *                      |            .          |
 *          8 * x       |-----------------------|           x
 *                      |        PT Entry x     |
 *    8 * (x + 1)       |-----------------------|       x + 1
 *                      |            .          |
 *            ...       |            .          |         ...
 *                      |            .          |
 *          8 * N       |-----------------------|           N
 *                      |        PT Entry N     |
 *                      '-----------------------'
 * 
 *  页表中的表项称为页表项(Page Table Entry, PTE), 其结构如下:
 *      Bit Offset:        63 62  61 60        54 53                           10 9   8  7   6   5   4   3   2   1   0
 *                       .---------------------------------------------------------------------------------------------,
 *                       | N | PBMT |  Reserved  |         PDT Block PFN         | RSW | D | A | G | U | X | W | R | V |
 *                       '---------------------------------------------------------------------------------------------'
 *          length         1   2 Bit    7 Bit                 44 Bit              2 Bit  1   1   1   1   1   1   1   1
 * 
 *  其中:
 *      1. PGD/PMD/PT 默认都占整个页, 而一个页 4096B, 即 2^12B. 故 PT 地址低12位为0, 且高 63~54 位为保留位.
 *         因此 PT 地址只需要记录 PT 地址的第 53~13 位, 即只需要保存 41 位即可.
 *         事实上, 由于一个 PTE 项占用 64 个Bit, 所以实际上记录了 页 地址的 53~10 位, 一共 44 位.
 *  
 * 
 * 
 *  Sv39的虚拟地址结构如下:
 *      Bit Offset:      63                    39 38            30 29             21 20            12 11                      0
 *                       .-----------------------------------------------------------------------------------------------------,
 *                       |       Reserved        | PGDE索引,VPN[2] | PMDE索引,VPN[1] |  PTE索引,VPN[0] |   页内偏移, Page Offset  |
 *                       '-----------------------------------------------------------------------------------------------------'
 *          length:                25 Bit              9 Bit             9 Bit            9 Bit                12 Bit
 * 
 *  
 *  
 *  
 *  Sv39的物理地址结构
 * 
 *      Bit Offset:      63         56 55                  30 29                  21 20                  12 11                0
 *                       .-----------------------------------------------------------------------------------------------------,
 *                       |  Reserved  |        PPN[2]        |        PPN[1]        |        PPN[0]        |    Page Offset    |
 *                       '-----------------------------------------------------------------------------------------------------'
 *          length:           8 Bit             26 Bit                9 Bit                   9 Bit                12 Bit
 * 
 * 
 *  注意:
 *      1. Sv39一个页占 4 KB, 即 4096 字节. 而RV64一个地址占 64 Bit, 即 8 个字节. 因此, Sv39中, 一个页可以存储 4096/8=512 个地址. 故:
 *           - PGD 占据一个页, 可以存储    512 个  PMD     地址
 *           - PMD 占据一个页, 可以存储    512 个  PT      地址
 *           - PT  占据一个页, 可以存储    512 个  PTE     地址
 *      2. 对于一个虚拟地址而言, 
 *           - VPN[2], 即PGD Entry索引 38 - 30 + 1 = 9     Bit,    可以索引    2^9 = 512   个  PGD Entry
 *           - VPN[1], 即PMD Entry索引 29 - 21 + 1 = 9     Bit,    可以索引    2^9 = 512   个  PMD Entry
 *           - VPN[0], 即PTE索引 20 - 12 + 1 = 9     Bit,    可以索引    2^9 = 512   个  PT  Entry
*/


/// 全局页目录表项/中间页目录表项/页表项中的PFN偏移位数
#define PAGE_PFN_SHIFT      10

/// `PAGE_OFFSET_MASK`用于获取页内偏移
#define PAGE_OFFSET_MASK    0b0000000000000000000000000000000000000000000000000000111111111111
/// `PAGE_VPN2_MASK`用于获取虚拟地址中的VPN[2], 即PGD索引
#define PAGE_VPN2_MASK      0b0000000000000000000000000111111111000000000000000000000000000000
/// `PAGE_VPN1_MASK`用于获取虚拟地址中的VPN[1], 即PMD索引
#define PAGE_VPN1_MASK      0b0000000000000000000000000000000000111111111000000000000000000000
/// `PAGE_VPN0_MASK`用于获取虚拟地址中的VPN[0], 即PT索引
#define PAGE_VPN0_MASK      0b0000000000000000000000000000000000000000000111111111000000000000

/// `PAGE_VPN2_MASK`用于获取物理地址中的PPN[2]
#define PAGE_PPN2_MASK      0b0000000011111111111111111111111111000000000000000000000000000000
/// `PAGE_VPN1_MASK`用于获取物理地址中的PPN[1]
#define PAGE_PPN1_MASK      0b0000000000000000000000000000000000111111111000000000000000000000
/// `PAGE_VPN0_MASK`用于获取物理地址中的PPN[0]
#define PAGE_PPN0_MASK      0b0000000000000000000000000000000000000000000111111111000000000000

/* ********** PGD相关定义 ********** */ 

/// `pgd_t`是全局页目录表类型
typedef addr_t pgd_t;

/// `pgd_entry_t`是`PGD`表项结构体
typedef struct __pgd_entry_t {
    uint64_t val;
} pgd_entry_t;

#define PGD_SHIFT 30                                        /// `Sv39`三级页表中`PGD`, 全局页目录表的位偏移
#define PGD_SIZE  (1UL << PGD_SHIFT)                        /// `Sv39`三级页表中一个`PGD Entry`所能映射的内存大小
#define PGD_MASK  (~(PGD_SIZE - 1))                         /// `Sv39`虚拟地址中`PGD Entry`索引掩码
#define PGDE_PER_PGD (PAGE_SIZE / (sizeof(pgd_entry_t)))    /// `Sv39`三级页表中`PGD`中`PGD Entry`数量


/* ********** PMD相关定义 ********** */ 

/// `pmd_t`是中间页目录表类型
typedef addr_t pmd_t;

/// `pmd_entry_t`是`PMD`表项结构体
typedef struct __pmd_entry_t {
    uint64_t val;
} pmd_entry_t;

#define PMD_SHIFT 21                                    /// `Sv39`三级页表中`PMD`, 中间页目录表的位偏移
#define PMD_SIZE  (1UL << PMD_SHIFT)                    /// `Sv39`三级页表中`PMD`中巨页的大小
#define PMD_MASK  (~(PMD_SIZE - 1))                     /// `Sv39`虚拟地址中`PMD`索引掩码
#define PMDE_PER_PMD (1 << (PGD_SHIFT - PMD_SHIFT))     /// `Sv39`三级页表中`PMD`中`PMD Entry`数量


/* ********** PT相关定义 ********** */ 

/// `pt_t`是页表类型
typedef addr_t pt_t;

/// `pt_entry_t`是`PT`表项结构体
typedef struct __pt_entry_t {
    uint64_t val;
} pt_entry_t;

#define PTE_SHIFT 12                                    /// `Sv39`三级页表中`PTE`, 中间页目录表的位偏移
#define PTE_SIZE  (1UL << PTE_SHIFT)                    /// `Sv39`三级页表中`PTE`中页的大小
#define PTE_MASK  (~(PTE_SIZE - 1))                     /// `Sv39`虚拟地址中`PTE`索引掩码
#define PTE_PER_PT (1 << (PMD_SHIFT - PTE_SHIFT))     /// `Sv39`三级页表中`PTE`中`PTE Entry`数量

#endif


#endif