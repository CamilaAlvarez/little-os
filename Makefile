DEST := build
OBJECTS := $(DEST)/loader.o $(DEST)/kmain.o $(DEST)/io.o $(DEST)/framebuffer.o $(DEST)/serial_port.o $(DEST)/stdio.o
CC = x86_64-linux-gnu-gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Werror -c
LDFLAGS = -T image/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf
SRC := src

all: $(DEST)/kernel.elf

$(DEST)/kernel.elf: $(OBJECTS)
	x86_64-linux-gnu-ld $(LDFLAGS) $(OBJECTS) -o $(DEST)/kernel.elf

os.iso: $(DEST)/kernel.elf
	cp $(DEST)/kernel.elf image/iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		image/iso

run: os.iso
	bochs -f bochsrc.txt -q

$(DEST)/%.o: $(SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I include/ $< -o $@

$(DEST)/%.o: $(SRC)/%.s
	mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf build/ os.iso image/iso/boot/kernel.elf	
		
