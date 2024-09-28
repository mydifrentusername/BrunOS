//segmentation

#include "virtualmemory.h"


void init_segment_table() {
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        segment_table[i].base = 0;
        segment_table[i].limit = 0;
    }
}


void create_segment(int segment_number, unsigned int base, unsigned int limit) {
    if (segment_number < 0 || segment_number >= MAX_SEGMENTS) {
        return;
    }

    if (limit <= 0 || base + limit > FRAME_COUNT * PAGE_SIZE) {
        return;
    }

    segment_table[segment_number].base = base;
    segment_table[segment_number].limit = limit;
}


unsigned char access_segmented_memory(int segment_number, unsigned int offset) {
    if (segment_number < 0 || segment_number >= MAX_SEGMENTS) {
        return 0;
    }

    SegmentDescriptor seg_desc = segment_table[segment_number];

    if (offset >= seg_desc.limit) {
        return 0;
    }

    unsigned int physical_address = seg_desc.base + offset;

    return physical_memory[physical_address];
}


void load_page(int page_number) {
    if (page_number < 0 || page_number >= NUM_PAGES) {
        return;
    }

    page_table[page_number].frame_number = page_number;
    page_table[page_number].valid = 1;

}


void output_char(char c) {
    // Implementation for outputting a character goes here
}


void output_string(const char *str) {
    while (*str) {
        output_char(*str++);
    }
}


void int_to_string(int num, char *buffer) {
    int i = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    buffer[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}