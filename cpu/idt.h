#ifndef CPU_IDT_H
#define CPU_IDT_H

#ifndef __UINT32_T_DEFINED__
typedef unsigned long uint32_t;
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

#define IDT_ENTRY_COUNT 256

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_set_entry(int num, uint32_t base, uint16_t selector, uint8_t type_attr);
void idt_init();

#endif