#include "../api/kernelapi.h"


#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096
#define CLUSTER_COUNT 32
#define FAT_ENTRIES (CLUSTER_COUNT + 2)


uint8_t fat_table[FAT_ENTRIES];


bool cluster_used[CLUSTER_COUNT];

void fat32_init(void);

void format_file(void);

void get_volume_info(void);

void unmount_volume(void);

void create_file(const char* filename);

void open_file(const char* filename);

void close_file(int file_descriptor);

void delete_file(const char* filename);

void rename_file(const char* old_name, const char* new_name);

size_t get_file_size(const char* filename);

void get_file_attributes(const char* filename);

void create_directory(const char* dirname);

void open_directory(const char* dirname);

void close_directory(int dir_descriptor);

void delete_directory(const char* dirname);

void list_directory_contents(const char* dirname);

void read_from_file(int file_descriptor, void* buffer, size_t size);

void write_to_file(int file_descriptor, const void* buffer, size_t size);

void seek_in_file(int file_descriptor, size_t offset);

int allocate_cluster(void);

void free_cluster(int cluster_index);

int get_next_cluster(int current_cluster);

void fat32_init(void) {
    print_string("Initializing FAT32 file system...", 0x1F00, 0, 0);
    memset(fat_table, 0x00, sizeof(fat_table));
    memset(cluster_used, false, sizeof(cluster_used));
    cluster_used[0] = true;
    cluster_used[1] = true;
    print_string("FAT32 file system initialized.", 0x07, 0, 0);
}

void format_file(void) {
    print_string("Formatting file system...", 0x07, 0, 0);
    memset(fat_table, 0x00, sizeof(fat_table));
    memset(cluster_used, false, sizeof(cluster_used));
    cluster_used[0] = true;
    cluster_used[1] = true;
    print_string("File system formatted.", 0x07, 0, 0);
}

void get_volume_info(void) {
    print_string("Getting volume info...", 0x07, 0, 0);
}

void unmount_volume(void) {
    print_string("Unmounting volume...", 0x07, 0, 0);
}

void create_file(const char* filename) {
    print_string("Creating file: ", 0x07, 0, 0);
    print_string(filename, 0x07, 0, 0);
    int cluster_index = allocate_cluster();
    if (cluster_index == -1) {
        print_string("Error: No free clusters available.", 0x07, 0, 0);
        return;
    }
}

void open_file(const char* filename) {
    print_string("Opening file: ", 0x07, 0, 0);
    print_string(filename, 0x07, 0, 0);
}

void close_file(int file_descriptor) {
    print_string("Closing file descriptor: ", 0x07, 0, 0);
}

void delete_file(const char* filename) {
    print_string("Deleting file: ", 0x07, 0, 0);
    print_string(filename, 0x07, 0, 0);
}

void rename_file(const char* old_name, const char* new_name) {
    print_string("Renaming file from: ", 0x07, 1 ,1); 
    print_string(old_name ,0x07 ,1 ,2); 
    print_string("to: ", 0x07 ,1 ,3); 
    print_string(new_name ,0x07 ,1 ,4); 
}

size_t get_file_size(const char* filename) {
   return -1; 
}

void get_file_attributes(const char* filename) {
   print_string("Getting attributes for: ", 0x07 ,1 ,5); 
   print_string(filename ,0x07 ,1 ,6); 
}

void create_directory(const char* dirname) {
   print_string("Creating directory: ", 0x07 ,1 ,7); 
   print_string(dirname ,0x07 ,1 ,8); 
   int cluster_index = allocate_cluster();
   if (cluster_index == -1) {
       print_string("Error: No free clusters available.", 0x07 ,1 ,9); 
       return;
   }
}

void open_directory(const char* dirname) {
   print_string("Opening directory: ", 0x07 ,2 ,10); 
   print_string(dirname ,0x07 ,2 ,11); 
}

void close_directory(int dir_descriptor) {
   print_string("Closing directory descriptor: ", 0x07 ,2 ,12); 
}

void delete_directory(const char* dirname) {
   print_string("Deleting directory: ", 0x07 ,2 ,13); 
   print_string(dirname ,0x07 ,2 ,14); 
}

void list_directory_contents(const char* dirname) {
   print_string("Listing contents of directory: ", 0x07 ,2 ,15); 
   print_string(dirname ,0x07 ,2 ,16); 
}

void read_from_file(int file_descriptor, void* buffer, size_t size) {
   print_string("Reading from file descriptor: ", 0x07 ,3 ,17); 
}

void write_to_file(int file_descriptor,const void* buffer,size_t size){
   print_string("Writing to file descriptor: ",3 | (size <<4),3 |(size <<4),3 |(size <<4)); 
}

void seek_in_file(int file_descriptor,size_t offset){
}

int allocate_cluster(void){
     for(int i =2; i < CLUSTER_COUNT; i++){
         if(!cluster_used[i]){ 
             cluster_used[i] = true; 
             return i;  
         }  
     }
     return -1;  
}   

void free_cluster(int cluster_index){
     if(cluster_index >=2 && cluster_index < CLUSTER_COUNT){
         cluster_used[cluster_index] = false;  
     }  
}   

int get_next_cluster(int current_cluster){
     return -1;  
}