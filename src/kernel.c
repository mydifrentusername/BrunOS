//687 line
//860 line
//914 line

//VGA text mode memory address
#define VGA_MEMORY ((volatile unsigned short *) 0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

//Memory management definitions
#define NULL ((void*)0) // Define NULL
#define MEMORY_POOL_SIZE 1024 // Define the size of the memory pool

//keyboard scancode definitions for the shift key
#define KEYBOARD_DATA_PORT 0x60 // Example keyboard data port
#define KEY_LEFT_SHIFT 0x2A
#define KEY_RIGHT_SHIFT 0x36

#define VK_RCONTROL 0xA3  // Right Control key
#define VK_DELETE    0x2E  // Delete key


//time define
#define RTC_PORT 0x70 // Example RTC port (Check your system's documentation)
#define DELAY_MS 1000 // Update the time every 1000 milliseconds (1 second)

//Custom type definitions
typedef unsigned char uint8_t;  // 8-bit unsigned integer
typedef unsigned short uint16_t; // 16-bit unsigned integer
typedef unsigned int uint32_t;   // 32-bit unsigned integer


//include /cpu/
#include "../cpu/timer.h"
#include "../cpu/gdt.h"
#include "../cpu/idt.h"


//include /syscalls/
#include "../syscalls/syscalls.h"
#include "../syscalls/assembly/assembly.h"

//include /multitasking/
#include "../multitasking/multitasking.h"


//include /memory/
#include "../memory/paging.h"
#include "../memory/segmentation.h"
#include "../memory/virtualmemory.h"

//include drivers
//#include "../drivers/vga.h"
//#include "../drivers/keyboard.h"
//#include "../drivers/VGAEnhancedGraphicsDriver.h"
#include "../hardware/usbdrivers/usb.h"

//fat32
#include "../fat/fat32.h"


// Memory pool for allocation
char memory_pool[MEMORY_POOL_SIZE]; // Create a memory pool
int memory_used = 0; // Track used memory

// Function to allocate memory from the pool
void* allocate_memory(int size) {
    if (memory_used + size > MEMORY_POOL_SIZE) {
        return NULL; // Not enough memory
    }
    void* ptr = &memory_pool[memory_used]; // Allocate memory
    memory_used += size; // Update used memory
    return ptr; // Return pointer to allocated memory
}

// Function to free memory (not actually reclaiming it)
void free_memory(int size) {
    if (memory_used >= size) {
        memory_used -= size; // Free memory (not actually reclaiming it)
    }
}

// Cursor position
static unsigned int cursor_x = 0;
static unsigned int cursor_y = 0;

// Keyboard port
#define KEYBOARD_PORT 0x60

// Function to read a byte from the keyboard port
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Function to print a character to the VGA text buffer
void print_char(char c, unsigned short color, int x, int y) {
    // Ensure background is dark blue (1) and keep the foreground color
    unsigned short final_color = (color & 0x8700) | 0x8000; // Dark blue background

    // Calculate the position in the VGA memory
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
    }
}

// Function to print a string to the VGA text buffer
void print_string(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0; // Reset x to the start of the line
            y++; // Move to the next line
            if (y >= VGA_HEIGHT) {
                y = 0; // Reset to the top if we exceed the height
            }
        } else {
            print_char(*str, color, x, y); // Print the character
            x++; // Move to the next column
            if (x >= VGA_WIDTH) {
                x = 0; // Reset x to the start of the next line
                y++; // Move to the next line
                if (y >= VGA_HEIGHT) {
                    y = 0; // Reset to the top if we exceed the height
                }
            }
        }
        str++; // Move to the next character in the string
    }
}


// Function to clear the screen
void clear_screen() {
    // Set the background color to dark blue (1) and foreground color to white (F)
    unsigned short dark_blue_background = 0x888888; // White on dark blue: 0x1F00,0x7F7F7F

    // Fill the screen with spaces having a dark blue background
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
    }
}

//keyboard

static inline uint8_t inb(uint16_t port);
static inline unsigned char port_read(unsigned short port);

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SPACE 0x39 // Scan code for the space bar

