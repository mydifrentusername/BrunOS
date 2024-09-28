#include "vga.h"

//graphics mode:


#define VGA_WIDTH 320
#define VGA_HEIGHT 200
#define VGA_BUFFER 0xA0000 // VGA memory address


static unsigned char* vga_buffer = (unsigned char*)VGA_BUFFER;


void set_mode(unsigned char mode);
void clear_screeng(unsigned char color);
void put_pixel(int x, int y, unsigned char color);
void draw_rectangle(int x, int y, int width, int height, unsigned char color);


void set_mode(unsigned char mode) {
    asm volatile (
        "movb %0, %%al\n"
        "movb $0x00, %%ah\n"
        "int $0x10"
        : 
        : "r"(mode)
        : "%al", "%ah"
    );
}


void clear_screeng(unsigned char color) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            put_pixel(x, y, color);
        }
    }
}


void put_pixel(int x, int y, unsigned char color) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        vga_buffer[y * VGA_WIDTH + x] = color;
    }
}


void draw_rectangle(int x, int y, int width, int height, unsigned char color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            put_pixel(x + j, y + i, color);
        }
    }
}


void init_vga_driver() {
    set_mode(0x13);
    clear_screeng(0);
}