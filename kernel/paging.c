/**
 * @file paging.c
 * @author Zhuir Xiao (xzr3356142450@gmail.com)
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `paging.h`是`X2WOS`的内存分页机制模块
 * @version 0.1
 * @date 2023-05-16
 * 
 * @copyright Copyright Zhuiri Xiao, Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "types.h"
#include "string.h"
#include "constrains.h"
#include "asm/plic.h"
#include "asm/uart.h"
#include "asm/clint.h"
#include "kernel/paging.h"

pgd_t *kernel_pgd = (pgd_t *)_s_kernel_pgd;

void paging_init(void){
    // 初始化内核页目录表
    memset(kernel_pgd, 0, PAGE_SIZE);
    kprintf("kernel PGD is at: %#X\n", (addr_t) kernel_pgd);
    // 在内核页目录标中构建内核代码段和数据段的恒等映射
    create_identical_mapping();
    // 在内核页目录表中创建设备寄存器`MMIO`的恒等映射
    create_mmio_mapping();
    // 注册Load Page Fault函数
    register_ktrap_handler(CAUSE_EXCEPTION_LOAD_PAGE_FAULT, False, "Load Page Fault Exception", paging_load_page_fault_exception_handler);
    // 注册Store/AMO Page Fault函数
    register_ktrap_handler(CAUSE_EXCEPTION_STORE_PAGE_FAULT, False, "Store Page Fault Exception", paging_load_page_fault_exception_handler);
    // 开启虚拟地址翻译机制, 即开启内存分页机制
    enable_vm_translation();
}


/**
 *  定义在`kernel.ld`中的内核数据信息:
 *      1. `_s_text_boot`是内核的`.text.boot`段的起始地址, 即内核代码段的开始地址
 *      2. `_e_text`是内核`.text`段的结束地址, 即内核代码段的结束地址
 *      3. `_s_rodata`是内核`.rodata`段的开始地址, 即内核数据段的起始地址
 *      4. `_e_bss`是内核`.bss`段的结束地址, 即内核数据段的结束地址
 */
extern char _s_text_boot[], _e_text[], _s_rodata[], _e_bss[];

void create_identical_mapping(void){
    kprintf("start %s\n", __func__);

    addr_t start_addr, end_addr; size_t size, pages;

    // 为内存中的内核代码段创建恒等映射, 内核代码段的属性是可读可执行
    start_addr = (addr_t) _s_text_boot, end_addr = (addr_t) _e_text, size = (size_t) (end_addr - start_addr);
    pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    kprintf("\tmapping kernel text, %#X~%#X, %7d Bytes, %2d Pages used (%2d * 4096 = %7d)\n", start_addr, end_addr, size, pages, pages, pages * PAGE_SIZE);
    page_property_t ktext_prot = {(uint64_t) KERNEL_PAGE_READ_EXEC};
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, ktext_prot, 0);

    // 为内存中的内核数据段创建恒等映射, 这里额外分配32个页, 用于未来的动态内存分配用
    // 内核数据段的属性是可读可写
    start_addr = (addr_t) _s_rodata, end_addr = (addr_t) _e_bss, size = (size_t) (end_addr - start_addr);
    pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    kprintf("\tmapping kernel data, %#X~%#X, %7d Bytes, %2d Pages used (%2d * 4096 = %7d)\n", start_addr, end_addr, size, pages, pages, pages * PAGE_SIZE);
    page_property_t kdata_prot = {(uint64_t) KERNEL_PAGE};
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, kdata_prot, 0);
}


