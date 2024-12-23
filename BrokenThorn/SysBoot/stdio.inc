%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__

bits 16

Puts16:
	pusha
.Loop1:
	lodsb
	or al, al
	jz Puts16Done
	mov ah, 0eh
	int 10h
	jmp .Loop1
Puts16Done:
	popa
	ret
; 32 bit mode protected routines

bits 32

%define VIDMEM 0xB8000 ; video memory (text with color)
%define COLS 80
%define LINES 25
%define CHAR_ATTRIB 63 ; white text on light blue background

_CurX db 0
_CurY db 0

Putch32:
	pusha
	mov edi, VIDMEM ; get pointer to video memory

	xor eax, eax ; clear eax
	; Get location
	mov ecx, COLS*2 ; 2 bytes per char in Mode 7
	mov al, byte [_CurY] ; store _CurY value in al
	mul ecx ; al = al * ecx => curY = curY*2*COLS
	push eax

	mov al, byte [_CurX]
	mov cl, 2
	mul cl ; cx = 2*cx
	pop ecx ; ecx = 2*cols*cury
	add eax, ecx

	xor ecx, ecx ; we won't be using this register so we clean it
	add edi, eax ; add 2*cx + 2*cols*cury to the base address

	; What out of new line
	cmp bl, 0x0A
	je .Row

	mov dl, bl ; char in dl
	mov dh, CHAR_ATTRIB ; attributes in dh 
	mov word [edi], dx ; print the char

	inc byte [_CurX]
	cmp byte [_CurX], COLS ; have we reached the end of the line
	je .Row
	jmp .done

.Row:
	mov byte [_CurX], 0 ; go back to col 0
	inc byte [_CurY] ; move one row down

.done:
	popa
	ret ; restore registers and return

; Prints a null terminated string
; param\ EBX = address of string to print
Puts32:
	pusha ; store current registers
	push ebx
	pop edi

.loop:
	mov bl, byte [edi]
	cmp bl, 0
	je .done

	call Putch32

	inc edi
	jmp .loop

.done:
	mov bh, byte [_CurY]
	mov bl, byte [_CurX]
;	call MovCur

	popa
	ret

; MoveCur: Update hardware cursor
; param/ bh = Y pos
; parm / bl = X pos

bits 32

MovCur:
	pusha

	xor eax, eax
	mov ecx, COLS
	mov al, bh
	mul ecx
	add al, bl 
	mov ebx, eax ; ebx = x + y*cols

	; send low byte
	mov al, 0x0f
	mov dx, 0x03D4 ; index register
	out dx, al

	mov al, bl
	mov dx, 0x03D5
	out dx, al

	xor eax, eax

	; send high byte
	mov al, 0x0e
	mov dx, 0x03D4
	out dx, al

	mov al, bh
	mov dx, 0x03D5
	out dx, al

	popa
	ret

; ClrScr32() - Clears Screen

bits 32

ClrScr32:
	pusha
	cld
	mov edi, VIDMEM
	mov cx, 2000
	mov ah, CHAR_ATTRIB
	mov al, ' '
	rep stosw

	mov byte [_CurX], 0
	mov byte [_CurY], 0
	popa
	ret

; GotoXY - set the current X/Y locations
; param\ AL=X
; param\ AH=Y

bits 32

GotoXY:
	pusha
	mov [_CurX], al
	mov [_CurY], ah
	popa
	ret


%endif ; __STDIO_INC_67343546FDCC56AAB872_INCLUDED__	
