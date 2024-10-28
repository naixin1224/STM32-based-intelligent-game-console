#include "game.h"
#include "nes_main.h"
#include "fatfs.h"
#include "st7789.h"
#include "string.h"
#include "key.h"
#include "dac.h"
#include "wav.h"
#include "spi.h"

typedef struct 
{
	char fname[20];
}Game_NameTypeDef;

typedef struct 
{
	uint16_t file_start;
	uint16_t file_end;

	uint16_t list_start;
	uint16_t list_select;
	uint16_t list_end;

	uint16_t LIST_MAX;
}Game_List_View_TypeDef;

FILINFO game_name_list[30];
uint16_t game_name_num;

Game_List_View_TypeDef game_view;
Game_NameTypeDef game_list_name[30];

void Game_List_View(Game_NameTypeDef *file_info_list,Game_List_View_TypeDef *view,char sta,uint8_t is_show)
{
	uint16_t i;
	
	if(sta == '+')
	{
		if(view->list_select < view->list_end-1)
		{
			view->list_select++;
			if(is_show == 1)
			{
				Gui_DrawFont_GBK16(0,(view->list_select-view->list_start-1)*20,WHITE,BLACK,(uint8_t*)file_info_list[view->list_select-1].fname);
				Gui_DrawFont_GBK16(0,(view->list_select-view->list_start)*20,RED,BLACK,(uint8_t*)file_info_list[view->list_select].fname);
			}
		}else if(view->list_end < view->file_end)
		{
			LCD_Clear(BLACK);  //清屏
			view->list_start++;
			view->list_select++;
			view->list_end++;
			if(is_show == 1)
			{
				for(i = view->list_start; i < view->list_end; i++)
				{
					if(i == view->list_select)
					{
						Gui_DrawFont_GBK16(0,(i-view->list_start)*20,RED,BLACK,(uint8_t*)file_info_list[i].fname);
					}else
					{
						Gui_DrawFont_GBK16(0,(i-view->list_start)*20,WHITE,BLACK,(uint8_t*)file_info_list[i].fname);
					}
				}
			}
		}
	}else if(sta == '-')
	{
		if(view->list_select > view->list_start)
		{
			view->list_select--;
			if(is_show == 1)
			{
				Gui_DrawFont_GBK16(0,(view->list_select-view->list_start)*20,RED,BLACK,(uint8_t*)file_info_list[view->list_select].fname);
				Gui_DrawFont_GBK16(0,(view->list_select-view->list_start+1)*20,WHITE,BLACK,(uint8_t*)file_info_list[view->list_select+1].fname);
			}
		}else if(view->list_start > view->file_start)
		{
			LCD_Clear(BLACK);  //清屏
			view->list_start--;
			view->list_select--;
			view->list_end--;
			if(is_show == 1)
			{
				for(i = view->list_start; i < view->list_end; i++)
				{
					if(i == view->list_select)
					{
						Gui_DrawFont_GBK16(0,(i-view->list_start)*20,RED,BLACK,(uint8_t*)file_info_list[i].fname);
					}else
					{
						Gui_DrawFont_GBK16(0,(i-view->list_start)*20,WHITE,BLACK,(uint8_t*)file_info_list[i].fname);
					}
				}
			}
		}
	}else if(sta == '*')
	{
		if(is_show == 1)
		{
			for(i = view->list_start; i < view->list_end; i++)
			{
				if(i == view->list_select)
				{
					Gui_DrawFont_GBK16(0,(i-view->list_start)*20,RED,BLACK,(uint8_t*)file_info_list[i].fname);
				}else
				{
					Gui_DrawFont_GBK16(0,(i-view->list_start)*20,WHITE,BLACK,(uint8_t*)file_info_list[i].fname);
				}
			}
		}
	}
}

void Game_Init(void)
{
	uint8_t i;
	
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	
	Fatfs_File_Scan("0:/",AM_ARC,"nes",game_name_list,&game_name_num);//搜索游戏
	
	for(i = 0; i < game_name_num; i++)
	{
		strcpy(game_list_name[i].fname,game_name_list[i].fname);
	}
	
	game_view.file_start = 0;
	game_view.file_end = game_name_num;
	
	game_view.LIST_MAX = 12;
	game_view.list_start = 0;
	game_view.list_select = 0;
	if(game_view.file_end < game_view.LIST_MAX) game_view.list_end = game_view.file_end;
	else                   game_view.list_end = game_view.LIST_MAX;
	
	Game_List_View(game_list_name,&game_view,'*',1);
	
	while(1)
	{
		Key_Scan();
		if(key_value & 0x0080)          //上
		{
			key_value = key_value & ~(0x0080);
			Game_List_View(game_list_name,&game_view,'-',1);
		}else if(key_value & 0x0040)    //下
		{
			key_value = key_value & ~(0x0040);
			Game_List_View(game_list_name,&game_view,'+',1);
		}else if(key_value & 0x0020)    //左
		{
			key_value = key_value & ~(0x0020);
		}else if(key_value & 0x0010)    //右
		{
			key_value = key_value & ~(0x0010);
		}else if(key_value & 0x0100)    //返回
		{
			key_value = key_value & ~(0x0100);
			HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
			break;
		}else if(key_value & 0x0001)    //确认
		{
			key_value = key_value & ~(0x0001);
			LCD_Clear(BLACK);             //清屏
			nes_load((char*)game_list_name[game_view.list_select].fname);
			while(HAL_DMA_GetState(hspi1.hdmatx) != HAL_DMA_STATE_READY);//等待DMA传输完成
			LCD_Clear(BLACK);             //清屏
			Game_List_View(game_list_name,&game_view,'*',1);
		}
	}
}
