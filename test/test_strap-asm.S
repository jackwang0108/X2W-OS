.global trigger_load_access_fault
trigger_load_access_fault:
	li a0, 0x70000000
	ld a0, (a0)
	ret

.global trigger_breakpoint
trigger_breakpoint:
	li a0, 0x70000000
	ebreak
	ret

.global trigger_ecall
trigger_ecall:
	li a7, 0x03
	ecall
	ret