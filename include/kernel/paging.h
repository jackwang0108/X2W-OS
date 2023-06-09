/**
 * @file paging.h
 * @author Zhuir Xiao (xzr3356142450@gmail.com)
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `paging.h`是`X2WOS`的内存分页机制模块
 * @version 0.1
 * @date 2023-05-16
 * 
 * @note 目前仅支持`Sv39`的虚拟页表转换, 即目前仅支持三级页表
 * 
 * @copyright Copyright Zhuiri Xiao, Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_PAGING_H
#define __INCLUDE_KERNEL_PAGING_H

#include "types.h"
#include "constrains.h"
#include "asm/svxx.h"
#include "kernel/mm.h"
#include "kernel/ktrap.h"
#include "kernel/kdebug.h"

#ifndef PAGE_SHIFT
/// `PAGE_SHIFT`表示页内偏移在虚拟地址中占用的`Bit`数
#define PAGE_SHIFT  12
#endif

#ifndef PAGE_SIZE
/// `PAGE_SIZE`表示页的大小
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#endif


/* ---------- X2WOS中的页属性 ---------- */ 

/// 系统所有的页默认属性: 该页有效(在内存中), CPU访问过该页, 用户可访问该页
#define SYS_PAGE_ATTR_BASE              (PAGE_ATTR_VALID | PAGE_ATTR_ACCESS | PAGE_ATTR_USER)
/// 系统无效页属性
#define SYS_PAGE_ATTR_NONE              (~PAGE_ATTR_VALID)
/// 系统可读页属性
#define SYS_PAGE_ATTR_READ              (SYS_PAGE_ATTR_BASE | PAGE_ATTR_READ)
/// 系统可写页属性
#define SYS_PAGE_ATTR_WRITE             (SYS_PAGE_ATTR_BASE | PAGE_ATTR_WRITE)
/// 系统可执行页属性
#define SYS_PAGE_ATTR_EXEC              (SYS_PAGE_ATTR_BASE | PAGE_ATTR_EXEC)
/// 系统可读可执行页属性
#define SYS_PAGE_ATTR_READ_EXEC         (SYS_PAGE_ATTR_BASE | PAGE_ATTR_READ | PAGE_ATTR_EXEC)
/// 系统可写可执行页属性
#define SYS_PAGE_ATTR_WRITE_EXEC        (SYS_PAGE_ATTR_BASE | PAGE_ATTR_WRITE | PAGE_ATTR_EXEC)


#define PAGE_TABLE                      PAGE_ATTR_VALID


/// 内核页默认属性是可读, 可写, 有效, 被访问, 被写, 全局
#define KERNEL_PAGE                     (PAGE_ATTR_READ | PAGE_ATTR_WRITE | PAGE_ATTR_VALID | PAGE_ATTR_ACCESS | PAGE_ATTR_DIRTY | PAGE_ATTR_GLOBAL)
/// 只读内核页
#define KERNEL_PAGE_READ                (KERNEL_PAGE & ~PAGE_ATTR_WRITE)
/// 可执行内核页
#define KERNEL_PAGE_EXEC                (KERNEL_PAGE | PAGE_ATTR_EXEC)
/// 可读可执行页
#define KERNEL_PAGE_READ_EXEC           ((KERNEL_PAGE & ~PAGE_ATTR_WRITE) | PAGE_ATTR_EXEC)
/// 可读可写可执行页
#define KERNEL_PAGE_READ_WRITE_EXEC     (KERNEL_PAGE | PAGE_ATTR_WRITE | PAGE_ATTR_EXEC)
/// 留给软件用的内核页
#define KERNEL_PAGE_RESERVED            (PAGE_ATTR_SOFTWARE)


// `_s_kernel_pgd`是内核页目录表的起始地址, 定义在`kernel.ld`中, 在连接阶段由编译器负责填充和重定位
extern char _s_kernel_pgd[];
// `kernel_pgd`是内核页目录表, 定义在`kernel.ld`中, 即该变量的内存在编译/链接/加载阶段创建
extern pgd_t *kernel_pgd;

// **********************************************************************************************************
// * 页表转储函数
// **********************************************************************************************************

/// @brief `addr_marker_t`是地址标识符, 用于给一个地址标识其名称
typedef struct __addr_marker_t {
	addr_t start_address;
	char *name;
} addr_marker_t;

