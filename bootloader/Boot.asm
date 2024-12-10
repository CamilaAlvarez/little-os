org 0x7c00

bits 16

start:          jmp loader					; jump over OEM block

;*************************************************;
;	OEM Parameter block
;*************************************************;

TIMES 0Bh-$+start DB 0

bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 	            DB 0xF0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "

msg db "Welcome to My Operating System", 0

print:
	lodsb
	or al, al
	jz print_done
	mov ah, 0eh
	int 10h
	jmp print
print_done:
	ret 
loader:
.reset:
	mov ah, 0 ; function 0
	mov dl, 0 ; drive 0
	int 0x13 ; reset drive interruption
	jc .reset ; if it fails carry flag is set (we retry)

	mov ax, 0x1000
	mov es, ax
	xor bx, bx ; sectors will be loaded to 0x1000:0

	mov ah, 0x02 ; operation 0x02
	mov al, 1 ; read 1 sector
	mov ch, 1 ; from track 1
	mov cl, 1 ; read second sector
	mov dh, 0 ; head number
	mov dl, 0 ; drive number
	int 0x13
	jmp 0x1000:0 ; far jmp to second stage bootloader 

times 510 - ($ - $$) db 0
dw 0xAA55

org 0x1000

cli 
hlt
