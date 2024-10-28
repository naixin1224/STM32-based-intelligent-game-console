#include "main.h"
#include "sccb.h"

void SCCB_Delay_Us(uint32_t us)
{
	uint8_t temp;
	while(us--)
	{
		temp = 37;
		while(temp--);
	}
}

void SCCB_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = SCCB_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SCCB_SDA_PORT, &GPIO_InitStruct);
}

void SCCB_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = SCCB_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SCCB_SDA_PORT, &GPIO_InitStruct);
}

void SCCB_Init(void)
{			
	SCCB_SCL_H;	
	SCCB_SDA_H;
	
	SCCB_SDA_OUT();
}		 

void SCCB_Start(void)
{
	SCCB_SDA_H;     //数据线高电平	   
	SCCB_SCL_H;	    //在时钟线高的时候数据线由高至低
	SCCB_Delay_Us(50);  
	SCCB_SDA_L;
	SCCB_Delay_Us(50);	 
	SCCB_SCL_L;	    //数据线恢复低电平，单操作函数必要	  
}

void SCCB_Stop(void)
{
	SCCB_SDA_L;
	SCCB_Delay_Us(50);	 
	SCCB_SCL_H;	
	SCCB_Delay_Us(50); 
	SCCB_SDA_H;	
	SCCB_Delay_Us(50);
}  

void SCCB_No_Ack(void)
{
	SCCB_Delay_Us(50);
	SCCB_SDA_H;	
	SCCB_SCL_H;	
	SCCB_Delay_Us(50);
	SCCB_SCL_L;	
	SCCB_Delay_Us(50);
	SCCB_SDA_L;	
	SCCB_Delay_Us(50);
}

uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)
		{
			SCCB_SDA_H;	
		}
		else
		{
			SCCB_SDA_L;
		}
		dat<<=1;
		SCCB_Delay_Us(50);
		SCCB_SCL_H;	
		SCCB_Delay_Us(50);
		SCCB_SCL_L;		   
	}			 
	SCCB_SDA_IN();		//设置SDA为输入 
	SCCB_Delay_Us(50);
	SCCB_SCL_H;			//接收第九位,以判断是否发送成功
	SCCB_Delay_Us(50);
	if(SCCB_READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCCB_SCL_L;		 
	SCCB_SDA_OUT();		//设置SDA为输出    
	return res;  
}	 

uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;    
	SCCB_SDA_IN();		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		SCCB_Delay_Us(50);
		SCCB_SCL_H;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		SCCB_Delay_Us(50);
		SCCB_SCL_L;
	}	
	SCCB_SDA_OUT();		//设置SDA为输出    
	return temp;
} 							    

uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	SCCB_Delay_Us(100);
	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	SCCB_Delay_Us(100);
	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
	SCCB_Stop();	  
	return	res;
}		  					    

uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	SCCB_Delay_Us(100);	 
  SCCB_WR_Byte(reg);			//写寄存器地址	  
	SCCB_Delay_Us(100);	  
	SCCB_Stop();   
	SCCB_Delay_Us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	SCCB_Delay_Us(100);
	val=SCCB_RD_Byte();		 	//读取数据
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}
