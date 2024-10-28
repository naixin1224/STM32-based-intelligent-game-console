#include "timer.h"
#include "wav.h"
#include "calendar.h"
#include "key.h"
#include "NES_main.h" 

uint32_t  time_cnt = 0;
uint8_t sound_play_flag = 0;

void Time_Init(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(calendar_alarm_flag == 1)//ÄÖÖÓ
		{
			if(!KEY_BACK)  //¹Ø±ÕÄÖÖÓ
			{
				calendar_alarm_flag = 0;
			}
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,calendar_alarm_data[time_cnt]);
			time_cnt++;
			if(time_cnt >= 500)
			{
				time_cnt = 0;
			}
		}else
		{
			if(sound_play_flag == 1)
			{
				WAV_Play();
			}else if(sound_play_flag == 2)
			{
				NES_Play();
			}
		}
	}
}