// Function to read a byte from a port
static inline unsigned char port_read(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Function to print an error message to the screen
void print_error(const char *message) {
    print_string(message, 0x0C, 0, 24); // Print in red at the bottom of the screen
}

// Function to convert scancodes to ASCII characters
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
        //numbers
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
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

// Define menu options
#define MAX_MENU_OPTIONS 3
const char *menu_options[MAX_MENU_OPTIONS] = {
    "File",
    "Exit",
    "Help"
};

//Define right selections

#define MAX_RIGHT_SELECTIONS 6
const char *right[MAX_RIGHT_SELECTIONS] = {
    "",
    "",
    "",
    "Settings",
    "BrunOS browser",
    "Brunos media player"
};




// Define selections
#define MAX_SELECTIONS 8
const char *selections[MAX_SELECTIONS] = {
    "======================================================================================",
    "Readme",
    "Calc.bin",
    "notepad.bin",
    "Explorer.bin",
    "Write.bin",
    "Shell.bin",
    "Updates"
};

int selected_index = 0; // Index of the currently selected option (menu or selection)

// Define help message
const char *help_message = "HELP: This OS is not a fully kernel but it's a codename OS and a better kernel will be out soon! You can also read the README for more help.";

// Function to print menu and selections with highlighting
void print_menu_and_selections() {
    unsigned short white_on_dark_blue = 0x1800; // White on dark blue
    unsigned short black_on_white = 0x0700; // Highlight color

    // Clear the screen before printing menu and selections
    clear_screen();

    // Print menu options
    for (int i = 0; i < MAX_MENU_OPTIONS; i++) {
        if (i == selected_index && selected_index < MAX_MENU_OPTIONS) {
            print_string(menu_options[i], white_on_dark_blue, i * 20, 0); // Highlight selected menu option
        } else {
            print_string(menu_options[i], black_on_white, i * 20, 0); // Normal color for unselected menu options
        }
    }

    // Print selections
    for (int i = 0; i < MAX_SELECTIONS; i++) {
        if (i == selected_index - MAX_MENU_OPTIONS) { // Adjust index for selections
            print_string(selections[i], white_on_dark_blue, 0, i + 1); // Highlight selected selection
        } else {
            print_string(selections[i], black_on_white, 0, i + 1); // Normal color for unselected selections
        }
    }
    
    //print right selections
    for (int i = 0; i < MAX_RIGHT_SELECTIONS; i++) {
        if (i == selected_index - MAX_SELECTIONS) { // Adjust index for selections
            print_string(right[i], white_on_dark_blue, 13, i + 1); // Highlight selected selection
        } else {
            print_string(right[i], black_on_white, 13, i + 1); // Normal color for unselected selections
        }
    }
}

// Function to handle keyboard input
void handle_input() {
    static uint8_t last_key = 0; // Store the last key pressed
    uint8_t key = inb(KEYBOARD_PORT);

    // Check for key press (assuming key codes for up arrow, down arrow, left arrow, right arrow, and enter are 0x48, 0x50, 0x4B, 0x4D, and 0x1C respectively)
    if (key == 0x48 && last_key != 0x48) { // Up arrow
        if (selected_index > 0) {
            selected_index--;
        }
        print_menu_and_selections(); // Redraw the menu and selections with updated selection
    } else if (key == 0x50 && last_key != 0x50) { // Down arrow
        if (selected_index < MAX_MENU_OPTIONS + MAX_SELECTIONS + MAX_RIGHT_SELECTIONS - 1) {
            selected_index++;
        }
        print_menu_and_selections(); // Redraw the menu and selections with updated selection
    } else if (key == 0x4D && last_key != 0x4D) { // Right arrow
        if (selected_index < MAX_MENU_OPTIONS - 1) {
            selected_index++;
        }
        print_menu_and_selections(); // Redraw the menu and selections with updated selection
    } else if (key == 0x4B && last_key != 0x4B) { // Left arrow
        if (selected_index > 0) {
            selected_index--;
        }
        print_menu_and_selections(); // Redraw the menu and selections with updated selection
    } else if (key == 0x1C && last_key != 0x1C) { // Enter key
        if (selected_index == 1) { // Exit option selected
            unsigned short dark_blue_background = 0x0000; // White on dark blue

            // Fill the screen with spaces having a dark blue background
            for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
                VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
            }
            clear_screen();
            print_stringb("================================================================================" , 0x1F00, 0, 0);
            print_stringb("                                                                                 " , 0x1F00, 0, 1);
            print_stringb("                                                                                 " , 0x1F00, 0, 2);
            print_stringb("                                                                                 " , 0x1F00, 0, 3);
            print_stringb("                                                                                 " , 0x1F00, 0, 4);
            print_stringb("================================================================================" , 0x1F00, 0, 5);

            print_string("                                BrunOS is shutting down...                                ", 0x1F00, 0, 11);
            
            print_stringb("================================================================================" , 0x1F00, 0, 24);
            print_stringb("                                                                                 " , 0x1F00, 0, 23);
            print_stringb("                                                                                 " , 0x1F00, 0, 22);
            print_stringb("                                                                                 " , 0x1F00, 0, 21);
            print_stringb("                                                                                 " , 0x1F00, 0, 20);
            print_stringb("================================================================================" , 0x1F00, 0, 19);
            delay(444);
            for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
                VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
            }
            print_string("", 0x07, 0, 0); // Print nothing to clear the screen
            print_string("", 0x07, 0, 1); // Print nothing to clear the screen
            print_string("", 0x07, 0, 2); // Print nothing to clear the screen
            print_string("", 0x07, 0, 3); // Print nothing to clear the screen
            print_string("", 0x07, 0, 4); // Print nothing to clear the screen
            print_string("", 0x07, 0, 5); // Print nothing to clear the screen
            print_string("                         ======================================", 0x07, 0, 6);
            print_string("                         |You can now turn off your computer. |", 0x0004, 0, 7); // Print exit message
            print_string("                         ======================================", 0x07, 0, 8);
            // Shut down the machine
            __asm__ volatile ("cli; hlt");
        } else if (selected_index == 2) { // Help option selected
            clear_screen(); // Clear the screen
            print_string(help_message, 0x1F00, 0, 24); // Print help message
            // Wait for a key press to return to the menu
            while (1) {
                if (inb(KEYBOARD_PORT)) { // Wait for any key press
                    break; // Exit the loop on key press
                }
            }
            print_menu_and_selections(); // Redraw the menu and selections
        } else if (selected_index == 4) { // Help option selected
            readme();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 5) { // Help option selected
            calculatorbin();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 6) { // Help option selected
            notepadbin();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 7) {
            explorerbin();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 8) {
            writebin();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 9) {
            shellbin();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 10) {
            updates();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 0) {
            filemenu();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 2) {
            helpmenu();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 11) {
            settingsapp();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (selected_index == 12) {
            browser();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        } else if (key == VK_RCONTROL && last_key != VK_RCONTROL) {
            task_manager();
        } else if (selected_index == 13) {
            Brunos_media_player();
            if (key == 0x4D && last_key != 0x4D)
                kmain();
        }
    }

    last_key = key;
}


