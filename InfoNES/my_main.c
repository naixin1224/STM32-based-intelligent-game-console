#include "my_main.h"
#include "ff.h"
#include "st7789.h"
#include "key.h"
#include "uart.h"
#include "wav.h"
#include "w25qxx.h" 
#include "jpeg.h"
#include "timer.h"
#include "rtc.h"
#include "menu.h"
#include "adc.h"

FATFS fatfs;

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

uint8_t adc_cnt = 0;
float adc_dat = 0;

void my_main(void)
{
	Time_Init();
	ST7789_Init();
	
	if(f_mount(&fatfs,"0:",1) == FR_OK)
	{
		printf("文件系统挂载成功\r\n");
	}else
	{
		printf("文件系统挂载失败\r\n");
	}
	
	while(1)
	{
		adc_dat = 0;
		for(adc_cnt = 0; adc_cnt < 20; adc_cnt++)
		{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,1000);
			if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC))
			{
				adc_dat = adc_dat + HAL_ADC_GetValue(&hadc1);
			}
		}
//		adc_dat = adc_dat / 20.0;
//		adc_dat = (3.3*(adc_dat/4096.0)*100)*2;
//		LCD_ShowNums(200,0,WHITE,BLACK,adc_dat,16,3);
//		adc_dat = (adc_dat - 345)/65.0*100;
//		if(adc_dat > 100)
//		{
//			adc_dat = 100;
//		}
//		LCD_ShowNums(290,0,WHITE,BLACK,adc_dat,16,3);
		
		HAL_Delay(100);
		
		HAL_RTC_GetTime(&hrtc,&rtc_time,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&rtc_date,RTC_FORMAT_BIN);
		
		LCD_ShowNums(120,100,WHITE,BLACK,rtc_time.Hours,16,2);
		LCD_ShowChar(140,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(150,100,WHITE,BLACK,rtc_time.Minutes,16,2);
		LCD_ShowChar(170,100,WHITE,BLACK,':',16,0);
		LCD_ShowNums(180,100,WHITE,BLACK,rtc_time.Seconds,16,2);
		
		LCD_ShowNums(100,120,WHITE,BLACK,2000 + rtc_date.Year,16,4);
		Gui_DrawFont_GBK16(140,120,WHITE,BLACK,"年");
		LCD_ShowNums(160,120,WHITE,BLACK,rtc_date.Month,16,2);
		Gui_DrawFont_GBK16(180,120,WHITE,BLACK,"月");
		LCD_ShowNums(200,120,WHITE,BLACK,rtc_date.Date,16,2);
		Gui_DrawFont_GBK16(220,120,WHITE,BLACK,"日");
		
		Key_Scan();
		if(key_value & 0x0001)
		{
			menu_sta = 1;
		}
		
		if(menu_sta == 1)
		{
			Menu_Del();
		}
	}
}
