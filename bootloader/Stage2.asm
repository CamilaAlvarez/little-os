; We're still in RING 0,
; this program will set 32 bit mode 
; and load the kernel

; There's no 512 byte limit here!
org 0x0

bits 16 ; still 16 bits real mode

jmp main

Print:
	lodsb ; load next of si to al
	or al, al ; if we haven't reached the end
	jz PrintDone
	mov ah, 0eh ; print instruction
	int 10h ; call interrupt (still in real mode, we have access to BIOS interrupts)
	jmp Print
PrintDone:
	ret

main:
	cli ; causes processor to ignore maskable interrupts
	push cs
	pop ds ; now ds = cs

	mov si, Msg ; lodsb works with si, Msg is a variable
	call Print

	cli
	hlt

Msg db "Preparing to load operating system ...",13,10,0 
	
