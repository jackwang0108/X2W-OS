/**
 * @file main.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `main.c`是内核的主函数
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "types.h"
#include "test/test.h"
#include "device/ddr.h"
#include "device/uart.h"
#include "kernel/ktrap.h"
#include "kernel/kmain.h"
#include "kernel/kinit.h"
#include "kernel/kdebug.h"
#include "kernel/kstdio.h"

void kernel_main(void){
    kprintf(DELIMITER);
    kprintf("In kernel!\n");
    kprintf("Kernel init!\n");

	// 输出内核内存映像信息
	print_kmem();

	// 初始化内核
    kinit_all();

    kprintf("Start testing!\n");
	// 测试库文件
    // test_all();

    kprintf("Kernel Hanging Here!\n");

    kprintf("local_interrupt_enable\n");
	// 打开S模式下所有中断
    supervisor_interrupt_enable();

	addr_t unmapped_addr = DDR_END_ADDR + 4096;
	*(uint64_t *) unmapped_addr = 0x55;
	kprintf("Done");
    while (1);
}

void print_kmem(void){
    kprintf(DELIMITER);
    kprintf("X2W-OS Image Layout:\n");
    kprintf("  kernel: 0x%08lx - 0x%08lx (%6ld B)\n",
			(addr_t)_s_kernel, (addr_t)_e_kernel,
			(addr_t)(_e_kernel - _s_kernel));
    kprintf("  .text.boot: 0x%08lx - 0x%08lx (%6ld B)\n",
			(addr_t)_s_text_boot, (addr_t)_e_text_boot,
			(addr_t)(_e_text_boot - _s_text_boot));
	kprintf("       .text: 0x%08lx - 0x%08lx (%6ld B)\n",
			(addr_t)_s_text, (addr_t)_e_text,
			(addr_t)(_e_text - _s_text));
	kprintf("     .rodata: 0x%08lx - 0x%08lx (%6ld B)\n",
			(addr_t)_s_rodata, (addr_t)_e_rodata,
			(addr_t)(_e_rodata - _s_rodata));
	kprintf("       .data: 0x%08lx - 0x%08lx (%6ld B)\n",
			(addr_t)_s_data, (addr_t)_e_data,
			(addr_t)(_e_data - _s_data));
	kprintf("        .bss: 0x%08lx - 0x%08lx (%6ld B)\n",
			(addr_t)_s_bss, (addr_t)_e_bss,
			(addr_t)(_e_bss - _s_bss));
    kprintf(DELIMITER);
}