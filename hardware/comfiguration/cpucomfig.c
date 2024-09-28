//cpu comfiguration

#include "../../drivers/vga.h"
#include "../../fat/fat32.h"

#define VGA_WIDTH 1024
#define VGA_HEIGHT 768

// Function to execute CPUID instruction
void cpuid(unsigned int info[4], unsigned int leaf) {
    __asm__ volatile (
        "cpuid"
        : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3])
        : "a"(leaf)
    );
}

// Function to convert an integer to a string
void itoa(unsigned int num, char* str, unsigned int base) {
    unsigned int i = 0;
    unsigned int isNegative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers only for base 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        unsigned int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }

    // If the number is negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    unsigned int start = 0;
    unsigned int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Function to print CPU information
void print_cpu_info() {
    unsigned int info[4];

    // Get vendor string
    cpuid(info, 0);
    char vendor[13];
    *((unsigned int*)&vendor[0]) = info[1]; // EBX
    *((unsigned int*)&vendor[4]) = info[3]; // EDX
    *((unsigned int*)&vendor[8]) = info[2]; // ECX
    vendor[12] = '\0'; // Null-terminate the string

    // Print vendor
    print_string("CPU Vendor: ", 0x0F, 0, 0); // White text
    print_string(vendor, 0x0F, 0, 1); // Print vendor on the next line

    // Get CPU brand and other information
    cpuid(info, 1);

    char buffer[50]; // Buffer to hold the output string

    // Print CPU Family
    itoa((info[0] >> 8) & 0xF, buffer, 10);
    print_string("CPU Family: ", 0x0F, 0, 2);
    print_string(buffer, 0x0F, 0, 3);

    // Print CPU Model
    itoa((info[0] >> 4) & 0xF, buffer, 10);
    print_string("CPU Model: ", 0x0F, 0, 4);
    print_string(buffer, 0x0F, 0, 5);

    // Print CPU Stepping
    itoa(info[0] & 0xF, buffer, 10);
    print_string("CPU Stepping: ", 0x0F, 0, 6);
    print_string(buffer, 0x0F, 0, 7);

    // Print Number of Logical Processors
    itoa((info[1] >> 16) & 0xFF, buffer, 10);
    print_string("Number of Logical Processors: ", 0x0F, 0, 8);
    print_string(buffer, 0x0F, 0, 9);
}
