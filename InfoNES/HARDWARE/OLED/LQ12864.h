#include "codetab.h"
#include "main.h"
#include "gpio.h"

#define	Brightness	0xCF 
#define X_WIDTH 	  128
#define Y_WIDTH 	  64

#define IIC_PORT    GPIOB
#define SDA_PIN     GPIO_PIN_7
#define SCL_PIN     GPIO_PIN_6

#define SDA_H         IIC_PORT->BSRR  = SDA_PIN;
#define SDA_L         IIC_PORT->BSRR  = (uint32_t)SDA_PIN << 16U

#define SCL_H         IIC_PORT->BSRR  = SCL_PIN;
#define SCL_L         IIC_PORT->BSRR  = (uint32_t)SCL_PIN << 16U

unsigned char CMD_Data[] = 
{
	0xae,0x20,0x10,0xb0,0xc8,0x00,0x10,  /*横向A0，A1*/
	0x40,0x81,0xff,0xa1,0xa6,0xa8,0x3f,  /*纵向C0，C8*/
	0xa4,0xd3,0x00,0xd5,0xf0,0xd9,0x22,  /*亮度0x00-0xff*/
	0xda,0x12,0xdb,0x20,0x8d,0x14,0xaf
};

/*********************OLED驱动程序用的延时程序************************************/
void OLED_Delay_US(unsigned int t)
{
	unsigned int i;
	while(t--)
	{
		i = 8;
		while(i--);
	}
}

void OLED_Delay_MS(unsigned int t)
{
	unsigned int i;
	while(t--)
	{
		OLED_Delay_US(1000);
	}
}

void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_Start()
{
	IIC_SDA_OUT();
	SCL_H;		
	SDA_H;
	OLED_Delay_US(4);
	SDA_L;
	OLED_Delay_US(4);
	SCL_L;
}

void IIC_Stop()
{
	IIC_SDA_OUT();
	SCL_L;
	SDA_L;
	OLED_Delay_US(4);
	SCL_H;
	OLED_Delay_US(4);
	SDA_H;
}

/**********************************************
// 通过I2C总线写一个字节
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	IIC_SDA_OUT();
	SCL_L;
	for(i = 0; i < 8; i++)
	{
		if(IIC_Byte & 0x80)
		{
			SDA_H;
		}else
		{
			SDA_L;
		}
		OLED_Delay_US(4);
		SCL_H;
		OLED_Delay_US (4);
		SCL_L;
		IIC_Byte <<= 1;
	}
	SDA_H;
	SCL_H;
	SCL_L;
}

/*********************OLED写数据************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40);			       //write data
	Write_IIC_Byte(IIC_Data);
	IIC_Stop();
}
/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	Write_IIC_Byte(0x00);			       //write command
	Write_IIC_Byte(IIC_Command);
	IIC_Stop();
}
/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char x;
	unsigned char y;
	for(y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x02);   //0x00 0x02
		OLED_WrCmd(0x10);
		for(x = 0; x < X_WIDTH; x++)
			OLED_WrDat(bmp_dat);
	}
}
/*********************OLED清屏************************************/
void OLED_CLS(void)
{
	OLED_Fill(0);
}
/*********************OLED初始化************************************/
void OLED_Init(void)
{
	unsigned char i;
	
//	GPIO_InitTypeDef  GPIO_InitStructure; 
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);  
//	
//	GPIO_InitStructure.GPIO_Pin = SDA_PIN | SCL_PIN;                     
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;              
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //设置IO接口速度（2/10/50MHz）    
//	GPIO_Init(IIC_PORT,&GPIO_InitStructure);	
//	GPIO_SetBits(IIC_PORT,SDA_PIN | SCL_PIN);
	
	OLED_Delay_MS(100);                                    //100ms初始化之前的延时很重要！
	for(i = 0; i < 28; i++)
	{
		OLED_WrCmd(CMD_Data[i]);
	}
	OLED_CLS();                                            //初始清屏
} 
/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[])
{
	unsigned char c = 0,i = 0,j = 0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
			OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
void OLED_P8x16Num(unsigned char x, unsigned char y,unsigned char num)
{
	unsigned int n;
	unsigned char i;
	n = (num*16)+256;
	OLED_Set_Pos(x,y);
	for(i = 0; i < 8; i++)
	{
		OLED_WrDat(F8X16[n+i]);
	}
	OLED_Set_Pos(x,y+1);
	for(i = 0; i < 8; i++)
	{
		OLED_WrDat(F8X16[n+i+8]);
	}
}
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}
