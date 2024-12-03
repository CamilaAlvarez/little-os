#include "io.h"
#include "framebuffer.h"
#include "serial_port.h"
#include "stdio.h"

int main()
{
	char *str = "Hello World";
	printf(SERIAL_PORT, "%s %d %s%d", str, 2, "again", 4506);
}