// Function prototype for printchar2
void printchar2(char c, unsigned short color, int x, int y);

// Function to print a character to the screen
void printchar2(char c, unsigned short color, int x, int y) {
    int offset = (y * VGA_WIDTH + x) * 2;
    *(char *)(0xb8000 + offset) = c; // Character
    *(char *)(0xb8000 + offset + 1) = color; // Color attribute
}

// Function to convert scancodes to ASCII characters
char scan_code_to_ascii(unsigned char scan_code);

// Function to read a byte from a port
unsigned char port_read(unsigned short port);

// Function to allow typing on the screen
void type_anything() {
    char input_buffer[80]; // Buffer to hold the current line of input
    int buffer_index = 0; // Current index in the input buffer
    int x = 0, y = 5; // Current cursor position for output
    unsigned char scan_code;

    // Clear the input buffer
    for (int i = 0; i < 80; i++) {
        input_buffer[i] = 0;
    }

    // Main input loop
    while (1) {
        // Wait for user input
        while (scan_code == 0) {
            if (port_read(0x64) & 0x1) {
                scan_code = port_read(0x60);
            }
        }

        char c = scan_code_to_ascii(scan_code);

        // Print the character if it's valid
        if (c) {
            if (c == '\b') { // Backspace
                if (buffer_index > 0) {
                    buffer_index--;
                    x--;
                    printchar2(' ', 0x07, x, y); // Clear the character from the screen
                }
            } else if (buffer_index < 80) { // Check buffer size
                input_buffer[buffer_index++] = c; // Store in the buffer
                printchar2(c, 0x07, x++, y); // Print the character to the screen
                if (x >= VGA_WIDTH) { // Move to the next line if we exceed the width
                    x = 0;
                    y++;
                    if (y >= VGA_HEIGHT) {
                        y = 5; // Reset to the top if we exceed the height
                    }
                }
            }
        }

        // Reset scan_code for the next iteration
        scan_code = 0;
    }
}


//error handling


// Function to display an error message when the kernel fails to load correctly
void load_error() {
    clear_screen();
    print_string("ERROR: An error occurred while loading the kernel correctly.", 0x1F00, 0, 0);
    print_string("Please check your kernel code and try again.", 0x1F00, 1, 0);
}

// Function to display an error message when the kernel fails to load
void kernel_error() {
    clear_screen();
    print_string("ERROR: An error occurred while loading the kernel.", 0x1F00, 0, 0);
    print_string("Please check your kernel code and try again.", 0x1F00, 1, 0);
}

// Function to display an error message when the kernel crashes
void kernel_crash() {
    clear_screen();
    print_string("ERROR: The kernel has crashed.", 0x1F00, 0, 0);
    print_string("Please check your kernel code for any issues.", 0x1F00, 1, 0);
}

// Function to manage errors based on the error code
void error_manage(int error_code) {
    switch (error_code) {
        case 0x01:
            load_error();
            break;
        case 0x02:
            kernel_error();
            break;
        case 0x03:
            kernel_crash();
            break;
        default:
            clear_screen();
            print_string("UNKNOWN ERROR: An unknown error has occurred.", 0x1F00, 0, 0);
            print_string("Please check your kernel code and try again.", 0x1F00, 1, 0);
            break;
    }
}



//taskbar

void print_char2(char c, unsigned short color, int x, int y) {
    // Ensure background is dark blue (1) and keep the foreground color
    unsigned short final_color = (color & 0x0F00) | 0x2F00; // Dark blue background

    // Calculate the position in the VGA memory
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
    }
}

// Function to print a string to the VGA text buffer
void print_string2(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0; // Reset x to the start of the line
            y++; // Move to the next line
            if (y >= VGA_HEIGHT) {
                y = 0; // Reset to the top if we exceed the height
            }
        } else {
            print_char2(*str, color, x, y); // Print the character
            x++; // Move to the next column
            if (x >= VGA_WIDTH) {
                x = 0; // Reset x to the start of the next line
                y++; // Move to the next line
                if (y >= VGA_HEIGHT) {
                    y = 0; // Reset to the top if we exceed the height
                }
            }
        }
        str++; // Move to the next character in the string
    }
}


//create a start button
void start_button(){
    print_string2("|Start>", 0x1F00, 0, 23);
}

// Function to get the key code (simplified example - requires adaptation)
int get_key_code() {
    while (!(inb(0x64) & 1)); // Wait for keyboard interrupt
    return inb(KEYBOARD_DATA_PORT); // Read key code from keyboard data port
}
// Function to handle key press
void handle_key_press() {
    int key_code = get_key_code();
    if (key_code == KEY_RIGHT_SHIFT) {
        start_window();
    } 
}

int get_key_codet() {
    while (!(inb(0x64) & 1)); // Wait for keyboard interrupt
    return inb(KEYBOARD_DATA_PORT); // Read key code from keyboard data port
}
// Function to handle key press
void handle_key_presst() {
    int key_code = get_key_codet();
    if (key_code == VK_RCONTROL) {
        task_manager();
    } 
}


