/* 设置目标架构 */
OUTPUT_ARCH(riscv)

/* 设置入口地址 */
ENTRY(_start)

SECTIONS {
    /* SBI的加载入口地址设置为0x8000_0000 */
    . = 0x80000000,

    .text.boot : {
        *(.text.boot)
    }

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    . = ALIGN(0x8);
    bss_begin = .;
    .bss : {
        *(.bss*)
    }
    bss_end = .;
}
