#ifndef INCLUDE_STDIO_H
#define INCLUDE_STDIO_H

/* Available devices */
#define FRAMEBUFFER 0
#define SERIAL_PORT 1

/* Severity of the logs */
#define LOG_DEFAULT ""
#define LOG_DEBUG   "[DEBUG] "
#define LOG_INFO    "[INFO] "
#define LOG_ERROR   "[ERROR] "

/** strlen:
 * Computes the length of a string.
 *
 * @param str The string in question
 * @return The length of str
 */
unsigned int strlen(char *str);

/** itos:
 * Turns integer into string. It assumes the integer is
 * positive.
 *
 * @param output The string used to result the result
 * @param n The number to turn into string
 * @return The length of the new string
 */
int itos(char *output, int n);

/** printf:
 * Prints a formatted string into a device.
 *
 * @param device The device to use
 * @param fmt The format string
 * @param ... Extra parameters
 */
void printf(unsigned char device, char *fmt, ...);

#endif
