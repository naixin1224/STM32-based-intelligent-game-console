#include "wav.h"
#include "ff.h"
#include "uart.h"
#include "key.h"
#include "st7789.h"
#include "string.h"
#include "fatfs.h"
#include "timer.h"
#include "music_picture.h"

uint32_t         		 wav_buff_play_cnt       = 0;           //播放数据计数器
uint8_t          		 wav_buff_play_select    = 0;           //播放缓冲区选择
uint8_t          		 wav_buff_read_select    = 0;           //读取存放缓冲区选择
__IO uint32_t    		 wav_buff_num[2]         = {0};         //(缓冲区1，缓冲区2)每次实际读取数据数量
__IO uint8_t     		 wav_buff_state[2]       = {0};         //(缓冲区1，缓冲区2)状态标志位
uint32_t         		 wav_buff[2][WAV_BUFF_SIZE] = {0};      //缓冲区1,2
int16_t*          	 wav_p_buff;                            //指向缓冲区指针
int32_t         		 wav_paly_dat;                          //播放数据

float                wav_volume = 0.2;                       //音量
uint16_t             wav_name_num;                           //音乐数量
uint8_t              wav_play_flag;                          //音乐播放暂停标志位，0暂停，1播放

FILINFO              wav_name_list[20];                      //音乐名列表
WAV_Info_TypeDef     wav_info;                               //音乐信息
WAV_ListView_TypeDef wav_listview;                           //音乐显示列表


void WAV_ListView(FILINFO *file_info_list,WAV_ListView_TypeDef *view,char sta,uint8_t is_show)
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

