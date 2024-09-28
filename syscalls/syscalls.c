void write_console(const char *message) {
    volatile char *video_memory = (char *)0xB8000;
    while (*message) {
        *video_memory++ = *message++;
        *video_memory++ = 0x07;
    }
}

void sys_hello() {
    const char *message = "Hello, World!\n";
    write_console(message);
}