#ifndef _ST7735S_H
#define _ST7735S_H
#include "main.h"

#define SPI1_DR *(__IO uint32_t*)0x4001300c
#define SPI1_SR *(__IO uint32_t*)0x40013008

#define ST7735S_CS_PORT   GPIOD
#define ST7735S_RST_PORT  GPIOD
#define ST7735S_DC_PORT   GPIOD
#define ST7735S_LED_PORT  GPIOD
						
#define ST7735S_RST_PIN   GPIO_PIN_7
#define ST7735S_DC_PIN    GPIO_PIN_6
#define ST7735S_CS_PIN    GPIO_PIN_5
#define ST7735S_LED_PIN   GPIO_PIN_4

#define LCD_CS_H         ST7735S_CS_PORT->BSRR  = ST7735S_CS_PIN;
#define LCD_CS_L         ST7735S_CS_PORT->BSRR  = (uint32_t)ST7735S_CS_PIN << 16U
														 
#define LCD_RST_H        ST7735S_RST_PORT->BSRR = ST7735S_RST_PIN;
#define LCD_RST_L        ST7735S_RST_PORT->BSRR = (uint32_t)ST7735S_RST_PIN << 16U
														 
#define LCD_DC_H         ST7735S_DC_PORT->BSRR  = ST7735S_DC_PIN;
#define LCD_DC_L         ST7735S_DC_PORT->BSRR  = (uint32_t)ST7735S_DC_PIN << 16U
														 
#define LCD_LED_H        ST7735S_LED_PORT->BSRR  = ST7735S_LED_PIN;
#define LCD_LED_L        ST7735S_LED_PORT->BSRR  = (uint32_t)ST7735S_LED_PIN << 16U

#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //×ØÉ«
#define BRRED 			0XFC07 //×ØºìÉ«
#define GRAY  			0X8430 //»ÒÉ«

#define LCD_WIDTH   128
#define LCD_HIGH    128

uint8_t ST7735S_ReadWriteByte(uint8_t byte);
void ST7735S_Init(void);
void LCD_SetDirection(uint8_t dir);
void LCD_WR_REG(uint8_t data);
void LCD_WR_DATA(uint8_t data);
void LCD_WR_DATA_16Bit(uint16_t data);
void LCD_SetWindows(uint16_t x,uint16_t y,uint16_t width,uint16_t high);
void LCD_SetCursor(uint16_t x, uint16_t y);
void LCD_Clear(uint16_t color);
void LCD_Fill(uint16_t x,uint16_t y,uint16_t width,uint16_t high,uint16_t color);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_Draw_Circle_8(int xc, int yc, int x, int y, uint16_t color);
void LCD_DrawCircle(int xc, int yc,int r, uint16_t color, int fill);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t font_color,uint16_t background_color,uint8_t *p,uint8_t size);
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,uint32_t num,uint8_t font_size);
void LCD_ShowNumDouble(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,double num,uint8_t font_size);
void LCD_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
void LCD_DrawBMP(uint16_t x,uint16_t y, uint16_t width, uint16_t high, const unsigned char *p);
void LCD_DMAColor(uint16_t x,uint16_t y,uint16_t width,uint16_t high,uint8_t* color,uint16_t size);
void LCD_DMAPushColor(uint8_t* color,uint16_t size);

#endif
