#include "calendar.h"
#include "key.h"
#include "st7789.h"
#include "rtc.h"
#include "string.h"

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
RTC_AlarmTypeDef sAlarm = {0};

uint8_t calendar_aclock_flag = 0;    //闹钟开关标志

uint8_t calendar_date_sta = 0;       //日期状态
uint8_t calendar_time_sta = 0;       //时间状态
uint8_t calendar_aclock_sta = 0;     //闹钟状态
uint8_t calendar_sec_sta = 0;        //秒表状态

uint8_t calendar_sec = 0;
uint8_t calendar_min = 0;
uint8_t calendar_hour = 0;

uint8_t calendar_alarm_flag = 0;

uint16_t calendar_alarm_data[500] = {
00,11,55,99,1000,5000,5000
};

typedef struct 
{
	char fname[20];
}Calendar_Name;

typedef struct 
{
	uint16_t file_start;
	uint16_t file_end;

	uint16_t list_start;
	uint16_t list_select;
	uint16_t list_end;

	uint16_t LIST_MAX;
}Calendar;

Calendar_Name list_name[4];
Calendar calendar;

void Calendar_List_View(Calendar_Name *file_info_list,Calendar *view,char sta,uint8_t is_show)
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

void Calendar_Init(void)
{
	strcpy(list_name[0].fname,"设置日期");
	strcpy(list_name[1].fname,"设置时间");
	strcpy(list_name[2].fname,"设置闹钟");
	strcpy(list_name[3].fname,"秒表");
	
	calendar.file_start = 0;
	calendar.file_end = 4;
	
	calendar.LIST_MAX = 12;
	calendar.list_start = 0;
	calendar.list_select = 0;
	if(calendar.file_end < calendar.LIST_MAX) calendar.list_end = calendar.file_end;
	else                   calendar.list_end = calendar.LIST_MAX;
	
	Calendar_List_View(list_name,&calendar,'*',1);
	
	while(1)
	{
		Key_Scan();
		if(key_value & 0x0080)          //上
		{
			key_value = key_value & ~(0x0080);
			Calendar_List_View(list_name,&calendar,'-',1);
		}else if(key_value & 0x0040)    //下
		{
			key_value = key_value & ~(0x0040);
			Calendar_List_View(list_name,&calendar,'+',1);
		}else if(key_value & 0x0020)    //左
		{
			key_value = key_value & ~(0x0020);
		}else if(key_value & 0x0010)    //右
		{
			key_value = key_value & ~(0x0010);
		}else if(key_value & 0x0100)    //返回
		{
			key_value = key_value & ~(0x0100);
			break;
		}else if(key_value & 0x0001)    //确认
		{
			key_value = key_value & ~(0x0001);
			LCD_Clear(BLACK);  //清屏
			Calendar_Run();
			LCD_Clear(BLACK);  //清屏
			Calendar_List_View(list_name,&calendar,'*',1);
		}
	}
}

void Calendar_Clock_Cnt(void)
{
	if(calendar_sec_sta == 1)
	{
		calendar_sec++;
	}
}

