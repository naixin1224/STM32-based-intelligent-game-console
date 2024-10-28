#include "key.h"

uint16_t key_value = 0;

void Key_Init(void)
{
	
	
} 

uint16_t Key_Scan(void)
{
	key_value = 0;
	if(!KEY_BACK)
	{
  	HAL_Delay(10);
  	if(!KEY_BACK)
  	{
			key_value = key_value | 0x0100;
			while(!KEY_BACK);
		}
	}
	
	if(!KEY_UP)
	{
  	HAL_Delay(10);
  	if(!KEY_UP)
  	{
			key_value = key_value | 0x0080;
			while(!KEY_UP);
		}
	}
	
	if(!KEY_DOWN)
	{
  	HAL_Delay(10);
  	if(!KEY_DOWN)
  	{
			key_value = key_value | 0x0040;
			while(!KEY_DOWN);
		}
	}
	
	if(!KEY_LEFT)
	{
  	HAL_Delay(10);
  	if(!KEY_LEFT)
  	{
			key_value = key_value | 0x0020;
			while(!KEY_LEFT);
		}
	}
	
	if(!KEY_RIGH)
	{
  	HAL_Delay(10);
  	if(!KEY_RIGH)
  	{
			key_value = key_value | 0x0010;
			while(!KEY_RIGH);
		}
	}
	
	if(!KEY_START)
	{
  	HAL_Delay(10);
  	if(!KEY_START)
  	{
			key_value = key_value | 0x0008;
			while(!KEY_START);
		}
	}
	
	if(!KEY_SELECT)
	{
  	HAL_Delay(10);
  	if(!KEY_SELECT)
  	{
			key_value = key_value | 0x0004;
			while(!KEY_SELECT);
		}
	}
	
	if(!KEY_B)
	{
  	HAL_Delay(10);
  	if(!KEY_B)
  	{
			key_value = key_value | 0x0002;
			while(!KEY_B);
		}
	}
	
	if(!KEY_A)
	{
  	HAL_Delay(10);
  	if(!KEY_A)
  	{
			key_value = key_value | 0x0001;
			while(!KEY_A);
		}
	}
	
	return key_value;
}
