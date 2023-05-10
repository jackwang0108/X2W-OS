set disassemble-next-line on
add-symbol-file build/os.debug
add-symbol-file build/sbi.debug
b kernel_start
b general_strap_handler
target remote : 1234