void create_mmio_mapping(void){
    kprintf("start %s\n", __func__);

    addr_t start_addr, end_addr; size_t size, pages;
    page_property_t mmioreg_prot = {(uint64_t) KERNEL_PAGE};

    // PLIC寄存器MMIO内存的恒等映射
    start_addr = (addr_t) PLIC_BASE_ADDR, end_addr = (addr_t) PLIC_END_ADDR, size = (size_t) (end_addr - start_addr);
    pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    kprintf("\tmapping PLIC  MMIO registers,  %#8X~%#8X, %7d Bytes, %2d Pages used (%2d * 4096 = %7d)\n", start_addr, end_addr, size, pages, pages, pages * PAGE_SIZE);
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, mmioreg_prot, 0);

    // CLINT寄存器MMIO内存的恒等映射
    start_addr = (addr_t) CLINT_BASE_ADDR, end_addr = (addr_t) CLINT_END_ADDR, size = (size_t) CLINT_MMIO_SIZE;
    pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    kprintf("\tmapping CLINT MMIO registers,  %#8X~ %#8X, %7d Bytes, %2d Pages used (%2d * 4096 = %7d)\n", start_addr, end_addr, size, pages, pages, pages * PAGE_SIZE);
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, mmioreg_prot, 0);

    // UART寄存器MMIO内存的恒等映射
    start_addr = (addr_t) UART_BASE_ADDR, end_addr = (addr_t) UART_END_ADDR, size = (size_t) UART_MMIO_SIZE;
    pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    kprintf("\tmapping UART  MMIO registers, %#X~%#X, %7d Bytes, %2d Pages used (%2d * 4096 = %7d)\n", start_addr, end_addr, size, pages, pages, pages * PAGE_SIZE);
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, mmioreg_prot, 0);
}


// TODO: 未来实现换页机制需要修改该函数
NO_RETURN int64_t paging_load_page_fault_exception_handler(ktrapframe_t *ktf_ptr){
    // 关中断, 避免循环
    supervisor_interrupt_disable();
    // 读取S模式下Load Page Fault异常发生的地址 
    addr_t bad_addr = read_csr(stval);
    offset_t \
        vpn2 = get_vpn(bad_addr, 2),
        vpn1 = get_vpn(bad_addr, 1),
        vpn0 = get_vpn(bad_addr, 0),
        offs = get_vpn(bad_addr, -1);
    kprintf("==============================================================================\n");
    kprintf("Supervisor Load Page Fault Exception Happend, stvec = %#X\n", bad_addr);
    kprintf("This means you're trying to access an invalid virtual address %#X\n", bad_addr);
    kprintf("Load Page Fault exception happened may due to PGDE/PMDE/PTE is invalid\n");
    kprintf("This may because you didn't add memory mapping of that virtual address\n");
    kprintf("Please check correspoding PGDE/PMDE/PTE and use create_mapping() to fix it\n");
    kprintf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    kprintf("Here are some helpful infomation\n");
    kprintf("Detailed infomation of vaddr:\n");
    kprintf(
        "\tVPN[2] = %#3X\n"
        "\tVPN[1] = %#3X\n"
        "\tVPN[0] = %#3X\n"
        "\tOFFSET = %#3X\n",
        vpn2, vpn1, vpn0, offs
    );
    kprintf("Detailed infomation of PGD:\n");
    kprintf("\tkernel PGD is at %#X\n", (addr_t) kernel_pgd);
    kprintf("\tcurrent running thread PGD is at %#X\n", (addr_t) get_pgd());
    kprintf("Kernel PGD/PMT/PT Info:\n");
    // dump_pgd(get_pgd(), (addr_t) _s_text_boot, (addr_t) _e_bss);
    while (1);
    UNREACHABLE;
}


// **********************************************************************************************************
// * 页表转储函数
// **********************************************************************************************************

/**
 * @brief `pg_level_t`是每一级页表的结构体
 */
typedef struct __pg_level_t {
    size_t num;
    uint64_t mask;
    const char *name;
    const property_bit_t *prot_bits;
} pg_level_t;

static pg_level_t pg_levels[] = {
    {.name = "PGDE",    .prot_bits = prot_bits,    .num = ARRAY_SIZE(prot_bits)},
    {.name = "PMDE",    .prot_bits = prot_bits,    .num = ARRAY_SIZE(prot_bits)},
    {.name = "PTE",     .prot_bits = prot_bits,    .num = ARRAY_SIZE(prot_bits)}
};

