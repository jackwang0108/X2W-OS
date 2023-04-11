set disassemble-next-line on
symbol-file build/os.debug
symbol-file build/sbi.debug
b kernel_start
target remote : 1234
c