DEST := build
OBJECTS := $(DEST)/Boot.bin $(DEST)/KRNLDR.SYS
INCLUDES := Gdt.inc stdio.inc
$(DEST)/%.bin : %.asm
	mkdir -p $(DEST)
	nasm -f bin $< -o $@
$(DEST)/KRNLDR.SYS : Stage2.asm $(INCLUDES)
	nasm -f bin Stage2.asm -o $(DEST)/KRNLDR.SYS
$(DEST)/floppy: $(OBJECTS) 
	mkdir -p mount
	mkfs.msdos -C $(DEST)/floppy 1440
	dd if=$(DEST)/Boot.bin of=$(DEST)/floppy conv=notrunc
	sudo mount -o loop build/floppy mount/
	sudo cp $(DEST)/KRNLDR.SYS mount/
	sudo umount mount/

run: $(DEST)/floppy
	bochs -f bochsrc.txt -q

clean:
	rm -r build
	rm -r mount/