void print_charg(char c, unsigned short color, int x, int y) {
    // Ensure background is dark blue (1) and keep the foreground color
    unsigned short final_color = (color & 0x0000) | 0x8000; // Dark blue background

    // Calculate the position in the VGA memory
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
    }
}

// Function to print a string to the VGA text buffer
void print_stringg(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0; // Reset x to the start of the line
            y++; // Move to the next line
            if (y >= VGA_HEIGHT) {
                y = 0; // Reset to the top if we exceed the height
            }
        } else {
            print_charg(*str, color, x, y); // Print the character
            x++; // Move to the next column
            if (x >= VGA_WIDTH) {
                x = 0; // Reset x to the start of the next line
                y++; // Move to the next line
                if (y >= VGA_HEIGHT) {
                    y = 0; // Reset to the top if we exceed the height
                }
            }
        }
        str++; // Move to the next character in the string
    }
}

void print_charb(char c, unsigned short color, int x, int y) {
    // Ensure background is dark blue (1) and keep the foreground color
    unsigned short final_color = (color & 0x0000) | 0x1000; // Dark blue background

    // Calculate the position in the VGA memory
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
    }
}

// Function to print a string to the VGA text buffer
void print_stringb(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0; // Reset x to the start of the line
            y++; // Move to the next line
            if (y >= VGA_HEIGHT) {
                y = 0; // Reset to the top if we exceed the height
            }
        } else {
            print_charb(*str, color, x, y); // Print the character
            x++; // Move to the next column
            if (x >= VGA_WIDTH) {
                x = 0; // Reset x to the start of the next line
                y++; // Move to the next line
                if (y >= VGA_HEIGHT) {
                    y = 0; // Reset to the top if we exceed the height
                }
            }
        }
        str++; // Move to the next character in the string
    }
}

void render_video(){
    return 0;
}

void Brunos_media_player(){
    clear_screen();
    print_stringb("Brunos Media Player/===================================================== -|[]|X" , 0x1F00, 0 , 0);
}

//start window
void start_window(){
    print_stringb("|       Log off >     | Shut down > |", 0x1F00, 0, 21);
    print_stringg("|                     |Run >        |", 0x1F00, 0, 20);
    print_stringg("|                     |Help >       |", 0x1F00, 0, 19);
    print_stringg("|                     |Search >     |", 0x1F00, 0, 18);
    print_stringg("|                     |_____________|", 0x1F00, 0, 17);
    print_stringg("|                     |Drives >     |", 0x1F00, 0, 16);
    print_stringg("|                     |Control panel|", 0x1F00, 0, 15);
    print_stringg("|                     |_____________|", 0x1F00, 0, 14);
    print_stringg("|                     |My computer >|", 0x1F00, 0, 13);
    print_stringg("|                     |Music >      |", 0x1F00, 0, 12);
    print_stringg("|File explorer >      |Pictures >   |", 0x1F00, 0, 11);
    print_stringg("|BrunOS browser >     |Documents >  |", 0x1F00, 0, 10);
    print_stringb("|===================================|", 0x1F00, 0, 9);
    print_stringb("|                                   |", 0x1F00, 0, 8);
    print_stringb("|             administrator         |", 0x1F00, 0, 7);
    print_stringb("|                                   |", 0x1F00, 0, 6);
    print_stringb("|                                   |", 0x1F00, 0, 5);
    print_stringb(" =================================== ", 0x1F00, 0, 4);
}

//handle input for all the selecitons in the start bar
void handle_start_inputbar(){
    return 0;
}

//start bar selection apps

//runapp
void runapp(){
    print_string("============= -|[]|X", 0x1F00, 0, 21);
    print_string("run:               |", 0x1F00, 0, 20);
    print_string("====================", 0x1F00, 0, 19);
    return 0;
}

//helpapp
void helpapp(){
    print_string("============= -|[]|X", 0x1F00, 0, 0);
    print_string("help: Read readme  |", 0x1F00, 0, 1);
    print_string("for help.          |", 0x1F00, 0, 2);
    print_string("====================", 0x1F00, 0, 3);
    return 0;
}

//searchapp
void searchapp(){
    print_string("============= -|[]|X", 0x1F00, 0, 0);
    print_string("search:            |", 0x1F00, 0, 1);
    print_string("====================", 0x1F00, 0, 2);
    return 0;
}

//settingsapp
void settingsapp2(){
    settingsapp();
    return 0;
}

//documentsapp
void documentsapp(){
    explorerbin();
    return 0;
}

//programsapp
void programsapp(){
    print_string("=============================== -|[]|X", 0x1F00, 0, 0);
    print_string("programs:                            |", 0x1F00, 0, 1);
    print_string("|                                    |", 0x1F00, 0, 2);
    print_string("|                                    |", 0x1F00, 0, 3);
    print_string("|                                    |", 0x1F00, 0, 4);
    print_string("|                                    |", 0x1F00, 0, 5);
    print_string("|                                    |", 0x1F00, 0, 6);
    print_string("|                                    |", 0x1F00, 0, 7);
    print_string("|                                    |", 0x1F00, 0, 8);
    print_string("|                                    |", 0x1F00, 0, 9);
    print_string("|                                    |", 0x1F00, 0, 10);
    print_string("|                                    |", 0x1F00, 0, 11);
    print_string("|                                    |", 0x1F00, 0, 12);
    print_string("|                                    |", 0x1F00, 0, 13);
    print_string("|                                    |", 0x1F00, 0, 14);
    print_string("|                                    |", 0x1F00, 0, 15);
    print_string("|                                    |", 0x1F00, 0, 16);
    print_string("======================================", 0x1F00, 0, 17);
    return 0;
}