void dump_property(uint64_t property, const property_bit_t *bits, size_t num){
    // 循环检测属性
    for (int i = 0; i < num; i++, bits++){
        const char *s = ((property & bits->mask) == bits->val) ? bits->set : bits->clear;
        if (s)
            kprintf("%s", s);
    }
}

void print_page_table(addr_t s_vaddr, addr_t e_vaddr, int level, uint64_t entry){
    const char size_units[] = "KMGT";
    // 获取当前页的字节单元
    const char *unit = size_units;
    // 根据当前页的等级, 来获取该页的属性
    uint64_t property = entry & pg_levels[level].mask;

    for (int i = 0; i < ARRAY_SIZE(address_markers); i++)
        if (s_vaddr == address_markers[i].start_address)
            kprintf("---[ %s ]---\n", address_markers[i].name);
    
    // 未初始化的页目录表项/页表项, 直接过
    if (entry == 0)
        return;

    /*
     * 打印表项的范围, 例如:
     *      0x0000_0000_8020_0000-0x0000_0000_8020_1000
     */
    kprintf("0x%016lX-0x%016lX    ", s_vaddr, e_vaddr);
    
    // 计算使用的最大单位, 默认是从KB开始, 所以右移一位
    offset_t delta = (e_vaddr - s_vaddr) >> 10;
    while (!(delta & 1023) && size_units[1]){
        delta >>= 10;
        unit++;
    }

    /*
     * 打印表项的大小, 例如
     * 0123456789
     *          4K PTE
     *          2M PMDE
     *          1G PGDE
     */
    kprintf("%9lu%c %s", delta, *unit, pg_levels[level].name);

    /*
     * 打印表项的属性, 例如:
     *      D A G . X . R V
     */
    if (pg_levels[level].prot_bits)
        dump_property(property, pg_levels[level].prot_bits, pg_levels[level].num);
    kprintf("\n");
}

void parse_pt(pt_t *pt, addr_t s_vaddr, addr_t e_vaddr){
    addr_t curr_vpage = page_align(s_vaddr, False);
    addr_t end_vpage = page_align(e_vaddr, False);

    pt_entry_t *pt_ent = ((pt_entry_t *) pt) + get_vpn(curr_vpage, 0);
    do {
        // 打印页表项
        print_page_table(curr_vpage, curr_vpage + PAGE_SIZE, 2, pt_ent->val);
    } while (pt_ent++, curr_vpage += PAGE_SIZE, curr_vpage < end_vpage);
}


void parse_pmd(pmd_t *pmd, addr_t s_vaddr, addr_t e_vaddr){
    addr_t curr_vpage = page_align(s_vaddr, False);
    addr_t end_vpage = page_align(e_vaddr, False);
    addr_t next_vpage;

    pmd_entry_t *pmd_ent = ((pmd_entry_t *) pmd) + get_vpn(curr_vpage, 1);
    do {
        next_vpage = get_mapping_end(curr_vpage, e_vaddr, False);
        if (!is_valid_page(pmd_ent->val) || is_leaf_page(pmd_ent->val))
            // 无效中间页目录表项 或 中间页目录表项表示一个2MB的页, 直接打印
            print_page_table(curr_vpage, next_vpage, 1, pmd_ent->val);
        else if (is_valid_page(pmd_ent->val))
            // 中间页目录表项有效, 则对应的PT已经分配, 则解释PT
            parse_pt(get_pt(pmd_ent), curr_vpage, end_vpage);
    } while (pmd_ent++, curr_vpage = next_vpage, curr_vpage < end_vpage);
}

