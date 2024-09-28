#!/bin/bash

cd src

# Remove any existing object files and the kernel executable
rm kasm.o kc.o kernel app.o fileexplorer.o

# Assemble kernel.asm into kasm.o (using NASM)
nasm -f elf32 kernel.asm -o kasm.o

# Compile kernel.c into kc.o (using GCC with i386 architecture)
gcc -m32 -c -fno-stack-protector kernel.c -o kc.o


# compile settings.c into settings.o (using GCC with i386 architecture)
gcc -m32 -c settings.c -o settings.o

# compile browser.c into browser.o (using GCC with i386 architecture)
gcc -m32 -c browser.c -o browser.o

gcc -m32 -c fileexplorer.c -o fileexplorer.o

# Link all object files into the final executable (using LD with i386 architecture)
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o settings.o browser.o fileexplorer.o

# transfer the kernel outside of the src directory
mv kernel ../

cd ..

# Run the kernel using QEMU
qemu-system-i386 -kernel kernel


