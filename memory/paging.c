//paging



void init_page_table() {
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i].frame_number = -1;
        page_table[i].valid = 0;
    }
}


void load_page(int page_number) {
    if (page_number < 0 || page_number >= NUM_PAGES) {
        return;
    }

    page_table[page_number].frame_number = page_number;
    page_table[page_number].valid = 1;

}


unsigned char access_memory(int virtual_address) {
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    if (page_number < 0 || page_number >= NUM_PAGES) {
        return 0;
    }

    if (!page_table[page_number].valid) {
        load_page(page_number);
    }

    int physical_address = (page_table[page_number].frame_number * PAGE_SIZE) + offset;
    return physical_memory[physical_address];
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