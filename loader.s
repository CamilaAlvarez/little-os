global loader

extern write ; the function is defined somewhere else

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
	mov eax, 0xCAFEBABE
	mov esp, kernel_stack + KERNEL_STACK_SIZE ; end of the memory area = start of the stack
	lea eax, str	
	push dword 0x0B
	push eax
	call write
.loop:
	jmp .loop
section .data
	str: db "Hello world",0
section .bss
align 4
kernel_stack:
	resb KERNEL_STACK_SIZE
