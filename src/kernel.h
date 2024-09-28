#ifndef SRC_KERNEL_H
#define SRC_KERNEL_H

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

#include "settings.h"
#include "browser.h"
#include "fileexplorer.h"



void* allocate_memory(int size);
void free_memory(int size);

#define KEYBOARD_PORT 0x60

void print_char(char c, unsigned short color, int x, int y);
void print_string(char* str, unsigned short color, int x, int y);
void clearscreen();

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SPACE 0x39 // Scan code for the space bar

void print_error(const char *message);
void keyboard_input_handler();
void print_menu_and_selections();
void handle_input();
void printchar2(char c, unsigned short color, int x, int y);
unsigned char port_read(unsigned short port);
void type_anything();
void load_error();
void kernel_error();
void kernel_crash();
void error_manage(int error_code);
void print_char2(char c, unsigned short color, int x, int y);
void print_string2(const char *str, unsigned short color, int x, int y);
void start_button();
int get_key_code();
void handle_key_press();
int get_key_codet();
void handle_key_presst();
void print_charg(char c, unsigned short color, int x, int y);
void print_stringg(const char *str, unsigned short color, int x, int y);
void print_charb(char c, unsigned short color, int x, int y);
void print_stringb(const char *str, unsigned short color, int x, int y);
int get_time();
void shell();
void mouse();
void shutdownc();
void rebootc();
void log_offc();
void print_charbl(char c, unsigned short color, int x, int y);
void print_stringbl(const char *str, unsigned short color, int x, int y);
void print_charbu(char c, unsigned short color, int x, int y);
void print_stringbu(const char *str, unsigned short color, int x, int y);
void kmain(void);

#endif