void parse_pgd(pgd_t *pgd, addr_t s_vaddr, size_t size){
    addr_t curr_vpage = page_align(s_vaddr, False);
    addr_t end_vpage = page_align(s_vaddr + size, True);
    addr_t next_vpage;

    // 遍历页目录表
    pgd_entry_t *pgd_ent = ((pgd_entry_t *) pgd) + get_vpn(curr_vpage, 2);
    do {
        next_vpage = get_mapping_end(curr_vpage, end_vpage, True);
        if (!is_valid_page(pgd_ent->val) || is_leaf_page(pgd_ent->val))
            // 无效全局页目录表项 或 全局页目录表项表示一个2MB的页, 直接打印
            print_page_table(curr_vpage, next_vpage, 2, pgd_ent->val);
        else if (is_valid_page(pgd_ent->val))
            // 全局页目录表项有效, 则对应的PMD已经分配, 则解释PMD
            parse_pmd(get_pmd(pgd_ent), curr_vpage, next_vpage);
        else
            // 全局页目录表项无效, 则对应的PMD未分配, 不解释PMD
            ASSERT(False, "Invalid PGD Entry!");
    } while (pgd_ent++, curr_vpage = next_vpage, curr_vpage < end_vpage);
}

void dump_pgd(pgd_t *pgd, addr_t s_vaddr, addr_t e_vaddr){
    addr_t start_vpage = page_align(s_vaddr, False);
    addr_t end_vpage = page_align(e_vaddr, True);

    // 初始化使用的结构体
    for (int i = 0; i < ARRAY_SIZE(pg_levels); i++)
        if (pg_levels[i].prot_bits)
            for (int j = 0; j < pg_levels[i].num; j++)
                pg_levels[i].mask |= pg_levels[i].prot_bits[j].mask;
    
    // 解释全局页目录表
    parse_pgd(pgd, start_vpage, end_vpage);
}

// **********************************************************************************************************
// * 页表操作函数定义
// **********************************************************************************************************

// 物理页分配函数, 定义在`mm.c`中
extern addr_t alloc_ppage(Bool kpage);

void make_pmd_entry(pmd_entry_t *pmd_ent, addr_t s_vaddr, addr_t e_vaddr, addr_t paddr, page_property_t property, uint64_t flags){
    // 计算开始映射的物理页和虚拟页地址, 注意, PT中每个表项都映射一个页, 即4KB大小的内存区域
    // 注意, make_pmd_entry只会被make_pgd_entry调用. 因此传入的s_vaddr, e_vaddr, paddr都是4096字节对齐的
    addr_t curr_ppage = paddr;
    addr_t curr_vpage = s_vaddr;
    addr_t end_vpage = e_vaddr;

    // 若pmd_ent的值为0, 则表示PT未初始化, 则此时需要先分配一个页作为PT, 而后填充pmd_ent
    if (pmd_ent->val == 0){
        addr_t pt_ppage = alloc_ppage(True);
        page_property_t pt_prot = {(uint64_t) PAGE_TABLE};
        // 填充pmd_ent
        set_pmd_entry(pmd_ent, pt_ppage, pt_prot);

        // Debug
        // kprintf("Alloc A PT at %#X, coressponding PMD_Ent.val = %#X\n", pt_ppage);
    }

    // 逐页映射
    pt_t *pt = get_pt(pmd_ent);
    pt_entry_t *pt_ent = ((pt_entry_t *) pt) + get_vpn(curr_ppage, 0);
    do {
        set_pt_entry(pt_ent, curr_ppage, property);
        // Debug
        // kprintf("Map vpage at %#X to paddr at %#X\n", curr_vpage, curr_ppage);
        curr_ppage += PAGE_SIZE;
    } while (pt_ent++, curr_vpage += PAGE_SIZE, curr_vpage < end_vpage);
}


