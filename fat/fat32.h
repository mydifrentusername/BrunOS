#ifndef FAT_FAT32_H
#define FAT_FAT32_H

#include "../api/kernelapi.h"


//define sector size
#define SECTOR_SIZE 512

//define cluster size
#define CLUSTER_SIZE 4096

//define cluster count
#define CLUSTER_COUNT 16

//define cluster count
#define CLUSTER_COUNT_FAT32 32


//initialization
void fat32_init(void);

//FAT functions
void create_file();
void open_file();
void close_file();
void delete_file();
void rename_file();
void getfile_size();
void getfile_attributes();
void create_directory();
void open_directory();
void close_directory();
void delete_directory();
void list_directorycontents();
void readfromfile();
void writetofile();
void seekinfile();
void alocatecluster();
void freecluster();
void getnextcluster();
void format_file();
void getvolumeinfo();
void unmountvolume();

#endif