#include "io.h"
#include "framebuffer.h"
#include "serial_port.h"

int main()
{
	char *str = "Hello World";
	int len = 11;
	fb_write(str, len);
	serial_write(str, len);
}
