DEST := build
OBJECTS := $(DEST)/Boot.bin $(DEST)/KRNLDR.SYS $(DEST)/KRNL.SYS 
INCLUDES := Gdt.inc stdio.inc A20.inc Fat12.inc Floppy16.inc
$(DEST)/%.bin : %.asm
	mkdir -p $(DEST)
	nasm -f bin $< -o $@
$(DEST)/KRNL.SYS : Stage3.asm $(INCLUDES)
	nasm -f bin $< -o $@
$(DEST)/KRNLDR.SYS : Stage2.asm $(INCLUDES)
	nasm -f bin $< -o $@ 
$(DEST)/floppy: $(OBJECTS) 
	mkdir -p mount
	mkfs.msdos -C $(DEST)/floppy 1440
	dd if=$(DEST)/Boot.bin of=$(DEST)/floppy conv=notrunc
	sudo mount -o loop build/floppy mount/
	sudo cp $(DEST)/KRNLDR.SYS mount/
	sudo cp $(DEST)/KRNL.SYS mount/
	sudo umount mount/

run: $(DEST)/floppy
	bochs -f bochsrc.txt -q

clean:
	rm -r build
	rm -r mount/
