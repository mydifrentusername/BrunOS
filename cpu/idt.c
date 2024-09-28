idt_entry_t idt_entries[IDT_ENTRY_COUNT];
idt_ptr_t idt_ptr;

void idt_set_entry(int num, uint32_t base, uint16_t selector, uint8_t type_attr) {
    idt_entries[num].offset_low = (base & 0xFFFF);
    idt_entries[num].selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].type_attr = type_attr;
    idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init() {
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_ENTRY_COUNT) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    for (int i = 0; i < IDT_ENTRY_COUNT; i++) {
        idt_set_entry(i, 0, 0x08, 0x8E); // Set all entries to a default handler
    }

    asm volatile ("lidt %0" : : "m"(idt_ptr));
}