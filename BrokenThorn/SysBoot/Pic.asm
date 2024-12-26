%define ICW_1 0x11

%define PIC_1_CTRL 0x20
%define PIC_2_CTRL 0xA0

%define PIC_1_DATA 0x21
%define PIC_2_DATA 0xA1

%define IRQ_0 0x20
%define IRQ_8 0x28

MapPIC:
; ICW1
	mov al, ICW_1
	out PIC_1_CTRL, al
; ICW2 - base IRQ
	out PIC_2_CTRL, al ; send ICW1 to slave

	mov al, IRQ_0
	out PIC_1_DATA, al

	mov al, IRQ_8
	out PIC_2_DATA, al
; ICW3 - connect two pics
	mov al, 0x4
	out PIC_1_DATA, al

	mov al, 0x2
	out PIC_2_DATA, al
; ICW4 - set x86 mode
	mov al, 1

	out PIC_1_DATA, al
	out PIC_2_DATA, al
; Null out the data registers
	mov al, 0
	out PIC_1_DATA, al
	out PIC_2_DATA, al
