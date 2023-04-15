SECTIONS {
    . = 0x80200000,
    kernel_entry = .;
    s_kernel = .;

    s_text_boot = .;
    .text.boot : {
        *(.text.boot)
    }
    e_text_boot = .;

    s_text = .;
    .text : {
        *(.text)
    }
    e_text = .;

    s_rodata = .;
    .rodata : AT(ADDR(.rodata)) {
        *(.rodata)
    }
    e_rodata = .;

    s_data = .;
    .data : {
        *(.data)
    }
    e_data = .;

    . = ALIGN(0x8);
    s_bss = .;
    .bss : {
        *(.bss*)
    }
    e_bss = .;
    e_kernel = .;
}