//updateapp
void updateapp(){
    updates();
    return 0;
}

//shutdownapp
void shutdownapp(){
    clear_screen();
    print_string("                             =========================================\n", 0x1F00, 0, 0);
    print_string("                             |                BrunOS 1.01            |\n", 0x1F00, 0, 1);
    print_string("                             |                                       |\n", 0x1F00, 0, 2);
    print_string("                             |      GUI Kernel version[NT]           |\n", 0x1F00, 0, 3);
    print_string("                             |                                       |\n", 0x1F00, 0, 4);
    print_string("                             |                                       |\n", 0x1F00, 0, 5);
    print_string("                             |                                       |\n", 0x1F00, 0, 6);
    print_string("                             =========================================\n", 0x1F00, 0, 7);
    print_string("                                         Loading files...                ", 0x1F00, 0, 8);

    delay(999);

    
    print_string("", 0x07, 0, 0); // Print nothing to clear the screen
    print_string("", 0x07, 0, 1); // Print nothing to clear the screen
    print_string("", 0x07, 0, 2); // Print nothing to clear the screen
    print_string("", 0x07, 0, 3); // Print nothing to clear the screen
    print_string("", 0x07, 0, 4); // Print nothing to clear the screen
    print_string("", 0x07, 0, 5); // Print nothing to clear the screen
    print_string("                         ======================================", 0x07, 0, 6);
    print_string("                         |You can now turn off your computer. |", 0x0004, 0, 7); // Print exit message
    print_string("                         ======================================", 0x07, 0, 8);
    // Shut down the machine
    __asm__ volatile ("cli; hlt");
    return 0;
}


//time
int get_time() {
    // ... (Your code to read the time from the RTC) ...
    // Example - Assuming RTC returns time in BCD format
    int seconds = (inb(RTC_PORT + 0) & 0x0F) + 10 * ((inb(RTC_PORT + 0) >> 4) & 0x0F);
    int minutes = (inb(RTC_PORT + 1) & 0x0F) + 10 * ((inb(RTC_PORT + 1) >> 4) & 0x0F);
    int hours = (inb(RTC_PORT + 2) & 0x0F) + 10 * ((inb(RTC_PORT + 2) >> 4) & 0x0F);
    // ... (Code to handle 12/24 hour format if needed) ...
    return seconds * 1000 + minutes * 60 * 1000 + hours * 3600 * 1000; // Time in milliseconds
}
// Function to print the time
void print_local_time() {
    int time_ms = get_time(); // Get the time in milliseconds
    // Convert to seconds
    int seconds = time_ms / 1000;
    // Convert to hours, minutes, seconds
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int remaining_seconds = seconds % 60;
    // Print the time
    print_string("Current Time: ", 0x1F00, 0, 23);
    print_char(hours / 10 + '0', 0x1F00, 14, 23); 
    print_char(hours % 10 + '0', 0x1F00, 15, 23);
    print_char(':', 0x1F00, 16, 23);
    print_char(minutes / 10 + '0', 0x1F00, 17, 23); 
    print_char(minutes % 10 + '0', 0x1F00, 18, 23);
    print_char(':', 0x1F00, 19, 23);
    print_char(remaining_seconds / 10 + '0', 0x1F00, 20, 23); 
    print_char(remaining_seconds % 10 + '0', 0x1F00, 21, 23);
}


//seperate kernel apps

//include all seperate apps
#include "settings.h"
#include "browser.h"
#include "fileexplorer.h"

//unix-like shell
//unix-like shell
void shell() {
    
    char input_buffer[80]; // Buffer to hold the current line of input
    int buffer_index = 0; // Current index in the input buffer
    int x = 0, y = 24; // Current cursor position for output
    unsigned char scan_code = 0; // Keyboard scan code

    // Clear the input buffer
    for (int i = 0; i < 80; i++) {
        input_buffer[i] = 0;
    }

    // Print the prompt
    print_string("> ", 0x07, 0, y);

    // Main input loop
    while (1) {
        // Wait for user input
        while (scan_code == 0) {
            if (port_read(0x64) & 0x1) {
                scan_code = port_read(0x60);
            }
        }

        char c = scan_code_to_ascii(scan_code);

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
                print_string("> ", 0x07, 0, y); // Print the prompt again
            } else if (buffer_index < 80) { // Check buffer size
                input_buffer[buffer_index++] = c; // Store in the buffer
                print_char(c, 0x07, x++, y); // Print the character to the screen
                if (x >= VGA_WIDTH) { // Move to the next line if we exceed the width
                    x = 0;
                    y++;
                    if (y >= VGA_HEIGHT) {
                        y = 0; // Reset to the top if we exceed the height
                    }
                    print_string("> ", 0x07, 3, y); // Print the prompt again
                }
            }
        }

        // Reset scan_code for the next iteration
        scan_code = 0;
    }
}



//mouse
void mouse() {
    //mouse
}

