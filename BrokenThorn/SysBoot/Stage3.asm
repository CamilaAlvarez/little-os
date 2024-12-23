org 0x100000 ; Kernel starts at 1 MB

bits 32

jmp Stage3

%include "stdio.inc"
msg db  0x0A, 0x0A, "                       - OS Development Series -"
    db  0x0A, 0x0A, "                     MOS 32 Bit Kernel Executing", 0x0A, 0

Stage3:
	mov	ax, 0x10		; set data segments to data selector (0x10)
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h		; stack begins from 90000h

	call ClrScr32
	mov ebx, msg
	call Puts32

	cli
	hlt
