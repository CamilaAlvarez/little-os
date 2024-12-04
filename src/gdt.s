global lgdtb
global load_segment_regs

; lgdtb - loads a GDT
; stack: [esp + 4] GDT table
;        [esp    ] return address
lgdtb:
	lgdt [esp + 4]
	ret

; load_segment_regs - loads segment selectors into registers
load_segment_regs:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	jmp 0x08:flush_cs ; far jump to load cs
flush_cs:
	ret
