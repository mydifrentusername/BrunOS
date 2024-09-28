//virtual memory managment

#define PAGE_SIZE 256 // Size of each page in bytes
#define NUM_PAGES 256 // Total number of pages
#define FRAME_COUNT 256 // Total number of frames in physical memory

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

unsigned char physical_memory[FRAME_COUNT * PAGE_SIZE];

PageTableEntry page_table[NUM_PAGES];


void init_page_table() {
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i].valid = 0;
    }
}


void load_page(int page_number) {
    if (page_table[page_number].valid == 0) {
        for (int i = 0; i < PAGE_SIZE; i++) {
            physical_memory[page_number * PAGE_SIZE + i] = 0;
        }
        page_table[page_number].frame_number = page_number;
        page_table[page_number].valid = 1;
    }
}


unsigned char access_memory(int virtual_address) {
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    if (page_table[page_number].valid == 0) {
        load_page(page_number);
    }

    return physical_memory[page_table[page_number].frame_number * PAGE_SIZE + offset];
}


void output_char(char c) {
    volatile char* uart = (char*)0x10000000;
    *uart = c;
}


void output_string(const char* str) {
    while (*str) {
        output_char(*str++);
    }
}


void int_to_string(int num, char* buffer) {
    char* ptr = buffer;
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    if (num < 0) {
        *ptr++ = '-';
        num = -num;
    }

    int rev_num = 0;
    while (num > 0) {
        rev_num = rev_num * 10 + num % 10;
        num /= 10;
    }

    while (rev_num > 0) {
        *ptr++ = '0' + rev_num % 10;
        rev_num /= 10;
    }

    *ptr = '\0';

    if (*(buffer) == '-') {
        for (int i = 1; i < ptr - buffer; i++) {
            buffer[i] = buffer[ptr - buffer - i];
            buffer[ptr - buffer - i] = buffer[i];
        }
        ptr += 1;
    } else {
        for (int i = 0; i < (ptr - buffer)/2; i++) {
            char temp = buffer[i];
            buffer[i] = buffer[ptr - buffer - i - 1];
            buffer[ptr - buffer - i - 1] = temp;
        }
    }
}
