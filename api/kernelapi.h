#ifndef KERNELAPI_H
#define KERNELAPI_H

#define VGA_MEMORY ((volatile unsigned short *) 0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define COLOR_BLACK 0x0000
#define COLOR_BLUE 0x0001
#define COLOR_GREEN 0x0002
#define COLOR_CYAN 0x0003
#define COLOR_RED 0x0004
#define COLOR_MAGENTA 0x0005
#define COLOR_BROWN 0x0006
#define COLOR_LIGHT_GRAY 0x0007
#define COLOR_DARK_GRAY 0x0008
#define COLOR_LIGHT_BLUE 0x0009
#define COLOR_LIGHT_GREEN 0x000A
#define COLOR_LIGHT_CYAN 0x000B
#define COLOR_LIGHT_RED 0x000C
#define COLOR_LIGHT_MAGENTA 0x000D
#define COLOR_LIGHT_BROWN 0x000E
#define COLOR_WHITE 0x000F

void print_char(char c, unsigned short color, int x, int y);
void print_string(const char *str, unsigned short color, int x, int y);
void clear_screen();
void delay(int seconds);

#endif