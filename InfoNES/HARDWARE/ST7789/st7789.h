#ifndef _ST7789_H
#define _ST7789_H
#include "main.h"

#define SPI1_DR *(__IO uint32_t*)0x4001300c
#define SPI1_SR *(__IO uint32_t*)0x40013008
#define SPI2_DR *(__IO uint32_t*)0x4000380c
#define SPI2_SR *(__IO uint32_t*)0x40003808

#define ST7789_CS_PORT   GPIOD
#define ST7789_RST_PORT  GPIOD
#define ST7789_DC_PORT   GPIOD
#define ST7789_LED_PORT  GPIOD

#define ST7789_RST_PIN   GPIO_PIN_2
#define ST7789_DC_PIN    GPIO_PIN_1
#define ST7789_CS_PIN    GPIO_PIN_0
#define ST7789_LED_PIN   GPIO_PIN_3

#define LCD_CS_H         ST7789_CS_PORT->BSRR  = ST7789_CS_PIN;
#define LCD_CS_L         ST7789_CS_PORT->BSRR  = (uint32_t)ST7789_CS_PIN << 16U

#define LCD_RST_H        ST7789_RST_PORT->BSRR = ST7789_RST_PIN;
#define LCD_RST_L        ST7789_RST_PORT->BSRR = (uint32_t)ST7789_RST_PIN << 16U

#define LCD_DC_H         ST7789_DC_PORT->BSRR  = ST7789_DC_PIN;
#define LCD_DC_L         ST7789_DC_PORT->BSRR  = (uint32_t)ST7789_DC_PIN << 16U

#define LCD_LED_H        ST7789_LED_PORT->BSRR  = ST7789_LED_PIN;
#define LCD_LED_L        ST7789_LED_PORT->BSRR  = (uint32_t)ST7789_LED_PIN << 16U

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

#define LCD_WIDTH   320
#define LCD_HIGH    240

uint8_t ST7789_ReadWriteByte(uint8_t byte);
void ST7789_Init(void);
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
void LCD_DrawRoundRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t r,uint16_t color,uint8_t fill);
void LCD_DrawRoundRectangle1(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t r,uint8_t width,uint16_t border_color,uint16_t fill_color);
void LCD_Draw_Circle_8(int xc, int yc, int x, int y, uint16_t color);
void LCD_DrawCircle(int xc, int yc,int r, uint16_t color, int fill);
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,uint8_t num,uint8_t size,uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t font_color,uint16_t background_color,uint8_t *p,uint8_t size);
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,uint32_t num,uint8_t font_size);
void LCD_ShowNums(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,uint32_t num,uint8_t font_size,uint8_t cnt);
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
void LCD_DrawBMP(uint16_t x,uint16_t y, uint16_t width, uint16_t high, uint8_t *p);
void LCD_DMAColor(uint16_t x,uint16_t y,uint16_t width,uint16_t high,uint8_t* color,uint16_t size);
void LCD_DMAPushColor(uint8_t* color,uint16_t size);


#endif
