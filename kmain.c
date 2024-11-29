#include "io.h"
#include "framebuffer.h"

static unsigned char *fb = (unsigned char *)0x000B8000;
static unsigned short current_pos = 0;
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	fb[i] = c;
	fb[i+1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}
int write(char *buf, unsigned int len)
{
	//TODO: obtain current cursor pos (for now we're using a global variable) 
	for (int i = 0; i < len; i++)
	{
		fb_write_cell(current_pos, buf[i], FB_GREEN, FB_DARK_GREY);
		current_pos += 2; // because each character uses two spaces in the fb
		if (current_pos >= FB_MAX_POS)
		{
			return i;
		}	
		fb_move_cursor(current_pos);
		
	}
	// TODO: We might not be able to write all
	return len;
}
