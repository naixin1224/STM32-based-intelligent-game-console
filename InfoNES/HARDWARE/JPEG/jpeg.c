#include "jpeg.h"
#include "tjpgd.h"
#include "uart.h"
#include "st7789.h"
#include "key.h"
#include "fatfs.h"


size_t in_func(JDEC* jd,uint8_t* buff,size_t nbyte)
{
	uint32_t br;
	IODEV *dev = (IODEV*)jd->device;
	if(buff)
	{
		printf("in_func1\n");
		return (f_read(dev->fp,buff,nbyte,&br) == FR_OK) ? br:0;
	}else
	{
		printf("in_func2\n");
		return (f_lseek(dev->fp,f_tell(dev->fp) + nbyte) == FR_OK) ? nbyte:0;
	}
}

int out_func(JDEC* jd,void* bitmap,JRECT* rect)
{
	LCD_DrawBMP(rect->left,rect->top,rect->right+1,rect->bottom+1,(uint8_t*)bitmap);
	return 1;
}

#define app_size 3500
uint8_t app_buff[app_size];
int JPEG_Code(char *filepath)
{
	JRESULT res;
	FRESULT ret;
	JDEC jdec;
	IODEV devid;
	FIL fil;
	
	devid.fp = &fil;
	
	ret = f_open(devid.fp,filepath,FA_READ);
	if(ret != FR_OK)
	{
		printf("JPEG open fail1\r\n");
		f_close(devid.fp);
		return -1;
	}
	printf("JPEG open ok1\r\n");
	
	res = jd_prepare(&jdec,in_func,app_buff,app_size,&devid);
	printf("jd_prepare start\r\n");
	if(res == JDR_OK)
	{
		printf("jd_prepare ok\n");
		res = jd_decomp(&jdec,out_func,0);
		if(res == JDR_OK)
		{
			printf("jd_decomp() ok\n");
		}else
		{
			printf("jd_decomp() failed1 (rc=%d)\n",res);
		}
	}else
	{
		printf("jd_prepare fail (rc=%d)\n",res);
	}
	f_close(devid.fp);
	return res;
}

FILINFO file_info_list[30];
uint16_t file_info_num = 0;

uint16_t file_start;
uint16_t file_end;

uint16_t list_start;
uint16_t list_select;
uint16_t list_end;
uint16_t LIST_MAX;

void List_View(FILINFO *file_info_list,char sta,uint8_t is_show)
{
	uint16_t i;
	
	if(sta == '+')
	{
		if(list_select < list_end-1)
		{
			list_select++;
			if(is_show == 1)
			{
				LCD_ShowString(0,(list_select-list_start-1)*20,LCD_WIDTH-1,LCD_HIGH-1,WHITE,BLACK,(uint8_t*)file_info_list[list_select-1].fname,16);
				LCD_ShowString(0,(list_select-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,RED,BLACK,(uint8_t*)file_info_list[list_select].fname,16);
			}
		}else if(list_end < file_end)
		{
			LCD_Clear(BLACK);  //清屏
			list_start++;
			list_select++;
			list_end++;
			if(is_show == 1)
			{
				for(i = list_start; i < list_end; i++)
				{
					if(i == list_select)
					{
						LCD_ShowString(0,(i-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,RED,BLACK,(uint8_t*)file_info_list[i].fname,16);
					}else
					{
						LCD_ShowString(0,(i-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,WHITE,BLACK,(uint8_t*)file_info_list[i].fname,16);
					}
				}
			}
		}
	}else if(sta == '-')
	{
		if(list_select > list_start)
		{
			list_select--;
			if(is_show == 1)
			{
				LCD_ShowString(0,(list_select-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,RED,BLACK,(uint8_t*)file_info_list[list_select].fname,16);
				LCD_ShowString(0,(list_select-list_start+1)*20,LCD_WIDTH-1,LCD_HIGH-1,WHITE,BLACK,(uint8_t*)file_info_list[list_select+1].fname,16);
			}
		}else if(list_start > file_start)
		{
			LCD_Clear(BLACK);  //清屏
			list_start--;
			list_select--;
			list_end--;
			if(is_show == 1)
			{
				for(i = list_start; i < list_end; i++)
				{
					if(i == list_select)
					{
						LCD_ShowString(0,(i-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,RED,BLACK,(uint8_t*)file_info_list[i].fname,16);
					}else
					{
						LCD_ShowString(0,(i-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,WHITE,BLACK,(uint8_t*)file_info_list[i].fname,16);
					}
				}
			}
		}
	}else if(sta == '*')
	{
		if(is_show == 1)
		{
			for(i = list_start; i < list_end; i++)
			{
				if(i == list_select)
				{
					LCD_ShowString(0,(i-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,RED,BLACK,(uint8_t*)file_info_list[i].fname,16);
				}else
				{
					LCD_ShowString(0,(i-list_start)*20,LCD_WIDTH-1,LCD_HIGH-1,WHITE,BLACK,(uint8_t*)file_info_list[i].fname,16);
				}
			}
		}
	}
}

void JPEG_Seek(void)
{
	uint8_t sta = 0;
	
	Fatfs_File_Scan("0:/",AM_ARC,"jpg",file_info_list,&file_info_num);
	
	file_start = 0;
	file_end = file_info_num;
	
	LIST_MAX = 12;
	list_start = 0;
	list_select = 0;
	if(file_end < LIST_MAX) list_end = file_end;
	else                   list_end = LIST_MAX;
	
	List_View(file_info_list,'*',1);
	
	while(1)
	{
		Key_Scan();
		
		if(key_value & 0x0080)          //上
		{
			key_value = key_value & ~(0x0080);
			if(sta == 1) continue;
			List_View(file_info_list,'-',1);
		}else if(key_value & 0x0040)    //下
		{
			key_value = key_value & ~(0x0040);
			if(sta == 1) continue;
			List_View(file_info_list,'+',1);
		}else if(key_value & 0x0020)    //左
		{
			key_value = key_value & ~(0x0020);
			if(sta == 1)
			{
				if(list_select == file_start) continue;
				List_View(file_info_list,'-',0);
				LCD_Clear(BLACK);  //清屏
				JPEG_Code(file_info_list[list_select].fname);
			}
		}else if(key_value & 0x0010)    //右
		{
			key_value = key_value & ~(0x0010);
			if(sta == 1)
			{
				if(list_select == file_end-1) continue;
				List_View(file_info_list,'+',0);
				LCD_Clear(BLACK);  //清屏
				JPEG_Code(file_info_list[list_select].fname);
			}
		}else if(key_value & 0x0100)    //返回
		{
			key_value = key_value & ~(0x0100);
			if(sta == 1) 
			{
				sta = 0;
				LCD_Clear(BLACK);  //清屏
				List_View(file_info_list,'*',1);
			}else 
			{
				break;
			}
		}else if(key_value & 0x0001)    //确认
		{
			key_value = key_value & ~(0x0001);
			if(sta == 1) continue;
			sta = 1;
			LCD_Clear(BLACK);  //清屏
			JPEG_Code(file_info_list[list_select].fname);
		}
	}
}
