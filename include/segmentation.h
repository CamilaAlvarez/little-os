#ifndef INCLUDE_SEGMENTATION_H
#define INCLUDE_SEGMENTATION_H

struct segment_descriptor
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access_byte;
	unsigned char limit_and_flags;
	unsigned char base_high;
}__attribute__((packed));

struct gdt
{
	unsigned short size;
	unsigned int address;
}__attribute__((packed));

/** load_gdt:
 * Loads GDT and segment selector registries
 */
void load_gdt();
/** lgdtb:
 * Loads a global descriptor table
 * @param gdt A pointer to the address and size to the GDT
 */
void lgdtb(struct gdt table);
/** load_segment_regs:
 * Loads segment seletors into the respective registers
 */
void load_segment_regs();
#endif