//Control panel
void controlpanel(){
    print_stringb("Control panel/================== -|[]|X", 0x1F00, 0 , 0);
    print_stringb("|   @ System >                        |", 0x1F00, 0 , 1);
    print_stringb("|                                     |", 0x1F00, 0 , 2);
    print_stringb("|   & User comfiguration >            |", 0x1F00, 0 , 3);
    print_stringb("|                                     |", 0x1F00, 0 , 4);
    print_stringb("|   * Application manager             |", 0x1F00, 0 , 5);
    print_stringb("|                                     |", 0x1F00, 0 , 6);
    print_stringb("|                                     |", 0x1F00, 0 , 7);
    print_stringb("|                                     |", 0x1F00, 0 , 8);
    print_stringb("=======================================", 0x1F00, 0 , 9);
}

//browser
void browser(){
    browseri();
}

// settings
void settingsapp(){
    set();
}


//menu void

//file
void filemenu(){
    print_string("|Files:     |", 0x1F00, 0, 0);
    print_string("|exit       |", 0x1F00, 0, 1);
    print_string("|help       |", 0x1F00, 0, 2);
    print_string("|search     |", 0x1F00, 0, 3);
    print_string("|new file   |", 0x1F00, 0, 4);
    print_string("=============", 0x1F00, 0, 5);
}

//help
void helpmenu(){
    print_string("|Help:      |", 0x1F00, 0, 0);
    print_string("|for help re|", 0x1F00, 0, 1);
    print_string("|adme       |", 0x1F00, 0, 2);
    print_string("|search     |", 0x1F00, 0, 3);
    print_string("=============", 0x1F00, 0, 4);
}


//kernelOS applications from selection bar


//readme
void readme(){
    clear_screen();
    print_stringb("Notepad/================================================================= -|[]|X" , 0x1F00, 0, 0);
    delay(500);
    print_string("|                                 NotEditable                                  |", 0x1F00, 0, 1);
    print_string("", 0x1F00, 0, 2);
    print_string("This is a An gui operating system! It is a codename OS and a better kernel will be out soon! You can also read the README for more help." , 0x1F00, 0, 3);
}

//calculator
void calculatorbin(){
    clear_screen();
    print_stringb("Calculator/============================================================== -|[]|X" , 0x1F00, 0, 0);
    delay(500);
    char op;
    int num1, num2 , sum;
    print_string("", 0x1F00, 0, 1);
    print_string("Welcome to the calculator app", 0x1F00, 0, 2);
    print_string("Enter your calculation:", 0x1F00, 0, 3);
}

//notepad
void notepadbin(){
    clear_screen();
    print_stringb("Notepad/================================================================= -|[]|X" , 0x1F00, 0, 0);
    delay(500);
    print_string("", 0x1F00, 0, 1);
    print_string("Welcome to the notepad app", 0x1F00, 0, 2);
    print_string("Enter your text:", 0x1F00, 0, 3);
    shell();
}

//explorer
void explorerbin(){
    filemanage();
}

//write
void writebin(){
    clear_screen();
    print_stringb("Write/=================================================================== -|[]|X" , 0x1F00, 0, 0);
    delay(500);
    print_string("", 0x1F00, 0, 1);
    print_string("Write:", 0x1F00, 0, 2);
}


//shell
void shellbin(){
    clear_screen();
    print_stringb("bDOS]/=================================================================== -|[]|X" , 0x1F00, 0, 0);
    delay(500);
    shell();
}

//updates
void updates(){
    clear_screen();
    print_stringb("Updates/================================================================= -|[]|X" , 0x1F00, 0, 0);
    delay(500);
    print_string("", 0x1F00, 0, 1);
    print_string("UPDATES: No updates found!", 0x1F00, 0, 2);
}

//Delay
void delay(int seconds) {
    volatile long int count;
        for (int i = 0; i < seconds * 1000000; i++) {
            count++; // Busy wait
        }
    }

//version window
void version_window(){
    print_stringb("============================= -|[]|X", 0x1F00, 3, 0);
    print_string("|          Version: 1.0.0          |", 0x1F00, 3, 1);
    print_string("|          Build: 2023-08-01       |", 0x1F00, 3, 2);
    print_string("|                                  |", 0x1F00, 3, 3);
    print_string("|                                  |", 0x1F00, 3, 4);
    print_string("|                                  |", 0x1F00, 3, 5);
    print_string("|                                  |", 0x1F00, 3, 6);
    print_string("|                                  |", 0x1F00, 3, 7);
    print_string("|                                  |", 0x1F00, 3, 8);
    print_string("====================================", 0x1F00, 3, 9);
}

//control + alt + delete

//define key



