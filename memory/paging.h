#ifndef MEMORY_PAGING_H
#define MEMORY_PAGING_H

void init_page_table();
void load_page(int page_number);
unsigned char access_memory(int virtual_address);
void output_char(char c);
void output_string(const char *str);
void int_to_string(int num, char *buffer);

#endif