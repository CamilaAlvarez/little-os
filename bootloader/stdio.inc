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

%endif ; __STDIO_INC_67343546FDCC56AAB872_INCLUDED__	
