#include "st7735s.h"
#include "font.h"
#include "spi.h"
#include "dma.h"

uint8_t ST7735S_ReadWriteByte(uint8_t byte)
{
	uint8_t Rxdata;
//	HAL_SPI_Transmit(&hspi1,&byte,1,1000);
//	uint8_t Rxdata;
//	HAL_SPI_TransmitReceive(&hspi1,&byte,&Rxdata,1,1000);
	
	while((SPI1_SR & ((uint16_t)0x0002)) == (uint16_t)RESET);
	SPI1_DR = byte;
	while((SPI1_SR & ((uint16_t)0x0001)) == (uint16_t)RESET);
	Rxdata = SPI1_DR;
	
	return Rxdata;
} 

void ST7735S_Init(void)
{  		 
	__HAL_SPI_ENABLE(&hspi1);
	
	LCD_LED_L;
	
	LCD_RST_L;
	HAL_Delay(100);	
	LCD_RST_H;
	HAL_Delay(50);
	
	LCD_WR_REG(0x11);//Sleep exit 
	HAL_Delay(120);		
	//ST7735R Frame Rate
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 

	LCD_WR_REG(0xB2); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 

	LCD_WR_REG(0xB3); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 
	
	LCD_WR_REG(0xB4); //Column inversion 
	LCD_WR_DATA(0x07); 
	
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x84); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA(0xC5); 

	LCD_WR_REG(0xC2); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x00); 

	LCD_WR_REG(0xC3); 
	LCD_WR_DATA(0x8A); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA(0x8A); 
	LCD_WR_DATA(0xEE); 
	
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA(0x0E); 
	
	LCD_WR_REG(0x36); //MX, MY, RGB mode 				 
	LCD_WR_DATA(0xC8); 
	
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x1a); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x18); 
	LCD_WR_DATA(0x2f); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x20); 
	LCD_WR_DATA(0x22); 
	LCD_WR_DATA(0x1f); 
	LCD_WR_DATA(0x1b); 
	LCD_WR_DATA(0x23); 
	LCD_WR_DATA(0x37); 
	LCD_WR_DATA(0x00); 	
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x10); 

	LCD_WR_REG(0xe1); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x1b); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x33); 
	LCD_WR_DATA(0x2c); 
	LCD_WR_DATA(0x29); 
	LCD_WR_DATA(0x2e); 
	LCD_WR_DATA(0x30); 
	LCD_WR_DATA(0x30); 
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x3f); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0x10);  
	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7f);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x9f);
	
	LCD_WR_REG(0xF0); //Enable test command  
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xF6); //Disable ram power save mode 
	LCD_WR_DATA(0x00); 
	
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA(0x05); 	
	LCD_WR_REG(0x29);//Display on	
	
	LCD_WR_REG(0x36);  
	LCD_WR_DATA(0xc8);	
	
	LCD_LED_H;
	
	LCD_Clear(BLACK);
}

void LCD_SetDirection(uint8_t dir)
{
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(dir);     //0x00,0x20,0x40,0x60,0x80,0xa0,0xc0,0xe0
}

void LCD_WR_REG(uint8_t data)
{ 
	LCD_CS_L;
	LCD_DC_L;
  ST7735S_ReadWriteByte(data);
	LCD_CS_H;
}

void LCD_WR_DATA(uint8_t data)
{
	LCD_CS_L;
	LCD_DC_H;
  ST7735S_ReadWriteByte(data);
	LCD_CS_H;
}

void LCD_WR_DATA_16Bit(uint16_t data)
{	
	LCD_CS_L;
	LCD_DC_H;
	ST7735S_ReadWriteByte(data>>8);
	ST7735S_ReadWriteByte(data);
	LCD_CS_H;
}

