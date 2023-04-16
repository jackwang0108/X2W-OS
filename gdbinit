set disassemble-next-line on
add-symbol-file build/os.debug
add-symbol-file build/sbi.debug
b kernel_start
target remote : 1234
c