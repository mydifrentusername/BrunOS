typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

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

gdt_entry_t gdt_entries[GDT_ENTRY_COUNT];
gdt_ptr_t gdt_ptr;

void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt_entries[num].access = access;
}

void gdt_init() {
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRY_COUNT) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    asm volatile ("lgdt %0" : : "m"(gdt_ptr));

    asm volatile (
        "mov ax, %1\n"
        "mov ds, ax\n"
        "mov es, ax\n"
        "mov fs, ax\n"
        "mov gs, ax\n"
        "mov ss, ax\n"
        "jmp %2\n"
        : 
        : "i"(GDT_KERNEL_DATA), "i"(GDT_KERNEL_CODE)
        : "memory"
    );
}