void shutdownc(){
    unsigned short dark_blue_background = 0x0000; // White on dark blue

    // Fill the screen with spaces having a dark blue background
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
    }
    clear_screen();
    print_stringb("================================================================================" , 0x1F00, 0, 0);
    print_stringb("                                                                                 " , 0x1F00, 0, 1);
    print_stringb("                                                                                 " , 0x1F00, 0, 2);
    print_stringb("                                                                                 " , 0x1F00, 0, 3);
    print_stringb("                                                                                 " , 0x1F00, 0, 4);
    print_stringb("================================================================================" , 0x1F00, 0, 5);

    print_string("                                BrunOS is shutting down...                                ", 0x1F00, 0, 11);

    print_stringb("================================================================================" , 0x1F00, 0, 24);
    print_stringb("                                                                                 " , 0x1F00, 0, 23);
    print_stringb("                                                                                 " , 0x1F00, 0, 22);
    print_stringb("                                                                                 " , 0x1F00, 0, 21);
    print_stringb("                                                                                 " , 0x1F00, 0, 20);
    print_stringb("================================================================================" , 0x1F00, 0, 19);
    delay(444);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
    }
    print_string("", 0x07, 0, 0); // Print nothing to clear the screen
    print_string("", 0x07, 0, 1); // Print nothing to clear the screen
    print_string("", 0x07, 0, 2); // Print nothing to clear the screen
    print_string("", 0x07, 0, 3); // Print nothing to clear the screen
    print_string("", 0x07, 0, 4); // Print nothing to clear the screen
    print_string("", 0x07, 0, 5); // Print nothing to clear the screen
    print_string("                         ======================================", 0x07, 0, 6);
    print_string("                         |You can now turn off your computer. |", 0x0004, 0, 7); // Print exit message
    print_string("                         ======================================", 0x07, 0, 8);
    // Shut down the machine
    __asm__ volatile ("cli; hlt");
}

void rebootc(){
    unsigned short dark_blue_background = 0x0000; // White on dark blue

    // Fill the screen with spaces having a dark blue background
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = (unsigned short)(' ' | dark_blue_background); // Fill the screen with spaces
    }
    clear_screen();
    print_stringb("================================================================================" , 0x1F00, 0, 0);
    print_stringb("                                                                                 " , 0x1F00, 0, 1);
    print_stringb("                                                                                 " , 0x1F00, 0, 2);
    print_stringb("                                                                                 " , 0x1F00, 0, 3);
    print_stringb("                                                                                 " , 0x1F00, 0, 4);
    print_stringb("================================================================================" , 0x1F00, 0, 5);

    print_string("                                BrunOS is rebooting...                                ", 0x1F00, 0, 11);

    print_stringb("================================================================================" , 0x1F00, 0, 24);
    print_stringb("                                                                                 " , 0x1F00, 0, 23);
    print_stringb("                                                                                 " , 0x1F00, 0, 22);
    print_stringb("                                                                                 " , 0x1F00, 0, 21);
    print_stringb("                                                                                 " , 0x1F00, 0, 20);
    print_stringb("================================================================================" , 0x1F00, 0, 19);
    delay(444);
    kmain();
}

void log_offc(){
    //log off screen
}



//functions of ctrl + alt + del


//task manager
void task_manager(){
    print_stringb("============================================= -|[]|X", 0x1F00, 0, 0);
    print_string("|Task Manager:     CPU [ %]   Memmory[ %]|Disk[ %] |", 0x1F00, 0, 1);
    print_string("|==================================================|", 0x1F00, 0, 2);
    print_string("|File                                              |", 0x1F00, 0, 3);
    print_string("|procceses:                                        |", 0x1F00, 0, 4);
    print_string("|                                                  |", 0x1F00, 0, 5);
    print_string("|                                                  |", 0x1F00, 0, 6);
    print_string("|                                                  |", 0x1F00, 0, 7);
    print_string("|                                                  |", 0x1F00, 0, 8);
    print_string("|                                                  |", 0x1F00, 0, 9);
    print_string("|                                                  |", 0x1F00, 0, 10);
    print_string("|                                                  |", 0x1F00, 0, 11);
    print_string("|                                                  |", 0x1F00, 0, 12);
    print_string("|                                                  |", 0x1F00, 0, 13);
    print_string("|                                                  |", 0x1F00, 0, 14);
    print_string("|                                                  |", 0x1F00, 0, 15);
    print_string("|                                         _________|", 0x1F00, 0, 16);
    print_string("|                                        |Terminate|", 0x1F00, 0, 17);
    print_string("====================================================", 0x1F00, 0, 18);
    
}

//logo
void os_logo(){
    print_string(" ********               " , 0x1F00, 30, 6);
    print_string("**********    *        *" , 0x1F00, 30, 7);
    print_string("**********    **********" , 0x1F00, 30, 8);
    print_string("**********    **********" , 0x1F00, 30, 9);
    print_string("*        *    **********" , 0x1F00, 30, 10);
    print_string("               ******** " , 0x1F00, 30, 11);
    print_string(" ********               " , 0x1F00, 30, 12);
    print_string("**********    *        *" , 0x1F00, 30, 13);
    print_string("**********    **********" , 0x1F00, 30, 14);
    print_string("**********    **********" , 0x1F00, 30, 15);
    print_string("*        *    **********" , 0x1F00, 30, 16);
    print_string("               ******** " , 0x1F00, 30, 17);
}


void print_charbl(char c, unsigned short color, int x, int y) {
    // Ensure background is dark blue (1) and keep the foreground color
    unsigned short final_color = (color & 0x0000) | 0x0800; // Dark blue background

    // Calculate the position in the VGA memory
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
    }
}

// Function to print a string to the VGA text buffer
void print_stringbl(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0; // Reset x to the start of the line
            y++; // Move to the next line
            if (y >= VGA_HEIGHT) {
                y = 0; // Reset to the top if we exceed the height
            }
        } else {
            print_charbl(*str, color, x, y); // Print the character
            x++; // Move to the next column
            if (x >= VGA_WIDTH) {
                x = 0; // Reset x to the start of the next line
                y++; // Move to the next line
                if (y >= VGA_HEIGHT) {
                    y = 0; // Reset to the top if we exceed the height
                }
            }
        }
        str++; // Move to the next character in the string
    }
}

