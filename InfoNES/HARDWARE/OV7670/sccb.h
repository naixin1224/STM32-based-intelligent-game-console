#ifndef __SCCB_H
#define __SCCB_H
#include "main.h"
  
#define SCCB_SCL_PORT    		GPIOB 	
#define SCCB_SDA_PORT    		GPIOB	

#define SCCB_SCL_PIN    		GPIO_PIN_10	      //OV7670_SCL	
#define SCCB_SDA_PIN    		GPIO_PIN_11       //OV7670_SDA
	 
#define SCCB_SCL_H    		  HAL_GPIO_WritePin(SCCB_SCL_PORT,SCCB_SCL_PIN,GPIO_PIN_SET)
#define SCCB_SCL_L   		    HAL_GPIO_WritePin(SCCB_SCL_PORT,SCCB_SCL_PIN,GPIO_PIN_RESET)

#define SCCB_SDA_H   		    HAL_GPIO_WritePin(SCCB_SDA_PORT,SCCB_SDA_PIN,GPIO_PIN_SET)		
#define SCCB_SDA_L    		  HAL_GPIO_WritePin(SCCB_SDA_PORT,SCCB_SDA_PIN,GPIO_PIN_RESET)

#define SCCB_READ_SDA       HAL_GPIO_ReadPin(SCCB_SDA_PORT,SCCB_SDA_PIN)	  
#define SCCB_ID   			    0X42  				

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void);
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);

#endif