static const addr_marker_t address_markers[] = {
    {.start_address = 0,    .name = "Identical Mapping"},
};

/**
 * @brief `property_bits_t`是全局页目录表项/中间页目录表项/页表项属性中每一位的结构体
 * 
 * @note 全局页目录表项/中间页目录表项/页表项的属性都有10个`Bit`, 每个`Bit`表示不同的含义. 
 *      因此该结构体就是为每一位定义的数据类型, 详情请参考`prot_bits`数组
 */
typedef struct __property_bit_t {
	/// 属性对应的`mask`
	uint64_t mask;
	/// 属性对应的值
	uint64_t val;
	/// 属性设置时候打印的字符
	const char *set;
	/// 属性未设置时候打印的字符
	const char *clear;
} property_bit_t;

static const property_bit_t prot_bits[] = {
    /// `RS_----_----` 留给软件使用
	{ .mask = PAGE_ATTR_SOFTWARE,   .val = PAGE_ATTR_SOFTWARE,      .set = "RSW",   .clear = ".." },
    /// `--_D---_----` 表示页面是否被写过
    { .mask = PAGE_ATTR_DIRTY,      .val = PAGE_ATTR_DIRTY,         .set = "D",     .clear = "."  },
    /// `--_-A--_----` 表示页面是否被访问过
    { .mask = PAGE_ATTR_ACCESS,     .val = PAGE_ATTR_ACCESS,        .set = "A",     .clear = "."  },
    /// `--_--G-_----` 表示页面全局属性, TLB会用到
    { .mask = PAGE_ATTR_GLOBAL,     .val = PAGE_ATTR_GLOBAL,        .set = "G",     .clear = "."  },
    /// `--_---U_----` 表示页面用户是否可访问
    { .mask = PAGE_ATTR_USER,       .val = PAGE_ATTR_USER,          .set = "U",     .clear = "."  },
    /// `--_----_E---` 表示页面是否可执行
    { .mask = PAGE_ATTR_EXEC,       .val = PAGE_ATTR_EXEC,          .set = "X",     .clear = "."  },
    /// `--_----_-W--` 表示页面是否可写
    { .mask = PAGE_ATTR_WRITE,      .val = PAGE_ATTR_WRITE,         .set = "W",     .clear = "."  },
    /// `--_----_--R-` 表示页面是否可读
    { .mask = PAGE_ATTR_READ,       .val = PAGE_ATTR_READ,          .set = "R",     .clear = "."  },
    /// `--_----_---V` 表示页面是否有效
    { .mask = PAGE_ATTR_VALID,      .val = PAGE_ATTR_VALID,         .set = "V",     .clear = "."  },
};


/**
 * @brief `dump_property`用于循环检测页目录表项/页表项属性, 并输出对应的值
 * 
 * @param property 页目录表/页表项属性
 * @param prot_bits 属性位数组
 * @param num 循环次数
 */
void dump_property(uint64_t property, const property_bit_t *bits, size_t num);


/**
 * @brief `print_page_table`用于根据页目录表项/页表项`entry`打印页目录/页表内容. 注意, 未初始化的页目录表项/页表项会被跳过
 * 
 * @param s_vaddr 起始地址
 * @param e_vaddr 结束地址
 * @param level 页表等级
 * @param entry 页目录表项/页表项
 */
void print_page_table(addr_t s_vaddr, addr_t e_vaddr, int level, uint64_t entry);


/**
 * @brief `parse_pt`用于遍历页表`pt`从`s_vaddr`开始的每一个表项, 并进行解析
 * 
 * @param pt 要遍历的页表
 * @param s_vaddr 起始地址
 * @param e_vaddr 结束地址
 */
void parse_pt(pt_t *pt, addr_t s_vaddr, addr_t e_vaddr);

/**
 * @brief `parse_pmd`用于遍历中间页目录表`pmd`从`s_vaddr`开始的每一个表项, 并进行解析
 * 
 * @param pmd 要遍历的中间页目录表
 * @param s_vaddr 起始地址
 * @param e_vaddr 结束地址
 */
void parse_pmd(pmd_t *pmd, addr_t s_vaddr, addr_t e_vaddr);

/**
 * @brief `parse_pgd`用于遍历全局页目录表`pgd`从`s_vaddr`开始的每一个表项, 并进行解析
 * 
 * @param pgd 要遍历的全局页目录表
 * @param s_vaddr 起始地址
 * @param size 解释的内存单元长度
 */
