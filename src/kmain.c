#include "io.h"
#include "framebuffer.h"
#include "serial_port.h"
#include "stdio.h"
#include "segmentation.h"

int main()
{
	char *str = "Hello World";
	fb_write(LOG_INFO, str, strlen(str));
	load_gdt();
}
