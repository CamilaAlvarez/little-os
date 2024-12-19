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
%include "Fat12.inc"
; where the kernel is to be loaded to in protected mode
%define IMAGE_PMODE_BASE 0x100000

; where the kernel is to be loaded to in real mode
%define IMAGE_RMODE_BASE 0x3000
; kernel name (Must be 11 bytes)
ImageName     db "KRNL    SYS"

; size of kernel image in bytes
ImageSize     db 0


LoadingMsg db "Preparing to load operating system ...", 0x0D, 0x0A, 0x00
msgFailure db 0x0D, 0x0A, "*** FATAL: MISSING OR CURRUPT KRNL.SYS. Press Any Key to Reboot", 0x0D, 0x0A, 0x0A, 0x00
main:
	;set segments and stack
	cli ; causes processor to ignore maskable interrupts
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ax, 0x0 ; stack begins at 0x9000-0xffff
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
	
	; Init Filesystem
	call LoadRoot

	; Load Kernel - before protected mode
	mov ebx, 0
	mov bp, IMAGE_RMODE_BASE
	mov si, ImageName
	call LoadFile
	mov dword [ImageSize], ecx ; size of the kernel
	cmp ax, 0
	je EnterStage3
	mov si, msgFailure
	call Puts16
	mov ah, 0
	int 0x16 
	int 0x19
	cli
	hlt

EnterStage3:
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

CopyImage:
	mov eax, dword [ImageSize]
	movzx ebx, WORD [bpbBytesPerSector]
	mul ebx
	mov ebx, 4
	div ebx
	cld
	mov esi, IMAGE_RMODE_BASE
	mov edi, IMAGE_PMODE_BASE
	mov ecx, eax
	rep movsd

	jmp CODE_DESC:IMAGE_PMODE_BASE 

STOP:
	cli
	hlt