void parse_pgd(pgd_t *pgd, addr_t s_vaddr, size_t size);

/**
 * @brief `dump_pgd`用于在发生页错误的时候转储指定的全局页目录表`pgd`中指定的内存区域
 * 
 * @param pgd 要转储的全局页目录表
 * @param s_vaddr 要转储的内存区域开始处
 * @param e_vaddr 要转储的内存区域结束处
 * 
 * @note 目前该函数会将`PGD`打印到屏幕上
 * 
 * @note 运行该函数时, 需要关闭虚拟地址翻译
 */
void dump_pgd(pgd_t *pgd, addr_t s_vaddr, addr_t e_vaddr);


// **********************************************************************************************************
// * 页表操作函数定义
// **********************************************************************************************************

/**
 * @brief `page_align`用于将地址`addr`进行4K对齐, 即获得`addr`所表示的内存单元所在的页的地址
 * 
 * @param addr 需要对齐的地址
 * @param next 是否返回下一个页
 * @return addr_t `addr`所在的页的地址
 * 
 * @example
 *  1. page_align(0x0000000000000001, False) = 0x0000000000000000, 返回`addr`所在的页的地址
 *  2. page_align(0x0000000000000001, True) = 0x0000000000000001, 返回下一个页的地址
 */
static inline addr_t page_align(addr_t addr, Bool next){
    if (next == False)
        return (addr & ~(PAGE_SIZE - 1));
    return (addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

/**
 * @brief `get_offset`用于获得虚拟地址`vaddr`的页内偏移
 * 
 * @param vaddr 虚拟地址
 * @return offset_t 页内偏移
 */
static inline offset_t get_offset(addr_t vaddr){
    return (offset_t) vaddr & PAGE_OFFSET_MASK;
}

/**
 * @brief `get_vpn`用于获得虚拟地址`vaddr`的第`ppn`级虚拟页号
 * 
 * @param vaddr 虚拟地址
 * @param vpn 虚拟页号级别, -1 表示获取页内偏移
 * @return offset_t 虚拟页号
 */
static inline offset_t get_vpn(addr_t vaddr, int vpn){
    offset_t result = -1;
    if (vpn == 2)
        result = (offset_t) (vaddr >> 30) & (PGDE_PER_PGD - 1);
    else if (vpn == 1)
        result = (offset_t) (vaddr >> 21) & (PMDE_PER_PMD - 1);
    else if (vpn == 0)
        result = (offset_t) (vaddr >> 12) & (PTE_PER_PT - 1);
    else if (vpn == -1)
        result = (offset_t) (vaddr & (PAGE_SIZE - 1));
    ASSERT(result != -1, "vpn = %d, should be in [2, 1, 0, -1]!", vpn);
    return result;
    UNREACHABLE;
}

/**
 * @brief `get_ppn`用于获得物理地址`paddr`的物理页号
 * 
 * @param paddr 物理地址
 * @param ppn 物理页号级别
 * @return addr_t 物理页号
 */
static inline addr_t get_ppn(addr_t paddr, int ppn){
    if (ppn == 2)
        return (paddr & PAGE_PPN2_MASK) >> 30;
    else if (ppn == 1)
        return (paddr & PAGE_PPN1_MASK) >> 20;
    else if (ppn == 0)
        return (paddr & PAGE_PPN0_MASK) >> 12;
    else
        ASSERT(False, "ppn = %d, should be in [2, 1, 0]!");
    UNREACHABLE;
}

/**
 * @brief `get_pgd`用于获得当前正在运行线程的全局页目录表
 * 
 * @return pgd_t* 当前正在运行线程的全局页目录表
 * 
 * @note 目前仅返回内核的PGD, 未来实现用户线程之后需要进行修改
 */
static inline pgd_t *get_pgd(void){
    // TODO: 目前仅返回内核的PGD, 未来实现用户线程之后需要进行修改
    return (pgd_t *)_s_kernel_pgd;
}

/**
 * @brief `get_pgd`用于从指定的`PGD`表项中获得`PMD`
 * 
 * @return pmd_t* `PGD`表项表示的中间页目录表的地址
 */
static inline pmd_t *get_pmd(pgd_entry_t *pgd_ent){
    return (pmd_t *) ((pgd_ent->val >> PAGE_PFN_SHIFT) << PAGE_SHIFT);
}

/**
 * @brief `get_pt`用于从指定的`PMD`表项中获得`PT`
 * 
 * @return pt_t* `PMD`表项表示的页表的地址
 */
static inline pt_t *get_pt(pmd_entry_t *pmd_ent){
    return (pt_t *) ((pmd_ent->val >> PAGE_PFN_SHIFT) << PAGE_SHIFT);
}

/**
 * @brief `get_pgd_entry`用于获取`pgd`指向的全局页目录表中第`pgde_idx`个表项
 * 
 * @param pgd 指向全局页目录的指针
 * @param pgde_idx `PGD`表项索引
 * @return pgd_entry_t* `PGD`表项
 */
DEPRECATED static inline pgd_entry_t* get_pgd_entry(pgd_t *pgd, offset_t pgde_idx){
    // 将PGD指针转为指向PGD Entry的指针后像后移动pgde_idx位
    return ((pgd_entry_t *) pgd) + pgde_idx;
}

/**
 * @brief `set_pgd_entry`用于填充`pgd_ent`指向的`PGD`表项
 * 
 * @param pgd_ent 要填充的`PGD`表项
 * @param pmd_paddr `PGD`表项指向的中间页目录表的物理页地址
 * @param pmd_prot `PGD`表项的属性, 即中间页目录表的属性
 */
static inline void set_pgd_entry(pgd_entry_t *pgd_ent, addr_t pmd_paddr, page_property_t pmd_prot){
    pgd_ent->val = (uint64_t) (((pmd_paddr >> PAGE_SHIFT) << PAGE_PFN_SHIFT) | pmd_prot.val);
}

/**
 * @brief `get_pmd_entry`用于获取全局页目录表项`pgd_ent`中保存的的中间页目录表中第`pmde_idx`个表项
 * 
 * @param pgd_ent 指向`PGD`表项的指针
 * @param pmde_idx `PMD`表项索引
 * @return pmd_entry_t* `PMD`表项
 */
DEPRECATED static inline pmd_entry_t* get_pmd_entry(pgd_entry_t *pgd_ent, offset_t pmde_idx){
    pmd_t *pmd = get_pmd(pgd_ent);
    // 将PMD指针转为指向PMD Entry的指针后像后移动pmde_idx位
    return ((pmd_entry_t *) pmd) + pmde_idx;
}

/**
 * @brief `set_pmd_entry用于填充`pmd_ent`指向的`PMD`表项. 
 * 
 * @param pmd_ent 要填充的`PMD`表项
 * @param pt_paddr `PMD`表项指向的页表的物理页地址
 * @param pt_prot `PMD`表项的属性, 即页表的属性
 */
static inline void set_pmd_entry(pmd_entry_t *pmd_ent, addr_t pt_paddr, page_property_t pt_prot){
    pmd_ent->val = (uint64_t) (((pt_paddr >> PAGE_SHIFT) << PAGE_PFN_SHIFT) | pt_prot.val);
}

/**
 * @brief `get_pt_entry`用于获取中间页目录表项`pmd_ent`中保存的的页表中第`pte_idx`个表项
 * 
 * @param pmd_ent 指向`PMD`表项的指针
 * @param pte_idx `PT`表项索引
 * @return pt_entry_t* `PT`表项
 */
DEPRECATED static inline pt_entry_t* get_pt_entry(pmd_entry_t *pmd_ent, offset_t pte_idx){
    pt_t *pt = get_pt(pmd_ent);
    // 将PMD指针转为指向PMD Entry的指针后像后移动pmde_idx位
    return ((pt_entry_t *) pt) + pte_idx;
}

/**
 * @brief `set_pt_entry`用于填充`pt_ent`指向的`PT`表项. 即填充物理页地址为`p_paddr`, 并将物理页属性设置为`p_prot`
 * 
 * @param pmd_ent 要填充的`PMD`表项
 * @param p_paddr `PT`表项映射的物理页的地址
 * @param p_prot `PT`表项的属性, 即物理页的属性
 */
static inline void set_pt_entry(pt_entry_t *pt_ent, addr_t p_paddr, page_property_t p_prot){
    pt_ent->val = (uint64_t) (((p_paddr >> PAGE_SHIFT) << PAGE_PFN_SHIFT) | p_prot.val);
}


/**
 * @brief `get_mapping_end`用于计算映射区域的大小, 并返回映射区域的最后一个地址.
 * 
 * @note `PGD`中一项可以映射`1GB`内存, `PMD`中一项可以映射`2MB`内存. 但是需要映射的内存区域可能小于`1GB`/`2MB`, 因此
 *      该函数用于计算映射的内存区域的最后一位地址, 用于在填充`PGD Entry`/`PMG Entry`时判断是否需要多个表项
 * 
 * @param s_vaddr 映射区域起始虚拟地址
 * @param e_vaddr 映射区域最后虚拟地址
 * @param pgd_entry 是否为`PGD`表项
 * @return addr_t 映射区域最后一位地址
 */
static inline addr_t get_mapping_end(addr_t s_vaddr, addr_t e_vaddr, Bool pgd_entry){
    addr_t boundary;
    if (pgd_entry == True)
        boundary = (s_vaddr + PGD_SIZE) & PGD_MASK;
    else
        boundary = (s_vaddr + PMD_SIZE) & PMD_MASK;
    return boundary - 1 < e_vaddr - 1 ? boundary : e_vaddr;
}


/**
 * @brief `make_pgd_entry`用于构建一个`PGD`表项
 * 
 * @param pmd_ent 指向`PMD`表项的指针
 * @param s_vaddr 映射起始虚拟地址
 * @param e_vaddr 映射结束虚拟地址
 * @param paddr 映射起始物理地址
 * @param property 映射内存区域属性
 * @param flags 
 * 
 * @note `make_pmd_entry`函数会进行如下的工作:
 *      1. 若`pmd_ent`所指向的`PGD`表项的值为0, 则表示对应的`PT`未初始化, 此时首先分配一个物理页作为`PT`, 
 *          而后进行初始化, 即将`PT`中的内容填充为0
 *      2. 而后调用`make_pt_entry`构建`PT`表项
 */
void make_pmd_entry(
    pmd_entry_t *pmd_ent,
    addr_t s_vaddr,
    addr_t e_vaddr,
    addr_t paddr,
    page_property_t property,
    uint64_t flags
);

/**
 * @brief `make_pgd_entry`用于构建一个`PGD`表项. 注意, 该函数会递归的构建`PMD`表项和`PT`表项
 * 
 * @param pgd_ent 指向`PGD`表项的指针
 * @param s_vaddr 映射起始虚拟地址
 * @param e_vaddr 映射结束虚拟地址
 * @param paddr 映射起始物理地址
 * @param property 映射内存区域属性
 * @param flags 
 * 
 * @note `make_pgd_entry`函数会进行如下的工作:
 *      1. 若`pgd_ent`所指向的`PGD`表项的值为0, 则表示对应的`PMD`未初始化, 此时首先分配一个物理页作为`PMD`, 
 *          而后进行初始化, 即将`PMD`中的内容填充为0
 *      2. 而后调用`make_pmd_entry`构建`PMD`表项
 */
void make_pgd_entry(
    pgd_entry_t *pgd_ent,
    addr_t s_vaddr,
    addr_t e_vaddr,
    addr_t paddr,
    page_property_t property,
    uint64_t flags
);

/**
 * @brief `create_mapping`用于在`pgd`指向的页目录表中创建虚拟地址`vaddr`和物理地址`paddr`的长度为`size`个字节映射
 * 
 * @param pgd 创建映射的地址
 * @param vaddr 虚拟地址
 * @param paddr 物理地址
 * @param size 映射内存区域字节长度
 * @param property 映射内存区域的属性
 * @param flags 
 * 
 * @note 虚拟地址到物理地址的翻译流程为:
 *          1. 根据`vaddr`的PPN[2]域, 即`PGD Index`, 在`PGD`中查询得到`PGD`表项, `PGD`表项中保存着`PMD`地址
 *          2. 根据`vaddr`的PPN[1]域, 即`PMD Index`, 在`PMD`中查询得到`PMD`表项, `PMD`表项中保存着`PT`地址
 *          3. 根据`vaddr`的PPN[0]域, 即`PT Index`, 在`PT`中查询得到`PT`表项, `PT`表项中保存着物理页的地址
 *          4. 根据`vaddr`的Offset域, 即页内偏移, 和物理页地址相加即得到物理地址
 *      因此, 对应的, `create_mapping`中构建虚拟页和物理页的映射的流程为:
 *          1. 根据`vaddr`的PPN[2]域, 即`PGD Index`, 在`PGD`中查询得到`PGD`表项, `PGD`表项中保存着`PMD`地址
 *              - 若`PGD`表项为0, 则对应`PMD`未初始化, 此时需要先分配一个物理页作为`PMD`, 而后将`PMD`的物理地址填充进`PGD`表项
 *          2. 根据`vaddr`的PPN[1]域, 即`PMD Index`, 在`PMD`中查询得到`PMD`表项, `PMD`表项中保存着`PT`地址
 *              - 若`PMD`表项为0, 则对应`PT`未初始化, 此时需要先分配一个物理页作为`PT`, 而后将`PT`的物理地址填充进`PMD`表项
 *          3. 根据`vaddr`的PPN[0]域, 即`PT Index`, 在`PT`中查询得到`PT`表项, 分配一个物理页, 而后将物理页地址填充进`PT`表项
 * 
 * @note `vaddr`和`paddr`不一定是页起始地址, 因此会将其所在的前一个页也进行映射, 
 *      即将`page_align(vaddr, False)`和`page_align(paddr, False)`进行映射
 */
void create_mapping(
    pgd_t *pgd,
    addr_t vaddr, 
    addr_t paddr, 
    uint64_t size, 
    page_property_t property,
    uint64_t flags
);

// **********************************************************************************************************
// * 分页机制函数定义
// **********************************************************************************************************

/**
 * @brief `enable_vm_translation`会开启CPU的虚拟地址转换功能, 定义在`kernel/kasm.S`中
 * 
 * @note 
 *  1. 开启/关闭虚拟地址转换功能本质上就是通过读写`satp`寄存器实现的
 *  2. `enable_vm_translation`目前会开启Sv39虚拟地址转换
 */
extern void enable_vm_translation(void);

/**
 * @brief `disable_vm_translation`会关闭CPU的虚拟地址转换功能, 定义在`kernel/kasm.S`中
 * 
 * @note 
 *  1. 开启/关闭虚拟地址转换功能本质上就是通过读写`satp`寄存器实现的
 */
extern void disable_vm_translation(void);

/**
 * @brief `create_identical_mapping`用于在内核页目录表中创建内核代码段和数据段的恒等映射
 * 
 * @note 
 *  在编译阶段生成的指令中的地址都是相对0x80200000的物理地址, `X2WOS`在加载阶段被加载到物理地址的0x80200000处,
 *  但在开启分页(虚拟地址转换)后, 内核指令中的地址不会发生改变, 因此为了保证开启分页机制(虚拟地址转换)后, 内核代码能够正确访问到内核的代码和数据
 *  需要在内核页目录表中构建内核的代码段和数据段的恒等映射
 */
void create_identical_mapping(void);

/**
 * @brief `create_mmio_mapping`用于在内核页目录表中创建设备寄存器`MMIO`的恒等映射
 * 
 * @note
 *  `RISC-V`将设备寄存器和内存统一编址, 即通过`MMIO`的方式实现访问设备寄存器.
 *  因此, 无论是否开启虚拟地址转换, 设备寄存器映射的内存地址是一样的.
 *  所以需要在内核页目录表中构建设备寄存器映射内存段的恒等映射
 */
void create_mmio_mapping(void);

/**
 * @brief `pageing_init`是内存分页机制的初始化函数
 * 
 * @note `paging_init`完成如下的功能:
 *      1. 初始化内核页目录
 *      2. 在内核页目录标中构建内核代码段和数据段的恒等映射
 *      3. 在内核页目录表中创建设备寄存器`MMIO`的恒等映射
 *      4. 打开虚拟地址翻译机制, 即开启内存分页机制
 */
void paging_init(void);


/**
 * @brief `paging_load_page_fault_exception_handler`是S模式下的Load Page Fault异常处理函数
 * 
 * @param ktf_ptr 陷入帧, S模式下中断触发后在`ktrap_enter`中构建
 * @return int64_t 处理结果, 若为0则表示处理正常, -1表示处理失败
 * 
 * @note 当前的函数仅读取错误地址, 而后打印. 未来实现换页技术时候需要修改该函数
 */
NO_RETURN int64_t paging_load_page_fault_exception_handler(ktrapframe_t *ktf_ptr);


#endif