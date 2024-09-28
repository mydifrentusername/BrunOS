#ifndef MEMORY_VIRTUALMEMORY_H
#define MEMORY_VIRTUALMEMORY_H

#define PAGE_SIZE 256 // Size of each page in bytes
#define NUM_PAGES 256 // Total number of pages
#define FRAME_COUNT 256 // Total number of frames in physical memory

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

unsigned char physical_memory[FRAME_COUNT * PAGE_SIZE];
PageTableEntry page_table[NUM_PAGES];
void init_page_table();
void load_page(int page_number);
unsigned char access_memory(int virtual_address);
void output_char(char c);
void output_string(const char *str);
void int_to_string(int num, char *buffer);

#endif