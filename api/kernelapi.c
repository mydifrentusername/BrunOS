//define everything

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

#define VGA_MEMORY ((volatile unsigned short *) 0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25


void print_char(char c, unsigned short color, int x, int y) {
    unsigned short final_color = (color & 0x0F00) | 0x0100;

    VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
}


void print_string(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0;
            y++;
            if (y >= VGA_HEIGHT) {
                y = 0;
            }
        } else {
            print_char(*str, color, x, y);
            x++;
            if (x >= VGA_WIDTH) {
                x = 0;
                y++;
                if (y >= VGA_HEIGHT) {
                    y = 0;
                }
            }
        }
        str++;
    }
}


void clear_screen() {
    unsigned short dark_blue_background = 0x1F00;

    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background);
    }
}

void delay(int seconds) {
    volatile long int count;
        for (int i = 0; i < seconds * 1000000; i++) {
            count++;
        }
    }

