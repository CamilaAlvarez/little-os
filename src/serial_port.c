#include "serial_port.h"
#include "io.h"
#include "stdio.h"

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com),
		SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
	outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void serial_configure_line(unsigned short com)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}	

void serial_configure_buffers(unsigned short com)
{
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(unsigned short com)
{
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

int serial_is_transmit_fifo_empty(unsigned short com)
{
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write_char(unsigned short com, char c)
{
	outb(SERIAL_DATA_PORT(com), c);
}

int serial_write(const char * const severity, char *buf, unsigned int len)
{
	/* Spin until the FIFO queues are empty */
	/* We cannot write ANYTHING until the queues are empty
	 * (not even configuration) */
	while(!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE));

	/* Configure the serial port */
	serial_configure_baud_rate(SERIAL_COM1_BASE, 2);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_configure_buffers(SERIAL_COM1_BASE);
	serial_configure_modem(SERIAL_COM1_BASE);
	
	for(int i = 0; i < strlen((char *)severity); i++)
	{
		serial_write_char(SERIAL_COM1_BASE, *(severity + i));
	}
	for(int i = 0; i < len; i++)
	{
		serial_write_char(SERIAL_COM1_BASE, *(buf + i));
	}
	return len;
}	

