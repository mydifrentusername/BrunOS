#ifndef CPU_GDT_H
#define CPU_GDT_H

#ifndef __UINT32_T_DEFINED__

#define __UINT32_T_DEFINED__
#endif

#ifndef __UINT16_T_DEFINED__
typedef unsigned short uint16_t;
#define __UINT16_T_DEFINED__
#endif

#ifndef __UINT8_T_DEFINED__
typedef unsigned char uint8_t;
#define __UINT8_T_DEFINED__
#endif

#define GDT_ENTRY_COUNT 5

#define GDT_NULL 0x00
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE 0x18
#define GDT_USER_DATA 0x20

typedef struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void gdt_init();

#endif