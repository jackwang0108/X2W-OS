/**
 * @file kasm.S
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kasm.S`定义了`X2WOS`的汇编函数
 * @version 0.1
 * @date 2023-05-16
 * 
 * @copyright Copyright Zhuiri Xiao, Shihong Wang (c) 2023 with GNU Public License V3.0
*/

#include "constrains.h"


// kernel_pgd是内核页目录表, 定义在kernel.ld中, 在连接阶段由编译器负责填充和重定位
.extern kernel_pgd
#define SATP_MODE_SV39                                      (1UL << 63)

/**
 * @brief `enable_vm_translation`会开启CPU的虚拟地址转换功能, C语言描述为`void enable_vm_translation(void)`
 * 
 * @note 
 *  1. 开启/关闭虚拟地址转换功能本质上就是通过读写`satp`寄存器实现的
 *  2. `enable_vm_translation`目前会开启Sv39虚拟地址转换
 */
.global enable_vm_translation
enable_vm_translation:
    # 内核页目录表的物理地址
    la a2, _s_kernel_pgd
    # 计算内核页目录表的物理页号
    srl a2, a2, PAGE_SHIFT
    # 使用Sv39虚拟地址转换机制
    li a1, SATP_MODE_SV39
    or a2, a2, a1
    # 内存屏障, RISC-V是弱内存模型, 即为了提升指令吞吐量, CPU执行阶段会乱序执行指令
    # 开启虚拟地址转换后, 地址被解释为虚拟地址, 而开启虚拟地址转换前, 地址被解释为物理地址
    # 因此csrw satp, a2指令后内存访问指令不能在csrw satp, a2指令前运行
    # 否则会将指令中的虚拟地址解释为物理地址, 导致未可知的错误发生
    # 所以这里手动触发内存屏障, 即通知CPU在乱序执行指令的时候, sfence.vma之后的内存访问指令不能再sfence.vma指令前执行
    # 从而确保了虚拟地址访问指令不会在开启虚拟地址转换前被运行
    sfence.vma
    csrw satp, a2
    ret
