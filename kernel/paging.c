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


void paging_init(void){
    // 初始化内核页目录表
    memset(kernel_pgd, 0, PAGE_SIZE);
    kprintf("kernel PGD is at: %#X\n", (addr_t) kernel_pgd);
    // 在内核页目录标中构建内核代码段和数据段的恒等映射
    create_identical_mapping();
    // 在内核页目录表中创建设备寄存器`MMIO`的恒等映射
    create_mmio_mapping();
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
    kprintf("Start %s\n", __func__);

    addr_t start_addr, end_addr; size_t size;

    // 为内存中的内核代码段创建恒等映射, 内核代码段的属性是可读可执行
    start_addr = (addr_t) _s_text_boot, end_addr = (addr_t) _e_text, size = (size_t) (end_addr - start_addr);
    kprintf("\tmapping kernel text, %#X~%#X, %7d Bytes\n", start_addr, end_addr, size);
    page_property_t ktext_prot = {(uint64_t) KERNEL_PAGE_READ_EXEC};
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, ktext_prot, 0);

    // 为内存中的内核数据段创建恒等映射, 这里额外分配32个页, 用于未来的动态内存分配用
    // 内核数据段的属性是可读可写
    start_addr = (addr_t) _s_rodata, end_addr = (addr_t) _e_bss, size = (size_t) (end_addr - start_addr);
    kprintf("\tmapping kernel data, %#X~%#X, %7d Bytes\n", start_addr, end_addr, size);
    page_property_t kdata_prot = {(uint64_t) KERNEL_PAGE};
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, kdata_prot, 0);
}


void create_mmio_mapping(void){
    kprintf("Start %s\n", __func__);

    addr_t start_addr, end_addr; size_t size;
    page_property_t mmioreg_prot = {(uint64_t) KERNEL_PAGE};

    // PLIC寄存器MMIO内存的恒等映射
    start_addr = (addr_t) PLIC_BASE_ADDR, end_addr = (addr_t) PLIC_END_ADDR, size = (size_t) (end_addr - start_addr);
    kprintf("\tmapping PLIC MMIO registers, %#16X  :  %#16X, %7d Bytes\n", start_addr, end_addr, size);
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, mmioreg_prot, 0);

    // CLINT寄存器MMIO内存的恒等映射
    start_addr = (addr_t) CLINT_BASE_ADDR, end_addr = (addr_t) CLINT_END_ADDR, size = (size_t) CLINT_MMIO_SIZE;
    kprintf("\tmapping CLINT MMIO registers, %#16X  :  %#16X, %7d Bytes\n", start_addr, end_addr, size);
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, mmioreg_prot, 0);

    // UART寄存器MMIO内存的恒等映射
    start_addr = (addr_t) UART_BASE_ADDR, end_addr = (addr_t) UART_END_ADDR, size = (size_t) UART_MMIO_SIZE;
    kprintf("\tmapping UART MMIO registers, %#16X  :  %#16X, %7d Bytes\n", start_addr, end_addr, size);
    create_mapping((pgd_t *)kernel_pgd, start_addr, start_addr, size, mmioreg_prot, 0);
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
    pt_entry_t *pt_ent = get_pt_entry(pmd_ent, get_vpn(s_vaddr, 0));
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
    pmd_entry_t *pmd_ent = get_pmd_entry(pgd_ent, get_vpn(s_vaddr, 1));
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
    } while (pgd_ent++, curr_vpage = next_vpage, curr_vpage < end_vpage);
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
    pgd_entry_t *pgd_ent = get_pgd_entry(pgd, get_vpn(curr_vpage, 2));
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