#ifndef INCLUDE_SERIAL_PORT_H
#define INCLUDE_SERIAL_PORT_H

#define SERIAL_COM1_BASE                0x3F8 /* com1 base port => We still communicate using 
						 addresses, but they're independent 
						 from the memory address space */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base+2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base+3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base+4)
#define SERIAL_LINE_STATUS_PORT(base)   (base+5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * the the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** serial_configure_baud_rate:
 * Sets the speed of the data being sent. The default speed of a serial
 * port is 115200 bits/s. The argument is a divisor of that number, hence
 * the resulting speed becomes (115200 / divisor) bits/s.
 *
 * @param com     The COM port to configure
 * @param divisor The divisor
 */ 
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/** serial_configure_line:
 * Configures the line of a given serial port. The port is set to have a 
 * data length of 8 bits, no parity bits, one stop bit and break control 
 * disabled.
 *
 * @param com The serial port to configure
 */
void serial_configure_line(unsigned short com);

/** serial_configure_buffers:
 * Configures the buffers of the given serial port. The port is set to
 * enable FIFO, clear both receiver and transmission FIFO queues, and use
 * 14 bytes as size of the queue.
 *
 * @param com The serial port to configure
 */
void serial_configure_buffers(unsigned short com);

/** serial_configure_modem:
 * Configures the modem of the given serial port. The port is set to
 * ready to transmit and data terminal ready
 *
 * @param com The serial port to configure
 */
void serial_configure_modem(unsigned short com);

/** serial_is_transmit_fifo_empty:
 * Checks whether the transmit FIFO queue is empty or not for the given COM
 * port.
 *
 * @param com The COM port
 * @return 0 if the transmit FIFO queue is not empty
 *         1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned short com);

/** serial_write_char:
 * Writes the given char to the given serial port.
 *
 * @param com The COM port
 * @param c   The char to write
 */
void serial_write_char(unsigned short com, char c);
/** serial_write:
 * Writes len number of characters to COM1 port
 *
 * @param buf The buffer with the characters
 * @param len The number of characters to write
 */
int serial_write(const char * const severity, char *buf, unsigned int len);

#endif
