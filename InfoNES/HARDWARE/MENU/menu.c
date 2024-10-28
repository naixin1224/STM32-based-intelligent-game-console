#include "menu.h"
#include "st7789.h"
#include "picture.h"
#include "key.h"
#include "jpeg.h"
#include "calendar.h"
#include "game.h"
#include "wav.h"
#include "timer.h"

uint8_t menu_sta = 0;//�˵�״̬
uint8_t x_index = 1; //ͼ��x����
uint8_t y_index = 2; //ͼ��y����

void Menu_Init(void)
{
	sound_play_flag = 0;
	LCD_Clear(BLACK);  //����
	LCD_DrawBMP(75,35,135,95,(uint8_t *)music_picture); LCD_DrawBMP(175,35,235,95,(uint8_t *)photo_picture);
	Gui_DrawFont_GBK16(89,100,WHITE,BLACK,"����");Gui_DrawFont_GBK16(189,100,WHITE,BLACK,"ͼƬ");
	LCD_DrawBMP(75,125,135,185,(uint8_t *)game_picture);LCD_DrawBMP(175,125,235,185,(uint8_t *)clock_picture);
	Gui_DrawFont_GBK16(89,190,WHITE,BLACK,"��Ϸ");      Gui_DrawFont_GBK16(189,190,WHITE,BLACK,"ʱ��");
}

void Menu_Del(void)
{
	Menu_Init();
	while(1)
	{
		Key_Scan();
		if(key_value & 0x0080)          //��
		{
			if(y_index < 2)
				y_index++;
		}else if(key_value & 0x0040)    //��
		{
			if(y_index > 1)
				y_index--;
		}else if(key_value & 0x0020)    //��
		{
			if(x_index > 1)
				x_index--;
		}else if(key_value & 0x0010)    //��
		{
			if(x_index < 2)
				x_index++;
		}else if(key_value & 0x0100)    //����
		{
			menu_sta = 0;
			LCD_Clear(BLACK);
			break;
		}else if(key_value & 0x0001)    //ȷ��
		{
			menu_sta = 2;
		}
		
		if((x_index == 1) && (y_index == 1))              //ѡ�����½ǣ���Ϸ
		{
			LCD_DrawRoundRectangle(73,33,137,97,5,BLACK,0);
			LCD_DrawRoundRectangle(173,33,237,97,5,BLACK,0);
			LCD_DrawRoundRectangle(73,123,137,187,5,RED,0);
			LCD_DrawRoundRectangle(173,123,237,187,5,BLACK,0);
			if(menu_sta == 2)
			{
				LCD_Clear(BLACK);  //����
				sound_play_flag = 2;
				Game_Init();
				menu_sta = 1;
				Menu_Init();
			}
		}else if((x_index == 1) && (y_index == 2))        //ѡ�����Ͻǣ�����
		{
			LCD_DrawRoundRectangle(73,33,137,97,5,RED,0);
			LCD_DrawRoundRectangle(173,33,237,97,5,BLACK,0);
			LCD_DrawRoundRectangle(73,123,137,187,5,BLACK,0);
			LCD_DrawRoundRectangle(173,123,237,187,5,BLACK,0);
			if(menu_sta == 2)
			{
				LCD_Clear(BLACK);  //����
				sound_play_flag = 1;
				WAV_Init();
				menu_sta = 1;
				Menu_Init();
			}
		}else if((x_index == 2) && (y_index == 1))        //ѡ�����½ǣ�ʱ��
		{
			LCD_DrawRoundRectangle(73,33,137,97,5,BLACK,0);
			LCD_DrawRoundRectangle(173,33,237,97,5,BLACK,0);
			LCD_DrawRoundRectangle(73,123,137,187,5,BLACK,0);
			LCD_DrawRoundRectangle(173,123,237,187,5,RED,0);
			if(menu_sta == 2)
			{
				LCD_Clear(BLACK);  //����
				Calendar_Init();
				menu_sta = 1;
				Menu_Init();
			}
		}else if((x_index == 2) && (y_index == 2))        //ѡ�����Ͻ�,ͼƬ
		{
			LCD_DrawRoundRectangle(73,33,137,97,5,BLACK,0);
			LCD_DrawRoundRectangle(173,33,237,97,5,RED,0);
			LCD_DrawRoundRectangle(73,123,137,187,5,BLACK,0);
			LCD_DrawRoundRectangle(173,123,237,187,5,BLACK,0);
			if(menu_sta == 2)
			{
				LCD_Clear(BLACK);  //����
				JPEG_Seek();
				menu_sta = 1;
				Menu_Init();
			}
		}
	}
}
