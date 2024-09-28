#ifndef HARDWARE_CONFIGURATION_CPUCONFIG_H
#define HARDWARE_CONFIGURATION_CPUCONFIG_H

#include "../../drivers/vga.h"
#include "../../fat/fat32.h"

#define VGA_WIDTH 320
#define VGA_HEIGHT 200 

void cpuid(unsigned int info[4], unsigned int leaf);
void itoa(unsigned int num, char* str, unsigned int base);
void print_cpu_info();


#endif