void make_pgd_entry(pgd_entry_t *pgd_ent, addr_t s_vaddr, addr_t e_vaddr, addr_t paddr, page_property_t property, uint64_t flags){
    // 计算开始映射的物理页和虚拟页地址, 注意, PMD中每个表项都可以映射2MB大小的内存区域, 所以这里的page其实表示一个2MB的巨页
    // 注意, make_pgd_entry只会被create_mapping调用. 因此传入的s_vaddr, e_vaddr, paddr都是4096字节对齐的
    addr_t curr_ppage = paddr;
    addr_t curr_vpage = s_vaddr;
    addr_t end_vpage = e_vaddr;

    // 若pgd_ent的值为0, 则表示PMD未初始化, 则此时需要先分配一个页作为PMD, 而后填充pgd_ent
    if (pgd_ent->val == 0){
        addr_t pmd_ppage = alloc_ppage(True);
        page_property_t pmd_prot = {(uint64_t) PAGE_TABLE};
        // 填充pgd_ent
        set_pgd_entry(pgd_ent, pmd_ppage, pmd_prot);

        // Debug
        // kprintf("Alloc A PMD at %#X, corresponding PGD_Ent.val = %#X\n", pmd_ppage, pgd_ent->val);
    }

    // 获得起始PMD表项
    pmd_t *pmd = get_pmd(pgd_ent);
    pmd_entry_t *pmd_ent = ((pmd_entry_t *)pmd) + get_vpn(curr_ppage, 1);
    // 若要映射的范围大于2MB, 则需要使用多个PMD表项完成映射, 所以此时使用循环
    addr_t next_vpage;
    do {
        /* 计算当前PMD表项的映射的最后一位地址
         *  1. 若剩余需要映射的范围大于2MB, 则还需要一个PMD表项, 所以此时计算下一个PMD表项开始映射的虚拟页地址
         *  2. 若剩余需要映射的范围小于2MB, 则只使用当前的PMD表项即可, 此时next_vhpage的值不会被使用
         */
        next_vpage = get_mapping_end(curr_vpage, end_vpage, False);
        // 构建PMD表项
        make_pmd_entry(pmd_ent, curr_vpage, next_vpage, curr_ppage, property, flags);
        // 更新下一个开始映射的虚拟页地址
        curr_ppage += (next_vpage - curr_vpage);
    } while (pmd_ent++, curr_vpage = next_vpage, curr_vpage < end_vpage);
}


void create_mapping(pgd_t *pgd, addr_t vaddr, addr_t paddr, uint64_t size, page_property_t property, uint64_t flags){
    // 安全性检查
    ASSERT(0 < size, "mapping memory size must greater than 0, but size=%d!", size);

    // 计算开始映射的物理页和虚拟页地址, 注意, PGD中每个表项都可以映射1GB大小的内存区域, 所以这里的page其实表示一个2GB的巨页
    addr_t curr_ppage = page_align(paddr, False);
    addr_t curr_vpage = page_align(vaddr, False);
    // 要映射的最后一个虚拟页地址
    addr_t end_vpage = page_align(vaddr + size, True);

    // 获取起始PGD表项
    pgd_entry_t *pgd_ent = ((pgd_entry_t *) pgd) + get_vpn(curr_vpage, 2);
    // 若要映射的范围大于1GB, 则需要使用多个PGD表项完成映射, 所以此时使用循环
    addr_t next_vpage;
    do {
        /* 计算当前PGD表项的映射的最后一位地址
         *  1. 若剩余需要映射的范围大于1GB, 则还需要一个PGD表项, 所以此时计算下一个PGD表项开始映射的虚拟页地址
         *  2. 若剩余需要映射的范围小于1GB, 则只使用当前的PGD表项即可, 此时next_vhpage的值不会被使用
         */
        next_vpage = get_mapping_end(curr_vpage, end_vpage, True);
        // 构建PGD表项
        make_pgd_entry(pgd_ent, curr_vpage, next_vpage, curr_ppage, property, flags);
        // 更新下一个开始映射的虚拟页地址
        curr_ppage += (next_vpage - curr_vpage);
    } while (pgd_ent++, curr_vpage = next_vpage, curr_vpage < end_vpage);
}