//VGA text mode memory address
#define VGA_MEMORY ((volatile unsigned short *) 0xB8000)
#define VGA_WIDTH 1024
#define VGA_HEIGHT 768

void print_char(char c, unsigned short color, int x, int y) {
  // Ensure background is dark blue (1), and keep the foreground color
  unsigned short final_color = (color & 0x0F00) | 0x0100;

  // Print the character with the specified color
  VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
}

// Function to print a string to the VGA text buffer
void print_string(const char *str, unsigned short color, int x, int y) {
  while (*str) {
      if (*str == '\n') {
          x = 0;
          y++;
          if (y >= VGA_HEIGHT) {
              y = 0; // Reset to the top if we exceed the height
          }
      } else {
          print_char(*str, color, x, y);
          x++;
          if (x >= VGA_WIDTH) {
              x = 0; // Move to the next line if we exceed the width
              y++;
              if (y >= VGA_HEIGHT) {
                  y = 0; // Reset to the top if we exceed the height
              }
          }
      }
      str++;
  }
}

// Function to clear the screen
void clear_screen() {
  // Set the background color to dark blue (1) and foreground color to white (F)
  unsigned short dark_blue_background = 0x1F00; // White on dark blue

  // Fill the screen with spaces having a dark blue background
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
      VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
  }
}

