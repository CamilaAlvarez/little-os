#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

#define FB_MAX_POS 80*25

#define FB_BLACK          0
#define FB_BLUE           1
#define FB_GREEN          2
#define FB_CYAN           3
#define FB_RED            4
#define FB_MAGENTA        5
#define FB_BROWN          6
#define FB_LIGHT_GREY     7
#define FB_DARK_GREY      8
#define FB_LIGHT_BLUE     9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE         15

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/** fb_write_cell
 * Writes a character with the given foreground and background to position i
 * in the framebuffer
 * @param i location in the framebuffer
 * @param c the character
 * @param fg the foreground color
 * @param bg the background color
*/
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 *
 * @param pos The new position of the cursor (short because pos is 16bits)
 */
void fb_move_cursor(unsigned short pos);

/** fb_get_cursor:
 * Get the cursor's current position
 */
unsigned short fb_get_cursor();

/** fb_write:
 * Writes buffer into the framebuffer
 *
 * @param buf Buffer to write to the framebuffer
 * @param len Number of characters to write
 */
int fb_write(const char * const severity, char *buf, unsigned int len); 
#endif
