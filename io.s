global outb
global inb

; outb - send a byte to an I/O port
; stack: [esp + 8] the data byte
;        [esp + 4] the I/O port address
;        [esp    ] return address  
outb:
	mov al, [esp + 8] ; we use ebp, remember that we can only send a byte
	mov dx, [esp + 4]
	out dx, al
	ret 
inb:
	mov dx, [esp + 4]
	in al, dx
	ret