void Calendar_Clock(void)
{
	if(calendar_sec_sta == 1)
	{
		if(calendar_sec > 59)
		{
			calendar_sec = 0;
			calendar_min++;
			if(calendar_min > 59)
			{
				calendar_min = 0;
				calendar_hour++;
				if(calendar_hour > 23)
				{
					calendar_hour = 0;
				}
			}
		}
		LCD_ShowNums(100,100,WHITE,BLACK,calendar_hour,16,2);
		LCD_ShowChar(120,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(130,100,WHITE,BLACK,calendar_min,16,2);
		LCD_ShowChar(150,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(160,100,WHITE,BLACK,calendar_sec,16,2);
		
	}else if(calendar_sec_sta == 2)
	{
		calendar_sec_sta = 0;
		
		calendar_sec = 0;
		calendar_min = 0;
		calendar_hour = 0;
		
		LCD_ShowNums(100,100,WHITE,BLACK,calendar_hour,16,2);
		LCD_ShowChar(120,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(130,100,WHITE,BLACK,calendar_min,16,2);
		LCD_ShowChar(150,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(160,100,WHITE,BLACK,calendar_sec,16,2);
	}
}

void Calendar_Run(void)
{
	calendar_sec_sta = 0;
	calendar_date_sta = 0;
	calendar_time_sta = 0;
	calendar_aclock_sta = 0;
	
	if(calendar.list_select == 0)
	{
		HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
		
		LCD_ShowNums(80,100,RED,BLACK,2000 + sDate.Year,16,4);
		Gui_DrawFont_GBK16(120,100,WHITE,BLACK,"年");
		LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
		Gui_DrawFont_GBK16(164,100,WHITE,BLACK,"月");
		LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
		Gui_DrawFont_GBK16(208,100,WHITE,BLACK,"日");
		
	}else if(calendar.list_select == 1)
	{
		HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
		
		LCD_ShowNums(100,100,RED,BLACK,sTime.Hours,16,2);
		LCD_ShowChar(120,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
		LCD_ShowChar(150,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
	
	}else if(calendar.list_select == 2)
	{
		LCD_ShowNums(100,100,RED,BLACK,sAlarm.AlarmTime.Hours,16,2);
		LCD_ShowChar(120,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
		LCD_ShowChar(150,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
		
		if(calendar_aclock_flag == 1)
		{
			Gui_DrawFont_GBK16(200,100,WHITE,BLACK,"开");		
		}else
		{
			Gui_DrawFont_GBK16(200,100,WHITE,BLACK,"关");	
		}
	
	}else if(calendar.list_select == 3)
	{
		calendar_sec = 0;
		calendar_min = 0;
		calendar_hour = 0;
		
		LCD_ShowNums(100,100,WHITE,BLACK,calendar_hour,16,2);
		LCD_ShowChar(120,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(130,100,WHITE,BLACK,calendar_min,16,2);
		LCD_ShowChar(150,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(160,100,WHITE,BLACK,calendar_sec,16,2);
	}
	
	while(1)
	{
		Key_Scan();
		if(key_value & 0x0080)          //上
		{
			key_value = key_value & ~(0x0080);
			if(calendar.list_select == 0)
			{
				if(calendar_date_sta == 0)
				{
					if(sDate.Year < 100)
					{
						sDate.Year++;
					}
					LCD_ShowNums(80,100,RED,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 1)
				{
					if(sDate.Month < 12)
					{
						sDate.Month++;
					}
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,RED,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 2)
				{
					if(sDate.Date < 31)
					{
						sDate.Date++;
					}
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,RED,BLACK,sDate.Date,16,2);
				}
			}else if(calendar.list_select == 1)
			{
				if(calendar_time_sta == 0)
				{
					if(sTime.Hours < 23)
					{
						sTime.Hours++;
					}
					LCD_ShowNums(100,100,RED,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 1)
				{
					if(sTime.Minutes < 59)
					{
						sTime.Minutes++;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 2)
				{
					if(sTime.Seconds < 59)
					{
						sTime.Seconds++;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sTime.Seconds,16,2);
				}
			}else if(calendar.list_select == 2)
			{
				if(calendar_aclock_sta == 0)
				{
					if(sAlarm.AlarmTime.Hours < 23)
					{
						sAlarm.AlarmTime.Hours++;
					}
					LCD_ShowNums(100,100,RED,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 1)
				{
					if(sAlarm.AlarmTime.Minutes < 59)
					{
						sAlarm.AlarmTime.Minutes++;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 2)
				{
					if(sAlarm.AlarmTime.Seconds < 59)
					{
						sAlarm.AlarmTime.Seconds++;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 3)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
					calendar_aclock_flag = 1;
					if(calendar_aclock_flag == 0)
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"关");		
					}else
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"开");	
					}
				}
			}else if(calendar.list_select == 3)
			{
				
			}
		}else if(key_value & 0x0040)    //下
		{
			key_value = key_value & ~(0x0040);
			if(calendar.list_select == 0)
			{
				if(calendar_date_sta == 0)
				{
					if(sDate.Year > 0)
					{
						sDate.Year--;
					}
					LCD_ShowNums(80,100,RED,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 1)
				{
					if(sDate.Month > 0)
					{
						sDate.Month--;
					}
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,RED,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 2)
				{
					if(sDate.Date > 0)
					{
						sDate.Date--;
					}
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,RED,BLACK,sDate.Date,16,2);
				}
			}else if(calendar.list_select == 1)
			{
				if(calendar_time_sta == 0)
				{
					if(sTime.Hours > 0)
					{
						sTime.Hours--;
					}
					LCD_ShowNums(100,100,RED,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 1)
				{
					if(sTime.Minutes > 0)
					{
						sTime.Minutes--;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 2)
				{
					if(sTime.Seconds > 0)
					{
						sTime.Seconds--;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sTime.Seconds,16,2);
				}
			}else if(calendar.list_select == 2)
			{
				if(calendar_aclock_sta == 0)
				{
					if(sAlarm.AlarmTime.Hours > 0)
					{
						sAlarm.AlarmTime.Hours--;
					}
					LCD_ShowNums(100,100,RED,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 1)
				{
					if(sAlarm.AlarmTime.Minutes > 0)
					{
						sAlarm.AlarmTime.Minutes--;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 2)
				{
					if(sAlarm.AlarmTime.Seconds > 0)
					{
						sAlarm.AlarmTime.Seconds--;
					}
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 3)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
					calendar_aclock_flag = 0;
					if(calendar_aclock_flag == 0)
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"关");		
					}else
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"开");	
					}
				}
			}else if(calendar.list_select == 3)
			{
				
			}
		}else if(key_value & 0x0020)    //左
		{
			key_value = key_value & ~(0x0020);
			if(calendar.list_select == 0)
			{
				if(calendar_date_sta > 0)
				{
					calendar_date_sta--;
				}
				if(calendar_date_sta == 0)
				{
					LCD_ShowNums(80,100,RED,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 1)
				{
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,RED,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 2)
				{
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,RED,BLACK,sDate.Date,16,2);
				}
			}else if(calendar.list_select == 1)
			{
				if(calendar_time_sta > 0)
				{
					calendar_time_sta--;
				}
				if(calendar_time_sta == 0)
				{
					LCD_ShowNums(100,100,RED,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 1)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 2)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sTime.Seconds,16,2);
				}
			}else if(calendar.list_select == 2)
			{
				if(calendar_aclock_sta > 0)
				{
					calendar_aclock_sta--;
				}
				if(calendar_aclock_sta == 0)
				{
					LCD_ShowNums(100,100,RED,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 1)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 2)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sAlarm.AlarmTime.Seconds,16,2);
					if(calendar_aclock_flag == 0)
					{
						Gui_DrawFont_GBK16(200,100,WHITE,BLACK,"关");		
					}else
					{
						Gui_DrawFont_GBK16(200,100,WHITE,BLACK,"开");	
					}
				}else if(calendar_aclock_sta == 3)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
					if(calendar_aclock_flag == 0)
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"关");		
					}else
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"开");	
					}
				}
			}else if(calendar.list_select == 3)
			{
				
			}
		}else if(key_value & 0x0010)    //右
		{
			key_value = key_value & ~(0x0010);
			if(calendar.list_select == 0)
			{
				if(calendar_date_sta < 2)
				{
					calendar_date_sta++;
				}
				if(calendar_date_sta == 0)
				{
					LCD_ShowNums(80,100,RED,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 1)
				{
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,RED,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,WHITE,BLACK,sDate.Date,16,2);
				}else if(calendar_date_sta == 2)
				{
					LCD_ShowNums(80,100,WHITE,BLACK,2000 + sDate.Year,16,4);
					LCD_ShowNums(140,100,WHITE,BLACK,sDate.Month,16,2);
					LCD_ShowNums(184,100,RED,BLACK,sDate.Date,16,2);
				}
			}else if(calendar.list_select == 1)
			{
				if(calendar_time_sta < 2)
				{
					calendar_time_sta++;
				}
				if(calendar_time_sta == 0)
				{
					LCD_ShowNums(100,100,RED,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 1)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sTime.Seconds,16,2);
				}else if(calendar_time_sta == 2)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sTime.Seconds,16,2);
				}
			}else if(calendar.list_select == 2)
			{
				if(calendar_aclock_sta < 3)
				{
					calendar_aclock_sta++;
				}
				if(calendar_aclock_sta == 0)
				{
					LCD_ShowNums(100,100,RED,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 1)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,RED,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
				}else if(calendar_aclock_sta == 2)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,RED,BLACK,sAlarm.AlarmTime.Seconds,16,2);
					if(calendar_aclock_flag == 0)
					{
						Gui_DrawFont_GBK16(200,100,WHITE,BLACK,"关");		
					}else
					{
						Gui_DrawFont_GBK16(200,100,WHITE,BLACK,"开");	
					}
				}else if(calendar_aclock_sta == 3)
				{
					LCD_ShowNums(100,100,WHITE,BLACK,sAlarm.AlarmTime.Hours,16,2);
					LCD_ShowNums(130,100,WHITE,BLACK,sAlarm.AlarmTime.Minutes,16,2);
					LCD_ShowNums(160,100,WHITE,BLACK,sAlarm.AlarmTime.Seconds,16,2);
					if(calendar_aclock_flag == 0)
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"关");		
					}else
					{
						Gui_DrawFont_GBK16(200,100,RED,BLACK,"开");	
					}
				}
			}else if(calendar.list_select == 3)
			{
				
			}
		}else if(key_value & 0x0100)    //返回
		{
			key_value = key_value & ~(0x0100);
			if(calendar.list_select == 0)
			{
				
			}else if(calendar.list_select == 1)
			{
				
			}else if(calendar.list_select == 2)
			{
				
			}else if(calendar.list_select == 3)
			{
			
			}
			break;
		}else if(key_value & 0x0002)    //清除
		{
			if(calendar.list_select == 0)
			{
				
			}else if(calendar.list_select == 1)
			{
				
			}else if(calendar.list_select == 2)
			{
				
			}else if(calendar.list_select == 3)
			{
				calendar_sec_sta = 2;
			}
		}else if(key_value & 0x0001)    //确认
		{
			key_value = key_value & ~(0x0001);
			if(calendar.list_select == 0)                        //设置日期
			{
				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);    
				break;
			}else if(calendar.list_select == 1)                  //设置时间
			{
				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);    
				break;
			}else if(calendar.list_select == 2)                  //设置闹钟
			{
				sAlarm.AlarmTime.SubSeconds = 0;
				sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
				sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
				sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
				sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
				sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
				sAlarm.AlarmDateWeekDay = 1;
				sAlarm.Alarm = RTC_ALARM_A;
				HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
				break;
			}else if(calendar.list_select == 3)                  //秒表(暂停/开始)
			{
				if(calendar_sec_sta == 0)
				{
					calendar_sec_sta = 1;
				}else
				{
					calendar_sec_sta = 0;
				}
			}
		}
		
		Calendar_Clock();
	}
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)  //秒表计时中断
{
	Calendar_Clock_Cnt();
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)         //闹钟中断
{
	if(calendar_aclock_flag == 1)
	{
		//Gui_DrawFont_GBK16(0,20,RED,BLACK,"闹钟");
		calendar_alarm_flag = 1;
	}
}
