%ifndef __FAT12_INC_67343546FDCC56AAB872_INCLUDED__
%define __FAT12_INC_67343546FDCC56AAB872_INCLUDED__

bits 16 ; we're still in real mode

%include "Floppy16.inc"

%define ROOT_OFFSET 0x2e00
%define FAT_SEG 0x2c0
%define ROOT_SEG 0x2e0

; LoadRoot - Load root directory table to 0x7e00
LoadRoot:
	pusha
	push es

	; size of root directory
	xor cx, cx
	xor dx, dx
	mov ax, 32 ; 32 byte dir entry
	mul WORD [bpbRootEntries]
	div WORD [bpbBytesPerSector]
	xchg ax, cx

	; compute location of root dir and store in ax
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	add ax, WORD [bpbReservedSectors]
	mov WORD [datasector], ax
	add WORD [datasector], cx

	; read root dir into 0x7e00

	push word ROOT_SEG
	pop es
	mov bx, 0
	call ReadSectors
	pop es
	popa
	ret

; LoadFAT - Loads FAT table to 0x7c00
; Parm/ ES:DI Root Directory Table

LoadFAT:
	pusha
	push es

	;compute size of FAT and store in cx
	xor ax, ax
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	mov cx, ax

	; compute location of FAT and store in ax
	mov ax, WORD [bpbReservedSectors]

	; read FAT into memory (overwrites bootloader)
	push word FAT_SEG
	pop es
	xor bx, bx
	call ReadSectors
	pop es
	popa
	ret

; FindFile - Search for filename in root table
; parm/ DS:SI File name
; ret/ AX File index number in dir table, -1 if error

FindFile:
	push cx
	push dx
	push bx
	mov bx, si ; copy filename

	mov cx, WORD [bpbRootEntries]
	mov di, ROOT_OFFSET
	cld ; clear direction flag

	.LOOP:
	push cx
	mov cx, 11
	mov si, bx
	push di
	rep cmpsb ; compare string byte by byte
	pop di
	je .Found
	pop cx
	add di, 32
	loop .LOOP

	.NotFound:
	pop bx
	pop dx
	pop cx
	mov ax, -1
	ret

	.Found:
	pop ax
	pop bx
	pop dx
	pop cx
	ret

; LoadFile - Load file
; parm/ ES:SI File to load
; parm/ EBX:BP Buffer to load to
; ret/ AX -1 on error, 0 on succes
; ret/ CX Number of sectors to read

LoadFile:
	xor ecx, ecx
	push ecx

	.FIND_FILE:
	push bx ; bx => bp points to buffer to write
	push bp
	call FindFile
	cmp ax, -1
	jne .LOAD_IMAGE_PRE
	pop bp
	pop bx
	pop ecx
	mov ax, -1
	ret

	.LOAD_IMAGE_PRE:
	sub edi, ROOT_OFFSET ; edi will be with es as base, so we substract it first
	sub eax, ROOT_OFFSET

	; get starting cluster
	push WORD ROOT_SEG
	pop es
	mov dx, WORD [es:di + 0x001A]
	mov WORD [cluster], dx
	pop bx ; pop the previous push bp
	pop es ; es is now the bx param (new base)
	push bx
	push es
	call LoadFAT

	.LOAD_IMAGE:
	mov ax, WORD [cluster]
	pop es
	pop bx
	call ClusterLBA
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	call ReadSectors
	pop ecx
	inc ecx
	push ecx
	push bx
	push es
	mov ax, FAT_SEG
	mov es, ax
	xor bx, bx

	; get next cluster
	mov ax, WORD [cluster]
	mov cx, ax
	mov dx, ax
	shr dx, 0x0001 ; divide by two
	add cx, dx ; sum for 3/2

	mov bx, 0
	add bx, cx
	mov dx, WORD [es:bx]
	test ax, 0x0001
	jnz .ODD_CLUSTER

	.EVEN_CLUSTER:
	and dx, 0000111111111111b
	jmp .DONE

	.ODD_CLUSTER:
	shr dx, 0x00004 ; take high 12 bits

	.DONE:
	mov WORD [cluster], dx
	cmp dx, 0x0ff0
	jb .LOAD_IMAGE

	.SUCCESS:
	pop es
	pop bx
	pop ecx
	xor ax, ax
	ret
%endif


