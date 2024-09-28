#include "../api/kernelapi.h"
#include "icons.h"

void folder_icon(){
  print_string("__", 0x1F00, 0, 0);
  print_string("║╚══════╗", 0x1F00, 0, 1);
  print_string("║       ║", 0x1F00, 0, 2);
  print_string("╚═══════╝", 0x1F00, 0, 3);
}

void file_icon(){
  print_string("__", 0x1F00, 0, 0);
  print_string("║╚═══╗", 0x1F00, 0, 1);
  print_string("║    ║", 0x1F00, 0, 2);
  print_string("║    ║", 0x1F00, 0, 3);
  print_string("╚════╝", 0x1F00, 0, 4);
}

void computer_icon(){
  print_string(" _____   ", 0x1F00, 0, 0);
  print_string("|     |╔╗", 0x1F00, 0, 1);
  print_string("|_____|║║", 0x1F00, 0, 2);
  print_string("   ║   ╚╝", 0x1F00, 0, 3);
}

void flashdrive_icon(){
  print_string("╔═════╗__", 0x1F00, 0, 0);
  print_string("╚═════╝  ", 0x1F00, 0, 1);
}

void floppydisk_icon(){
  print_string("/══════╗", 0x1F00, 0, 0);
  print_string("║ ____ ║", 0x1F00, 0, 1);
  print_string("║ |  | ║", 0x1F00, 0, 2);
  print_string("╚══════╝", 0x1F00, 0, 3);
}

void cdrom_icon(){
  
}

void harddrive_icon(){
  print_string("╔════╗", 0x1F00, 0, 0);
  print_string("║    ║", 0x1F00, 0, 1);
  print_string("║  * ║", 0x1F00, 0, 2);
  print_string("║    ║", 0x1F00, 0, 3);
  print_string("╚════╝", 0x1F00, 0, 4);
}

void usb_icon(){
  print_string("______╔═════╗__", 0x1F00, 0, 0);
  print_string("      ╚═════╝  ", 0x1F00, 0, 1);
}