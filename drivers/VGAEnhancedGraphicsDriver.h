#ifndef DRIVERS_VGAENHANCEDGRAPHICSDRIVER_H
#define DRIVERS_VGAENHANCEDGRAPHICSDRIVER_H


#define VGA_WIDTH 320
#define VGA_HEIGHT 200
#define VGA_BUFFER 0xA0000 // VGA memory address


void set_mode(unsigned char mode);
void clear_screeng(unsigned char color);
void put_pixel(int x, int y, unsigned char color);
void draw_rectangle(int x, int y, int width, int height, unsigned char color);


#endif