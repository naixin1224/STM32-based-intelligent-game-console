#ifndef _OV7660_H
#define _OV7660_H
#include "sys.h"

#define OV7670_VSYNC_PORT     GPIOE
#define FIFO_WRST_PORT    		GPIOB 	
#define FIFO_WREN_PORT    		GPIOB	
#define FIFO_RRST_PORT    		GPIOC 	
#define FIFO_OE_PORT    		  GPIOA
#define FIFO_RCLK_PORT    		GPIOC 		

//      FIFO(D0-D7)           PE8-PE15      //8λ�������
#define OV7670_VSYNC_PIN    	GPIO_PIN_7    //֡�ź�
#define FIFO_WRST_PIN    		  GPIO_PIN_0	 	//дָ�븴λ
#define FIFO_WREN_PIN    		  GPIO_PIN_1    //д��FIFOʹ
#define FIFO_RRST_PIN    		  GPIO_PIN_4 	  //��ָ�븴λ
#define FIFO_OE_PIN    		    GPIO_PIN_4    //Ƭѡ�ź�(O
#define FIFO_RCLK_PIN    		  GPIO_PIN_5 	  //������ʱ��

#define FIFO_WRST_H    		    HAL_GPIO_WritePin(FIFO_WRST_PORT,FIFO_WRST_PIN,GPIO_PIN_SET)
#define FIFO_WRST_L   		    HAL_GPIO_WritePin(FIFO_WRST_PORT,FIFO_WRST_PIN,GPIO_PIN_RESET)

#define FIFO_WREN_H    		    HAL_GPIO_WritePin(FIFO_WREN_PORT,FIFO_WREN_PIN,GPIO_PIN_SET)
#define FIFO_WREN_L   		    HAL_GPIO_WritePin(FIFO_WREN_PORT,FIFO_WREN_PIN,GPIO_PIN_RESET)

#define FIFO_RRST_H    		    HAL_GPIO_WritePin(FIFO_RRST_PORT,FIFO_RRST_PIN,GPIO_PIN_SET)
#define FIFO_RRST_L   		    HAL_GPIO_WritePin(FIFO_RRST_PORT,FIFO_RRST_PIN,GPIO_PIN_RESET)

#define FIFO_OE_H    		      HAL_GPIO_WritePin(FIFO_OE_PORT,FIFO_OE_PIN,GPIO_PIN_SET)
#define FIFO_OE_L   		      HAL_GPIO_WritePin(FIFO_OE_PORT,FIFO_OE_PIN,GPIO_PIN_RESET)

#define FIFO_RCLK_H    		    HAL_GPIO_WritePin(FIFO_RCLK_PORT,FIFO_RCLK_PIN,GPIO_PIN_SET)
#define FIFO_RCLK_L   		    HAL_GPIO_WritePin(FIFO_RCLK_PORT,FIFO_RCLK_PIN,GPIO_PIN_RESET)


#define lightmode  0
#define saturation 3
#define brightness 2
#define contrast   4
#define effect     0


extern uint8_t OV7670_STA;

unsigned char OV7670_Init(void);

#endif
