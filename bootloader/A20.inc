%ifndef __A20_INC_67343546FDCC56AAB872_INCLUDED__
%define __A20_INC_67343546FDCC56AAB872_INCLUDED__

bits 16

wait_input:
	in al, 0x64 ; controller command port
	test al, 2 ; input buffer status
	jnz wait_input 
	ret

wait_output:
	in al, 0x64
	test al, 1 ; output buffer status
	jnz wait_output
	ret

EnableA20:
	cli ; disable interrupts
	pusha ; store all registers

	; send read output command port
	mov al, 0xD0
	out 0x64, al
	call wait_output

	; read input buffer ad store on stack
	in al, 0x60
	push eax
	call wait_input

	; send write output port command
	mov al, 0xD1
	out 0x64, al
	call wait_input

	; pop output port data and enable A20
	pop eax
	or al, 2
	out 0x60, al
	
	call wait_input
	mov al, 0xAE ; enable keyboard
	out 0x64, al

	call wait_input
	popa
	stri
	ret

%endif
