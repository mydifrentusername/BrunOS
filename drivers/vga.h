#ifndef DRIVERS_VGA_H
#define DRIVERS_VGA_H

//definitions:
#define VGA_MEMORY ((volatile unsigned short *) 0xB8000)
#define VGA_WIDTH 1024
#define VGA_HEIGHT 768

void print_char(char c, unsigned short color, int x, int y);
void print_string(const char *str, unsigned short color, int x, int y);
void clear_screen();

#endif