void print_charbu(char c, unsigned short color, int x, int y) {
    // Ensure background is dark blue (1) and keep the foreground color
    unsigned short final_color = (color & 0x0000) | 0x0100; // Dark blue background

    // Calculate the position in the VGA memory
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_MEMORY[x + y * VGA_WIDTH] = (unsigned short)(c | final_color);
    }
}

// Function to print a string to the VGA text buffer
void print_stringbu(const char *str, unsigned short color, int x, int y) {
    while (*str) {
        if (*str == '\n') {
            x = 0; // Reset x to the start of the line
            y++; // Move to the next line
            if (y >= VGA_HEIGHT) {
                y = 0; // Reset to the top if we exceed the height
            }
        } else {
            print_charbu(*str, color, x, y); // Print the character
            x++; // Move to the next column
            if (x >= VGA_WIDTH) {
                x = 0; // Reset x to the start of the next line
                y++; // Move to the next line
                if (y >= VGA_HEIGHT) {
                    y = 0; // Reset to the top if we exceed the height
                }
            }
        }
        str++; // Move to the next character in the string
    }
}


//boot animation
void boot_animation(){
    print_stringbl("===========================", 0x1F00, 28, 18);
    print_stringbl("|                         |", 0x1F00, 28, 19);
    print_stringbl("===========================", 0x1F00, 28, 20);
    for (int i = 0; i < 4; i++){
        print_stringbu("|||", 0x1F00, 30 , 19);
        delay(111);
        print_stringbu("   ", 0x1F00, 30 , 19);
        print_stringbu("   |||", 0x1F00, 33 , 19);
        delay(111);
        print_stringbu("     |||", 0x1F00, 36 , 19);
        delay(111);
        print_stringbu("       |||", 0x1F00, 39 , 19);
        delay(111);
        print_stringbu("          |||", 0x1F00, 40 , 19);
        print_stringbu("             ", 0x1F00, 41 , 19);
    }
}



// Kernel entry point
void kmain(void) {
    delay(444);
    clear_screen(); // Clear the screen at startup
    //startup screen
    print_string("", 0x07, 0, 0);
    print_string("", 0x07, 0, 1);
    print_string("", 0x07, 0, 2);
    print_string("", 0x07, 0, 3);
    print_string("", 0x07, 0, 4);
    print_string("", 0x07, 0, 5);
    print_string("", 0x07, 0, 6);
    print_string("", 0x07, 0, 7);
    print_string("                      =========================================\n", 0x1F00, 0, 8);
    print_string("                      |                BrunOS 1.01            |\n", 0x1F00, 0, 9);
    print_string("                      |                                       |\n", 0x1F00, 0, 10);
    print_string("                      |      GUI Kernel version[NT]           |\n", 0x1F00, 0, 11);
    print_string("                      |                                       |\n", 0x1F00, 0, 12);
    print_string("                      |                                       |\n", 0x1F00, 0, 13);
    print_string("                      |                                       |\n", 0x1F00, 0, 14);
    print_string("                      =========================================\n", 0x1F00, 0, 15);
    print_string("                                 Loading files...                ", 0x1F00, 0, 16);
    boot_animation();
    delay(999);
    clear_screen();
    print_stringb("================================================================================" , 0x1F00, 0, 0);
    print_stringb("                                                                                 " , 0x1F00, 0, 1);
    print_stringb("                                                                                 " , 0x1F00, 0, 2);
    print_stringb("                                                                                 " , 0x1F00, 0, 3);
    print_stringb("                                                                                 " , 0x1F00, 0, 4);
    print_stringb("================================================================================" , 0x1F00, 0, 5);

    print_string("                                Welcome                                ", 0x1F00, 0, 11);

    print_stringb("================================================================================" , 0x1F00, 0, 24);
    print_stringb("                                                                                 " , 0x1F00, 0, 23);
    print_stringb("                                                                                 " , 0x1F00, 0, 22);
    print_stringb("                                                                                 " , 0x1F00, 0, 21);
    print_stringb("                                                                                 " , 0x1F00, 0, 20);
    print_stringb("================================================================================" , 0x1F00, 0, 19);
    delay(444);
    
    // Show the menu and selections
    print_menu_and_selections();
    // Enter an infinite loop to handle input

    // Initialize devices
    if (!initialize_devices()) {
        error_manage(0x01); // Kernel error
        return; // Exit if device initialization fails
    }

    // Initialize processes
    if (!initialize_processes()) {
        error_manage(0x02); // Kernel crash
        return; // Exit if process initialization fails
    }


    // If everything is successful, proceed with the main loop or scheduler
    while (1) {
        handle_input();
        os_logo();
        //desktop icon

        print_string("BrunOS build: 2024-08-22", 0x07, 50, 21);
        
        print_string("________________________________________________________________________________", 0x1F00, 0, 22);
        print_stringb("================================================================================", 0x1F00, 0, 24);

        print_stringb("                                                                   |2024 10:09 |", 0x1F00, 0, 23);
        
        start_button();
        
        handle_key_press();
    }
}

//initializations


int initialize_devices() {
    // Device initialization logic
    // Return 1 for success, 0 for failure
    return 1; // Placeholder
}

int initialize_processes() {
    // Process initialization logic
    // Return 1 for success, 0 for failure
    return 1; // Placeholder
}