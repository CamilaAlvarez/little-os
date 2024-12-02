#include "io.h"
#include "framebuffer.h"

int main()
{
	char *str = "Hello World";
	int len = 11;
	fb_write(str, len);
}
