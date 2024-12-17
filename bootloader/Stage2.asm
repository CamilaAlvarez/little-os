; We're still in RING 0,
; this program will set 32 bit mode 
; and load the kernel

; There's no 512 byte limit here!
org 0x500 ; are unused form 0x500 to 0x7bff

bits 16 ; still 16 bits real mode

jmp main

%include "stdio.inc"
%include "Gdt.inc"
%include "A20.inc"

LoadingMsg db "Preparing to load operating system ...", 0x0D, 0x0A, 0x00
main:
	;set segments and stack
	cli ; causes processor to ignore maskable interrupts
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ax, 0x9000 ; stack begins at 0x9000-0xffff
	mov ss, ax
	mov sp, 0xFFFF ; points to the end of the stack 
	sti

	; Install GDT
	call InstallGDT

	; Enable A20 (before going to protected mode and enabling 32 bits)
	call EnableA20

	; Print loading message
	mov si, LoadingMsg
	call Puts16

	; go to protected mode
	cli ; do not re-enable interrupts! It will cause triple fault
	mov eax, cr0
	or eax, 1 ; set bit 0 in cr0
	mov cr0, eax
	
	jmp CODE_DESC:Stage3 ; far jump to set cs, and to start with 32 bits

bits 32
Stage3:
	; set registers
	mov ax, DATA_DESC
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov esp, 90000h ; stack begins from 90000h

	call ClrScr32
	mov ebx, msg
	call Puts32
STOP:
	cli
	hlt

msg db  0x0A, 0x0A, 0x0A, "               <[ OS Development Series Tutorial 10 ]>"
    db  0x0A, 0x0A,             "           Basic 32 bit graphics demo in Assembly Language", 0
