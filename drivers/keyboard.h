#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

// Keyboard definitions
#define KEYBOARD_DATA_PORT 0x60 // Example keyboard data port
#define KEY_LEFT_SHIFT 0x2A
#define KEY_RIGHT_SHIFT 0x36

#define KEY_UP_ARROW 0x48
#define KEY_DOWN_ARROW 0x50
#define KEY_LEFT_ARROW 0x4B
#define KEY_RIGHT_ARROW 0x4D

#define KEYBOARD_PORT 0x60

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SPACE 0x39 // Scan code for the space bar

// Include VGA header
#include "vga.h"

typedef unsigned char uint8_t;  // 8-bit unsigned integer
typedef unsigned short uint16_t; // 16-bit unsigned integer
typedef unsigned int uint32_t;   // 32-bit unsigned integer

static inline uint8_t inb(uint16_t port);
static inline unsigned char port_read(unsigned short port);

// Function to read a byte from the keyboard port (inb)
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}
// Function to read a byte from a port (port_read)
static inline unsigned char port_read(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Function to convert scan codes to ASCII characters
char scan_code_to_ascii(unsigned char scan_code);

// Main keyboard input handling function
void keyboard_input_handler();

#endif