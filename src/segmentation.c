#include "segmentation.h"

#define TABLE_SIZE    3
#define SEGMENT_BASE  0x0
#define SEGMENT_LIMIT 0xFFFFFFFF

#define SEGMENT_FLAGS       0x0C
#define SEGMENT_CODE_ACCESS 0x9A
#define SEGMENT_DATA_ACCESS 0x92

static struct segment_descriptor gdt_descriptors[TABLE_SIZE];

static void initialize_descriptor(int index, unsigned int base, unsigned int limit, unsigned char type_access, unsigned char flags)
{
	gdt_descriptors[index].limit_low = limit & 0xFFFF;
	gdt_descriptors[index].base_low = base & 0xFFFF;
	gdt_descriptors[index].base_middle = (base >> 16) & 0xFF;
	gdt_descriptors[index].access_byte = type_access;
	gdt_descriptors[index].limit_and_flags = ((limit >> 16) & 0x0F) | ((flags << 4) & 0xF0);
	gdt_descriptors[index].base_high = (base >> 24) & 0xFF;
}

void load_gdt()
{
	struct gdt table;
	initialize_descriptor(0, 0, 0, 0, 0);
	initialize_descriptor(1, SEGMENT_BASE, SEGMENT_LIMIT, SEGMENT_CODE_ACCESS, SEGMENT_FLAGS);
	initialize_descriptor(2, SEGMENT_BASE, SEGMENT_LIMIT, SEGMENT_DATA_ACCESS, SEGMENT_FLAGS);

	table.address = (unsigned int) gdt_descriptors;
	table.size = (unsigned short) (TABLE_SIZE * sizeof(*gdt_descriptors)) - 1;

	lgdtb(table);
	load_segment_regs();
}
