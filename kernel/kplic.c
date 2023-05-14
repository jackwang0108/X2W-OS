/**
 * @file kplic.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kplic.c`是的`PLIC`全局中断控制器初始化内核部分的代码
 * @version 0.1
 * @date 2023-05-13
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "io.h"
#include "kernel/kplic.h"
#include "kernel/ktrap.h"
#include "kernel/kstdio.h"

// 外部中断号
typedef enum __plic_interrupt_id_t {
    UART0_INTERRUPT = 10
} plic_interrupt_id_t;

// SiFive FU740第0个核监控用, 不处理外部中断, 所以这里+1自动跳过
#define CPU_TO_HART(cid) ((cid) + 1)

void kplic_set_priority(uint32_t hwiid, uint32_t priority){
    // 获得中断编号为hwiid的中断优先级寄存器的地址
    addr_t reg_addr = plic_priority_addr(hwiid);
	kprintf("%s: reg = %#X, prio = %d\n", __func__, reg_addr, priority);
    // 后三位表示优先级, PLIC上每个寄存器都是32位
    write_32_bits(reg_addr, priority);
}

void kplic_enable_interrupt(uint64_t cid, uint32_t hwiid, Bool enable, Bool m_mode){
    // PLIC每个寄存器32位, 先计算中断编号为hwiid在中断使能寄存器的的mask, 后面位操作用
    uint32_t hwiid_mask = 1 << (hwiid % 32);
    int hart_id = CPU_TO_HART(cid);
    // 计算hwiid的中断使能寄存器的MMIO地址
    addr_t reg_addr = plic_enable_addr(hart_id, m_mode) + 4 * (hwiid / 32);
    // 输出到屏幕上
    kprintf(
        "%s Hardware Interrrupt ID: %3d at %#16X\n", 
        enable == True ? "Enable" : "Disable", hwiid, reg_addr
    );
    if (enable)
        write_32_bits(reg_addr, read_32_bits(reg_addr) | hwiid_mask);
    else
        write_32_bits(reg_addr, read_32_bits(reg_addr) & ~hwiid_mask);
}

void kplic_init(void){
    // 设置所有中断的优先级为1
    for (int hwiid = 1; hwiid <= PLIC_MAX_INTERRUPTS_NUM; hwiid++)
        kplic_set_priority(hwiid, 1);
    // 逐核心设置
    for (int cpu_id = 0; cpu_id < MAX_CPU_NUM; cpu_id++){
        // 设置该核心的中断优先级阈值寄存器为0, 即不屏蔽任何外部设备
        write_32_bits(plic_threshold_addr(CPU_TO_HART(cpu_id), True), 0);
        // 设置改核心不使能任何外部设备中断, 即不响应任何外部设备的中断, 后面手动开启
        for (int hwiid = 1; hwiid <= PLIC_MAX_INTERRUPTS_NUM; hwiid++)
            kplic_enable_interrupt(cpu_id, hwiid, False, True);
    }

    // 打开外部中断总开关
    set_csr(sie, SIE_S_EXTERNAL_INTERRUPT);
    // 打开CPU0的UART0中断
    kplic_enable_interrupt(0, UART0_INTERRUPT, True, True);
}


// 外部中断信息表, 取决于硬件制造商, 这里用的是QEMU Virt模拟的开发板
const char *plic_intr_msg[PLIC_MAX_INTERRUPTS_NUM] = {
    [0] = "Error",
    [1 ... 8] = "VIRTIO_IRQ Interrupt",
    [9] = "Error",
    [10] = "UART0 Interrupt",
    [11] = "RTC Interrupt",
    [32 ... 35] = "PCIe Interrupt"
};

// 外部中断处理函数结构体
extern void uart_interrupt_handler(void);

int64_t kplic_interrupt_handler(ktrapframe_t *ktf_ptr){
    // 开始处理中断前, 关闭中断总开关, 避免中断嵌套
    clear_csr(sie, SIE_S_EXTERNAL_INTERRUPT);

    // TODO: 目前所有中断都让CPU0处理的, 未来多核设计需要修改
    int hart_id = CPU_TO_HART(0);
    // 计算中断请求寄存器MMIO地址,
    addr_t claim_reg_addr = plic_claim_addr(hart_id, True);
    uint32_t hwiid = -1;

    // 可能同时有多个中断, 因此需要循环处理
    while (
        // 读取中断请求寄存器, 0留空
        (hwiid = read_32_bits(claim_reg_addr)) != 0
    ){
        // 目前仅支持UART输入中断
        if (hwiid == UART0_INTERRUPT)
            uart_interrupt_handler();

        // 处理完当前中断, 写中断完成寄存器
        write_32_bits(claim_reg_addr, hwiid);
    }

    // 中断处理完成后, 打开当前核心的中断总开关
    set_csr(sie, SIE_S_EXTERNAL_INTERRUPT);
    return 0;
}