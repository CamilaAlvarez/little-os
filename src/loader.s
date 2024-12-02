global loader

extern main ; the function is defined somewhere else

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER
KERNEL_STACK_SIZE equ 4096

section .text
align 4
	dd MAGIC_NUMBER
	dd FLAGS
	dd CHECKSUM

loader:
	mov esp, kernel_stack + KERNEL_STACK_SIZE ; end of the memory area = start of the stack
	call main
.loop:
	jmp .loop
section .bss
align 4
kernel_stack:
	resb KERNEL_STACK_SIZE
