#include "stdio.h"
#include "framebuffer.h"
#include "serial_port.h"

#define MAX_LENGTH 255
static void write(unsigned char device, char *c, unsigned int len)
{
	switch (device)
	{
		case FRAMEBUFFER:
			fb_write(c, len);
			break;
		case SERIAL_PORT:
			serial_write(c, len);
			break;
		default:
			break;	
	}
}

unsigned int strlen(char *str)
{
	unsigned int len = 0;
	while(*str != '\0')
	{
		len++;
		str++;
	}
	return len;
}

/* We're assuming positive numbers with a maximum length of 255 */
int itos(char *output, int n)
{
	int i = 0, j = MAX_LENGTH - 1;
	char tmp[MAX_LENGTH];
	if (n == 0)
	{
		output[0] = '0';
		output[1] = '\0';
		return 1;
	}
	while (n > 0)
	{
		tmp[j] = (n % 10) + '0';
		i++;
		j--;
		n /= 10;
	}
	for (int k = 0; k < i; k++)
	{
		output[k] = tmp[++j];
	}
	output[i] = '\0';
	return i;  
}

void printf(unsigned char device, char *fmt, ...)
{
	char *c, tmp[MAX_LENGTH];
	unsigned int len;
        /* We know cdecl requires that parameters are pushed
	 * unto the stack, and that the right most are pushed last.
	 * We also know the stack memory is sequential */
	char *args = (char *)&fmt + sizeof(fmt);
	for (c = fmt; *c; c++)
	{
		if (*c != '%')
		{
			write(device, c, 1);
			continue;
		}

		switch (*(++c))
		{
			case 's':
				char *str_arg = *((char **)args);
				len = strlen(str_arg);
				write(device, str_arg, len);
				len = itos((char *)tmp, len);
				args += sizeof(str_arg);
				break;
			case 'd':
				int int_arg = *((int *)args); 
				len = itos((char *)tmp, int_arg);
				write(device, tmp, len);
			        args += sizeof(int_arg);
				break;
			default:
				break;
		}
	}
}
