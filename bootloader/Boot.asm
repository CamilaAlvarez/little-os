org 0 ; we'll set the registers by hand later

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

Print:
	lodsb
	or al, al
	jz PrintDone
	mov ah, 0eh
	int 10h
	jmp Print
PrintDone:
	ret

ReadSectors:
	.MAIN:
		mov di, 0x0005 ; five retries per sector
	.SECTORLOOP:
		push ax
		push bx
		push cx
		call LBACHS
		mov ah, 0x02
		mov al, 0x01
		mov ch, BYTE [absoluteTrack]
		mov cl, BYTE [absoluteSector]
		mov dh, BYTE [absoluteHead]
		mov dl, BYTE [bsDriveNumber]
		int 0x13
		jnc .SUCCESS
		xor ax, ax
		int 0x13
		dec di ; decrement error counter
		pop cx
		pop bx
		pop ax
		jnz .SECTORLOOP ; if counter is not zero => try again
		int 0x18 ; error while booting (could not find boot file)
	.SUCCESS:
		mov si, msgProgress
		call Print
		pop cx
		pop bx
		pop ax
		add bx, WORD [bpbBytesPerSector]
		inc ax
		loop .MAIN ; automatically reduces cx => it stops once it reaches 0
		ret

ClusterLBA:
	sub ax, 0x0002
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	mul cx ; ax = ax * cx
	add ax, WORD [datasector]
	ret

LBACHS:
	xor dx, dx
	div WORD [bpbSectorsPerTrack] ;ax = ax/bpbSectorsPerTrack (remainder in dx)
	inc dl
	mov BYTE [absoluteSector], dl ; store sector 
	xor dx, dx
	div WORD [bpbHeadsPerCylinder] ; ax already contains ax/bpbSectorsPerTrack
	mov BYTE [absoluteHead], dl
	mov BYTE [absoluteTrack], al
	ret
	
		
loader:

; adjust segment registers
; In real mode we don't need a GDT
	cli
	mov ax, 0x07C0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
; create stack
	mov ax, 0x0
	mov ss, ax
	mov sp, 0xFFFF
	sti ; retore interrupts
; Display loading message
	mov si, msgLoading
	call Print

; Load root directory table
	.LOAD_ROOT:
		; compute size of root directory and store in cx
		; we'll use it as a counter to go over the directory
		xor cx, cx
		xor dx, dx
		mov ax, 0x0020 ; 32 bytes directory entry
		mul WORD [bpbRootEntries]
		div WORD [bpbBytesPerSector]
		xchg ax, cx ; ax= cx and cx = ax
		
		; compute location of root directory and store in ax
		mov al, BYTE [bpbNumberOfFATs]
		mul WORD [bpbSectorsPerFAT]
		add ax, WORD [bpbReservedSectors]
		mov WORD [datasector], ax
		add WORD [datasector], cx ; datasector now contains the start of the address data area

		; read root directory into memory (we'll copy it to 7C00:0200)
		
		mov bx, 0x0200
		call ReadSectors

; Find stage 2
; We have the root directory loaded at 0x7C00:0200

		mov cx, WORD [bpbRootEntries] ; the loop stops once we have gone through all the files
		mov di, 0x0200
	.LOOP:
		push cx
		mov cx, 0x000B ; 11 char name
		mov si, ImageName
		push di
		rep cmpsb
		pop di
		je LOAD_FAT
		pop cx
		add di, 0x0020 ; add 32, next directory entry (32 bytes apart)
		loop .LOOP
		jmp FAILURE ; we couldn't find the file

; LOAD FAT
	LOAD_FAT:
		; Save starting cluster of boot image

		mov si, msgCRLF
		call Print
		mov dx, WORD [di + 0x001A]
		mov WORD [cluster], dx

		; compute size of FAT and store in cx

		xor ax, ax
		mov al, BYTE [bpbNumberOfFATs]
		mul WORD [bpbSectorsPerFAT]
		mov cx, ax

		; compute location of FAT and store in ax

		mov ax, WORD [bpbReservedSectors] ; adjust for bootsector
		
		; Read FAT into memory (7C00:0200)

		mov bx, 0x0200
		call ReadSectors

		; read image file into memory (0050:0000)

		mov si, msgCRLF
		call Print
		mov ax, 0x0050
		mov es, ax ; destination for image
		mov bx, 0x0000 ; destination for image
		push bx

	LOAD_IMAGE:
		mov ax, word [cluster]
		pop bx
		call ClusterLBA
		xor cx, cx
		mov cl, BYTE [bpbSectorsPerCluster]
		call ReadSectors
		push bx
	; compute next cluster

		mov ax, WORD [cluster]
		mov cx, ax
		mov dx, ax
		shr dx, 0x0001
		add cx, dx
		mov bx, 0x0200
		add bx, cx
		mov dx, WORD [bx]
		test ax, 0x0001
		jnz .ODD_CLUSTER
	.EVEN_CLUSTER:
		and dx, 0000111111111111b
		jmp .DONE
	.ODD_CLUSTER:
		shr dx, 0x0004
	.DONE:
		mov WORD [cluster], dx
		cmp dx, 0x0ff0 ; test for the end of the file
		jb LOAD_IMAGE ; jump if below (unsigned)

	DONE:
		mov si, msgCRLF
		call Print
		push WORD 0x0050
		push WORD 0x0000
		retf
	FAILURE:
		mov si, msgFailure
		call Print
		mov ah, 0x00
		int 0x16 ; await keypress
		int 0x19 ; warm boot computer

	; Variables

	absoluteSector db 0x00
	absoluteHead db 0x00
	absoluteTrack db 0x00

	datasector dw 0x0000
	cluster dw 0x0000
	ImageName db "KRNLDR  SYS"
	msgLoading db 0x0D, 0x0A, "Loading Boot Image ", 0x0D, 0x0A, 0x00
	msgCRLF db 0x0D, 0x0A, 0x00
        msgProgress db ".", 0x00
        msgFailure  db 0x0D, 0x0A, "ERROR : Press Any Key to Reboot", 0x0A, 0x00 

times 510 - ($ - $$) db 0
dw 0xAA55

