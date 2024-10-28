#include "ff.h"
#include "fatfs.h"
#include "string.h"

/*************
*path:路径
*format:格式
*suffix:后缀名
*file_info_list:文件列表
*file_info_num:文件数量
*************/
void Fatfs_File_Scan(const char *path,uint8_t format,char *suffix,FILINFO *file_info_list,uint16_t *file_info_num)
{
	DIR dir;
	FRESULT res;
	FILINFO file_info;
	*file_info_num = 0;
	char* pstr;
	
	if(f_opendir(&dir,path) == FR_OK)
	{
		while(1)
		{
			res = f_readdir(&dir,&file_info);
			if((res != FR_OK) || (file_info.fname[0] == 0)) break;
			if(*suffix == '*')
			{
				if(file_info.fattrib & format)//format = AM_ARC
				{
					file_info_list[*file_info_num] = file_info;
					*file_info_num = *file_info_num + 1;
				}
			}else
			{
				pstr = strrchr(file_info.fname,'.');
				if(pstr == NULL) continue;
				pstr++;
				if(file_info.fattrib & format && (strcasecmp(pstr,suffix) == 0))//format = AM_ARC
				{
					file_info_list[*file_info_num] = file_info;
					*file_info_num = *file_info_num + 1;
				}
			}
		}
	}
	f_closedir(&dir);
}