void LCD_SetWindows(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{	     
	LCD_WR_REG(0x2a);	
	LCD_WR_DATA(x1>>8);
	LCD_WR_DATA(0x00FF&x1+2);		
	LCD_WR_DATA(x2>>8);
	LCD_WR_DATA(0x00FF&x2+2);

	LCD_WR_REG(0x2b);	
	LCD_WR_DATA(y1>>8);
	LCD_WR_DATA(0x00FF&y1+3);		
	LCD_WR_DATA(y2>>8);
	LCD_WR_DATA(0x00FF&y2+3);	
	
	LCD_WR_REG(0X2C);      //开始写入GRAM
}

void LCD_SetCursor(uint16_t x, uint16_t y)
{	  	    			
	LCD_SetWindows(x,y,x,y);
} 

void LCD_Clear(uint16_t color)
{
	uint16_t i;
	uint16_t j;
	LCD_SetWindows(0,0,LCD_WIDTH-1,LCD_HIGH-1);
  for(j = 0; j < LCD_HIGH; j++)
  {
		for(i = 0; i < LCD_WIDTH; i++)
		{
			LCD_WR_DATA_16Bit(color);
		}
	}
} 

void LCD_Fill(uint16_t x,uint16_t y,uint16_t width,uint16_t high,uint16_t color)
{  	
	uint16_t i;
	uint16_t j;
	LCD_SetWindows(x,y,width,high);       //设置显示窗口
	for(i = 0; i <= high-y; i++)
	{
		for(j = 0; j <= width-x; j++)
		{
			LCD_WR_DATA(color>>8);
			LCD_WR_DATA(color);
		}
	}
}

void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetCursor(x,y);                //设置光标位置 
	LCD_WR_DATA_16Bit(color); 
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}  

void LCD_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	//LCD_Fill(x1,y1,x2,y2,&color);
}

void LCD_Draw_Circle_8(int xc, int yc, int x, int y, uint16_t color)
{
	LCD_DrawPoint(xc + x, yc + y, color);
	LCD_DrawPoint(xc - x, yc + y, color);
	LCD_DrawPoint(xc + x, yc - y, color);
	LCD_DrawPoint(xc - x, yc - y, color);
	LCD_DrawPoint(xc + y, yc + x, color);
	LCD_DrawPoint(xc - y, yc + x, color);
	LCD_DrawPoint(xc + y, yc - x, color);
	LCD_DrawPoint(xc - y, yc - x, color);
}

void LCD_DrawCircle(int xc, int yc,int r, uint16_t color, int fill)
{
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	if (fill) 
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				LCD_Draw_Circle_8(xc, yc, x, yi, color);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			LCD_Draw_Circle_8(xc, yc, x, y, color);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,uint8_t num,uint8_t size,uint8_t mode)
{  							  
  uint8_t temp,t1,t;
	uint16_t y0 = y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	
	//LCD_SetWindows(x,y,x+size-1,y+size-1);
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_DrawPoint(x,y,font_color);
			else if(mode==0)LCD_DrawPoint(x,y,background_color);
			temp<<=1;
			y++;
			if(y>=LCD_HIGH)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_WIDTH)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   

void LCD_ShowString(uint16_t x,
										uint16_t y,
										uint16_t width,
										uint16_t height,
										uint16_t font_color,
										uint16_t background_color,
										uint8_t *p,
										uint8_t size)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
	while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
	{       
			if(x>=width){x=x0;y+=size;}
			if(y>=height)break;//退出
			LCD_ShowChar(x,y,font_color,background_color,*p,size,0);
			x+=size/2;
			p++;
	}  
}

uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
//size 12  16
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,uint32_t num,uint8_t font_size)
{         	
	uint8_t i;
	uint8_t temp;
	uint8_t enshow = 0;			
	uint8_t len = 0;
	uint32_t num1;
	
	num1 = num;
	while(1)
	{
		len++;
		if(num1 > 0)
		{
			num1 = num1 / 10;
		}else
		{
			if(len > 1)
				len--;
			break;
		}
	}
	
	for(i = 0;i < len; i++)
	{
		temp = (num / mypow(10,len - i - 1)) % 10;
		if(enshow == 0 && i < (len-1))
		{
			if(temp == 0)
			{
				LCD_ShowChar(x+(font_size/2)*i,y,font_color,background_color,' ',font_size,0);
				continue;
			}else enshow=1; 
		}
	 	LCD_ShowChar(x+(font_size/2)*i,y,font_color,background_color,temp+'0',font_size,0); 
	}
} 
//size 12  16
void LCD_ShowNumDouble(uint16_t x,uint16_t y,uint16_t font_color,uint16_t background_color,double num,uint8_t font_size)
{         	
	double num1;
	uint8_t num_flag = 0;
	uint16_t i;
	uint16_t pow = 1;             //指数
	uint16_t point = 0;           //小数点位置下标
	uint16_t len = 0;             //数据长度  
	uint16_t lens = 0;
	
	if((num > 0) && ((uint32_t)num == 0))
	{
		num += 1.0;
		num_flag = 1;
	}
	num1 = num;
	if(num1 > (uint32_t)num1)
	{
		while(1)
		{
			point++;
			if((uint32_t)num1 > 0)
			{
				num1 = num1 / 10;
			}else
			{
				if(point > 1)
				{
					point--;
				}
				break;
			}
		}
	}
	num1 = num;
	if(num1 > (uint32_t)num1)
	{
		while(1)
		{
			if(num1 > (uint32_t)num1)
			{
				num1 = num1 * 10.0;
			}else
			{
				break;
			}
		}
	}
	num = num1;
	while(1)
	{
		len++;
		if((uint32_t)num1 > 0)
		{
			num1 = num1 / 10;
		}else
		{
			if(len > 1)
				len--;
			break;
		}
	}
	if(point != 0)
		lens = len + 1;
	else
		lens = len;
	pow = 0;
	for(i = 0; i < lens; i++)
	{
		if(i == 0)
		{
			if(num_flag)
			{
				LCD_ShowChar(x+(font_size/2)*i,y,font_color,background_color,'0',font_size,0); 
				pow++;
			}else
			{
				LCD_ShowChar(x+(font_size/2)*i,y,font_color,background_color,(uint32_t)num/mypow(10,len-1-pow)%10 + '0',font_size,0); 
				pow++;
			}
		}else 
		if((i == point) && (point != 0))
		{
			LCD_ShowChar(x+(font_size/2)*i,y,font_color,background_color,'.',font_size,0); 
		}else
		{
			LCD_ShowChar(x+(font_size/2)*i,y,font_color,background_color,(uint32_t)num/mypow(10,len-1-pow)%10 + '0',font_size,0); 
			pow++;
		}
	}
} 

void LCD_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	LCD_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) LCD_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	LCD_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) LCD_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	LCD_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) LCD_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
	}
}

void LCD_DrawBMP(uint16_t x,uint16_t y, uint16_t width, uint16_t high, const unsigned char *p) 
{
  uint32_t i; 
	uint16_t picH,picL; 
	LCD_SetWindows(x,y,width-1,high-1);        //窗口设置
  for(i = 0; i < (width - x)*(high - y); i++)
	{	
		picL=*(p+i*2);	                         //数据低位在前
		picH=*(p+i*2+1);				
		LCD_WR_DATA_16Bit(picH<<8|picL);  				
	}	
	LCD_SetWindows(0,0,LCD_WIDTH,LCD_HIGH);           //恢复显示窗口为全屏	
}

void LCD_DMAColor(uint16_t x,uint16_t y,uint16_t width,uint16_t high,uint8_t* color,uint16_t size)
{
	LCD_CS_L;
	LCD_DC_L;
	ST7735S_ReadWriteByte(0x2a);
	LCD_DC_H;
	ST7735S_ReadWriteByte(x >> 8);
	ST7735S_ReadWriteByte(x & 0xff);
	ST7735S_ReadWriteByte(width >> 8);
	ST7735S_ReadWriteByte(width & 0xff);
	
	LCD_DC_L;
	ST7735S_ReadWriteByte(0x2b);
	LCD_DC_H;
	ST7735S_ReadWriteByte(y >> 8);
	ST7735S_ReadWriteByte(y & 0xff);
	ST7735S_ReadWriteByte(high >> 8);
	ST7735S_ReadWriteByte(high & 0xff);
	
	LCD_DC_L;
	ST7735S_ReadWriteByte(0x2c);

	LCD_DC_H;
	HAL_SPI_Transmit_DMA(&hspi1,color,size);
	while(HAL_DMA_GetState(hspi1.hdmatx) != HAL_DMA_STATE_READY);
	LCD_CS_H;
}

void LCD_DMAPushColor(uint8_t* color,uint16_t size)
{
	LCD_CS_L;
	LCD_DC_H;
	HAL_SPI_Transmit_DMA(&hspi1,color,size);
	while(HAL_DMA_GetState(hspi1.hdmatx) != HAL_DMA_STATE_READY);
	LCD_CS_H;
}

