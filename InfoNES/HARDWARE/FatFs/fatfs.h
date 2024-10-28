#ifndef __FATFS_H
#define __FATFS_H	
#include "ff.h"

void Fatfs_File_Scan(const char *path,uint8_t format,char *suffix,FILINFO *file_info_list,uint16_t *file_info_num);

#endif
