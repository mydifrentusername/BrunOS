#ifndef MEMORY_SEGMENTATION_H
#define MEMORY_SEGMENTATION_H

#include "virtualmemory.h"

void init_segment_table();
void create_segment(int segment_number, unsigned int base, unsigned int limit);
unsigned char access_segmented_memory(int segment_number, unsigned int offset);
void load_page(int page_number);
void output_char(char c);
void output_string(const char *str);
void int_to_string(int num, char *buffer);

#endif