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

#include "vga.h"

static inline uint8_t inb(uint16_t port);
static inline unsigned char port_read(unsigned short port);

typedef unsigned char uint8_t;  // 8-bit unsigned integer
typedef unsigned short uint16_t; // 16-bit unsigned integer
typedef unsigned int uint32_t;   // 32-bit unsigned integer

// Function to read a byte from the keyboard port
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline unsigned char port_read(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char scan_code_to_ascii(unsigned char scan_code) {
  // Simple mapping from scan code to ASCII (for demonstration)
  switch (scan_code) {
      case 0x1E: return 'a';
      case 0x30: return 'b';
      case 0x2E: return 'c';
      case 0x20: return 'd';
      case 0x12: return 'e';
      case 0x21: return 'f';
      case 0x22: return 'g';
      case 0x23: return 'h';
      case 0x17: return 'i';
      case 0x24: return 'j';
      case 0x25: return 'k';
      case 0x26: return 'l';
      case 0x32: return 'm';
      case 0x31: return 'n';
      case 0x18: return 'o';
      case 0x19: return 'p';
      case 0x10: return 'q';
      case 0x13: return 'r';
      case 0x1F: return 's';
      case 0x14: return 't';
      case 0x16: return 'u';
      case 0x2F: return 'v';
      case 0x11: return 'w';
      case 0x2D: return 'x';
      case 0x15: return 'y';
      case 0x2C: return 'z';
      case BACKSPACE: return '\b';
      case ENTER: return '\n';
      case SPACE: return ' '; // Space character
      default: return 0; // Unknown scan code
  }
}

// Main keyboard input handling function
void keyboard_input_handler() {
  unsigned char scan_code = 0;

  // Wait for user input
  while (scan_code == 0) {
      if (port_read(0x64) & 0x1) {
          scan_code = port_read(0x60);
      } else {
          print_error("ERROR: keyboard driver | problem");
          return; // Exit the handler if there's an issue
      }
  }

  char c = scan_code_to_ascii(scan_code);
  static char input_buffer[80]; // Buffer to hold the current line of input
  static int buffer_index = 0; // Current index in the input buffer
  static int x = 0, y = 0; // Current cursor position for output

  // Print the character if it's valid
  if (c) {
      if (c == '\b') { // Backspace
          if (buffer_index > 0) {
              buffer_index--;
              x--;
              print_char(' ', 0x07, x, y); // Clear the character from the screen
          }
      } else if (c == '\n') { // Enter
          input_buffer[buffer_index] = 0; // Null-terminate the string
          // You can process the input buffer here
          // For example, you can pass it to a function that handles the input
          buffer_index = 0; // Reset buffer index for the next line
          y++; // Move to the next line
          if (y >= VGA_HEIGHT) {
              y = 0; // Reset to the top if we exceed the height
          }
          x = 0; // Reset x to the start of the line
      } else if (buffer_index < 80) { // Check buffer size
          input_buffer[buffer_index++] = c; // Store in the buffer
          print_char(c, 0x07, x++, y); // Print the character to the screen
          if (x >= VGA_WIDTH) { // Move to the next line if we exceed the width
              x = 0;
              y++;
              if (y >= VGA_HEIGHT) {
                  y = 0; // Reset to the top if we exceed the height
              }
          }
      }
  } else {
      print_error("ERROR: keyboard driver | problem");
  }
}

