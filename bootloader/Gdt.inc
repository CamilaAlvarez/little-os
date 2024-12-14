%ifndef __GDT_INC_67343546FDCC56AAB872_INCLUDED__
%define __GDT_INC_67343546FDCC56AAB872_INCLUDED__

bits 16

InstallGDT:
	cli
	pusha
	lgdt [toc]
	sti
	popa
	ret

; Here we define the table
gdt_data:
	dd 0 ; null descriptor
	dd 0 ; dd = 32 bits

; gdt code
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0

; gdt data
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0

end_of_gdt:
toc:
	dw end_of_gdt - gdt_data -1 ; Size
	dd gdt_data ; base address
%endif ; __GDT_INC_67343546FDCC56AAB872_INCLUDED__