void WAV_Init(void)
{
	wav_play_flag = 0;
	
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	
	Fatfs_File_Scan("0:/",AM_ARC,"wav",wav_name_list,&wav_name_num);//搜索音乐
	
	wav_listview.file_start = 0;
	wav_listview.file_end = wav_name_num;
	
	wav_listview.LIST_MAX = 12;
	wav_listview.list_start = 0;
	wav_listview.list_select = 0;
	if(wav_listview.file_end < wav_listview.LIST_MAX) wav_listview.list_end = wav_listview.file_end;
	else                   wav_listview.list_end = wav_listview.LIST_MAX;
	
	WAV_ListView(wav_name_list,&wav_listview,'*',1);
	
	while(1)
	{
		Key_Scan();
		if(key_value & 0x0080)          //上
		{
			key_value = key_value & ~(0x0080);
			WAV_ListView(wav_name_list,&wav_listview,'-',1);
		}else if(key_value & 0x0040)    //下
		{
			key_value = key_value & ~(0x0040);
			WAV_ListView(wav_name_list,&wav_listview,'+',1);
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
			LCD_Clear(BLACK);                      //清屏
			Gui_DrawFont_GBK16(130,50,WHITE,BLACK,(uint8_t*)wav_name_list[wav_listview.list_select].fname);
			LCD_DrawBMP(65,150,95,180,(uint8_t*)PrevR);
			LCD_DrawBMP(145,150,175,180,(uint8_t*)PrevS);
			LCD_DrawBMP(225,150,255,180,(uint8_t*)PrevL);
			WAV_Run();
			LCD_Clear(BLACK);                      //清屏
			WAV_ListView(wav_name_list,&wav_listview,'*',1);
		}
	}
}
//设置采样率的播放时间
void WAV_SetPlay_SamplesPerSec(uint32_t samples)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 168-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1*1000000/samples;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
//播放主界面运行
void WAV_Run(void)
{
	FIL fil;
	UINT br;
	wav_play_flag = 0;
	if(f_open(&fil,(const TCHAR*)wav_name_list[wav_listview.list_select].fname, FA_READ) == FR_OK)
	{      
		f_read(&fil,&wav_info,sizeof(wav_info),&br);
		printf("Riff_ID[0]:%d\r\n",wav_info.Riff_ID[0]);
		printf("Riff_ID[1]:%d\r\n",wav_info.Riff_ID[1]);
		printf("Riff_ID[2]:%d\r\n",wav_info.Riff_ID[2]);
		printf("Riff_ID[3]:%d\r\n",wav_info.Riff_ID[3]);
		printf("Riff_Size:%d\r\n",wav_info.Riff_Size);
		printf("Riff_Format[0]:%d\r\n",wav_info.Riff_Format[0]);
		printf("Riff_Format[1]:%d\r\n",wav_info.Riff_Format[1]);
		printf("Riff_Format[2]:%d\r\n",wav_info.Riff_Format[2]);
		printf("Riff_Format[3]:%d\r\n",wav_info.Riff_Format[3]);
		printf("Fmt_ID[0]:%d\r\n",wav_info.Fmt_ID[0]);
		printf("Fmt_ID[1]:%d\r\n",wav_info.Fmt_ID[1]);
		printf("Fmt_ID[2]:%d\r\n",wav_info.Fmt_ID[2]);
		printf("Fmt_ID[3]:%d\r\n",wav_info.Fmt_ID[3]);
		printf("Fmt_Size:%d\r\n",wav_info.Fmt_Size);
		
		
		printf("FormatTag:%d\r\n",wav_info.FormatTag);
		printf("Channels:%d\r\n",wav_info.Channels);
		printf("SamplesPerSec:%d\r\n",wav_info.SamplesPerSec);
		printf("AvgBytesPerSec:%d\r\n",wav_info.AvgBytesPerSec);
		printf("BlockAlign:%d\r\n",wav_info.BlockAlign);
		printf("BitsPerSample:%d\r\n",wav_info.BitsPerSample);
		
		
		printf("Data_ID[0]:%d\r\n",wav_info.Data_ID[0]);
		printf("Data_ID[1]:%d\r\n",wav_info.Data_ID[1]);
		printf("Data_ID[2]:%d\r\n",wav_info.Data_ID[2]);
		printf("Data_ID[3]:%d\r\n",wav_info.Data_ID[3]);
		printf("Data_Size:%d\r\n",wav_info.Data_Size);
		f_lseek(&fil,44);
		
		WAV_SetPlay_SamplesPerSec(wav_info.SamplesPerSec);//设置采样率的播放时间
		
		while(1)
		{
			if(wav_play_flag == 1)
			{
				while(wav_buff_state[wav_buff_read_select] == 1);
				f_read(&fil,&wav_buff[wav_buff_read_select][0],4096,&br);
				if(br <= 0)
				{
					WAV_ListView(wav_name_list,&wav_listview,'+',0);
					LCD_Fill(130,50,320-1,70-1,BLACK);
					Gui_DrawFont_GBK16(130,50,WHITE,BLACK,(uint8_t*)wav_name_list[wav_listview.list_select].fname);
					f_open(&fil,(const TCHAR*)wav_name_list[wav_listview.list_select].fname, FA_READ);
					f_read(&fil,&wav_info,sizeof(wav_info),&br);
					f_lseek(&fil,44);
					WAV_SetPlay_SamplesPerSec(wav_info.SamplesPerSec);//设置采样率的播放时间
				}
				wav_buff_num[wav_buff_read_select] = br/(wav_info.BitsPerSample/8.0);
				wav_buff_state[wav_buff_read_select] = 1;
				if(wav_buff_read_select == 0)
				{
					wav_buff_read_select = 1;
				}else
				{
					wav_buff_read_select = 0;
				}
			}
			
			Key_Scan();
			if(key_value & 0x0020)    //左
			{
				key_value = key_value & ~(0x0020);
				WAV_ListView(wav_name_list,&wav_listview,'-',0);
				LCD_Fill(130,50,320-1,70-1,BLACK);
				Gui_DrawFont_GBK16(130,50,WHITE,BLACK,(uint8_t*)wav_name_list[wav_listview.list_select].fname);
				f_open(&fil,(const TCHAR*)wav_name_list[wav_listview.list_select].fname, FA_READ);
				f_read(&fil,&wav_info,sizeof(wav_info),&br);
				f_lseek(&fil,44);
				WAV_SetPlay_SamplesPerSec(wav_info.SamplesPerSec);//设置采样率的播放时间
			}else if(key_value & 0x0010)    //右
			{
				key_value = key_value & ~(0x0010);
				WAV_ListView(wav_name_list,&wav_listview,'+',0);
				LCD_Fill(130,50,320-1,70-1,BLACK);
				Gui_DrawFont_GBK16(130,50,WHITE,BLACK,(uint8_t*)wav_name_list[wav_listview.list_select].fname);
				f_open(&fil,(const TCHAR*)wav_name_list[wav_listview.list_select].fname, FA_READ);
				f_read(&fil,&wav_info,sizeof(wav_info),&br);
				f_lseek(&fil,44);
				WAV_SetPlay_SamplesPerSec(wav_info.SamplesPerSec);//设置采样率的播放时间
			}else if(key_value & 0x0100)    //返回
			{
				key_value = key_value & ~(0x0100);
				wav_play_flag = 0;
				f_close(&fil);
				break;
			}else if(key_value & 0x0001)    //确认
			{
				if(wav_play_flag == 0)
				{
					wav_play_flag = 1;
					LCD_DrawBMP(145,150,175,180,(uint8_t*)PrevP);
				}else
				{
					wav_play_flag = 0;
					LCD_DrawBMP(145,150,175,180,(uint8_t*)PrevS);
				}
			}
		}
	}
}

//设置音量
void WAV_SetVolume(int32_t *dat,float vol)
{
	*dat = *dat * vol; 
	if (*dat < -0x8000)  
	{  
			*dat = -0x8000;  
	}else if (*dat > 0x7fff)
	{  
			*dat = 0x7fff;  
	}  
}
//放中断里面自动处理播放
void WAV_Play(void)
{
	if(wav_buff_state[wav_buff_play_select] == 1)
	{
		wav_paly_dat = wav_p_buff[wav_buff_play_cnt];
		WAV_SetVolume(&wav_paly_dat,wav_volume);//调节音量
		wav_paly_dat = (wav_paly_dat+0x7fff) >> 4;
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,wav_paly_dat);
			
		wav_buff_play_cnt += wav_info.Channels;//每次取数据下标偏移量
		if(wav_buff_play_cnt >= wav_buff_num[wav_buff_play_select])
		{
			wav_buff_play_cnt = 0;
			wav_buff_state[wav_buff_play_select] = 0;
			wav_buff_play_select++;
			if(wav_buff_play_select >= 2)
			{
				wav_buff_play_select = 0;
			}
			wav_p_buff = (int16_t*)&wav_buff[wav_buff_play_select][0];
		}
	}
}

