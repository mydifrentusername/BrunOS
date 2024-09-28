#include "../api/kernelapi.h"
#include "../fat/fat32.h"

//FILE AND DIRECTORY    --DIAGRAM--


/*
kernel:
api:
  kernelapi.o
cpu:
  gdt.o
  idt.o
  timer.o
drivers:
  internet.o
  keyboard.o
  vga.o
  VGAEchancedgraphicsDriver.o
fat:
  fat32.o
hardware:
  comfiguration:
    bootcomfig.o
    cpucomfig.o
    memorycomfig.o
  usbdrivers:
    usb.o
icons:
  icons.o
memory:
  paging.o
  segmentation.o
  virtualmemory.o
multitasking:
  multitasking.o
oslibr:
  types.o
src:
  browser.o
  fileexplorer.o
  kasm.o
  kc.o
  link.ld
  settings.o
syscalls:
  syscalls.o
  assembly:
    assembly.o
window:
  window.o
*/



void backround_explorer(){
  print_string("explorer/================================================================ - [] X" , 0x1F00, 0, 0);
  print_string("|          |Delete|Create|                                      |Search:       |" , 0x1F00, 0, 1);
  print_string("|          |===================================================================|" , 0x1F00, 0, 2);
  print_string("|          |Files/folders/:                                                    |" , 0x1F00, 0, 3);
  print_string("|          | kernel:                                                           |" , 0x1F00, 0, 4);
  print_string("|          | #  api:                                                    Folder |" , 0x1F00, 0, 5);
  print_string("|          | #  cpu:                                                    Folder |" , 0x1F00, 0, 6);
  print_string("|          | #  drivers:                                                Folder |" , 0x1F00, 0, 7);
  print_string("|          | #  fat:                                                    Folder |" , 0x1F00, 0, 8);
  print_string("|          | #  hardware:                                               Folder |" , 0x1F00, 0, 9);
  print_string("|          | #  icons:                                                  Folder |" , 0x1F00, 0, 10);
  print_string("|          | #  memory:                                                 Folder |" , 0x1F00, 0, 11);
  print_string("|          | #  multitasking:                                           Folder |" , 0x1F00, 0, 12);
  print_string("|          | #  oslibr:                                                 Folder |" , 0x1F00, 0, 13);
  print_string("|          | #  src:                                                    Folder |" , 0x1F00, 0, 14);
  print_string("|          | #  syscalls:                                               Folder |" , 0x1F00, 0, 15);
  print_string("|          | #  window:                                                 Folder |" , 0x1F00, 0, 16);
  print_string("|          |                                                            Folder |" , 0x1F00, 0, 17);
  print_string("|==========                                                                    |" , 0x1F00, 0, 18);
  print_string("|                                                                              |" , 0x1F00, 0, 19);
  print_string("|                                                                              |" , 0x1F00, 0, 20);
  print_string("|                                                                              |" , 0x1F00, 0, 21);
}

void filemanage(){
  backround_explorer();
}