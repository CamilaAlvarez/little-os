DEST := build
OBJECTS := $(DEST)/Boot.bin

$(DEST)/%.bin : %.asm
	mkdir -p $(DEST)
	nasm -f bin $< -o $@

$(DEST)/floppy: $(OBJECTS)
	mkfs.msdos -C $(DEST)/floppy 1440
	dd if=$(DEST)/Boot.bin of=$(DEST)/floppy conv=notrunc

run: $(DEST)/floppy
	bochs -f bochsrc.txt -q

clean:
	rm -r build
