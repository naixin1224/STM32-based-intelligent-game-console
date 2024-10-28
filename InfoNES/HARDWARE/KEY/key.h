#ifndef __KEY_H
#define __KEY_H	 
#include "main.h" 

#define KEY_BACK_PORT    GPIOD
#define KEY_UP_PORT      GPIOD
#define KEY_DOWN_PORT    GPIOD
#define KEY_LEFT_PORT    GPIOD
#define KEY_RIGH_PORT    GPIOD
#define KEY_START_PORT   GPIOA
#define KEY_SELECT_PORT  GPIOA
#define KEY_B_PORT       GPIOA
#define KEY_A_PORT       GPIOA

#define KEY_BACK_PIN     GPIO_PIN_12
#define KEY_UP_PIN       GPIO_PIN_11
#define KEY_DOWN_PIN     GPIO_PIN_9
#define KEY_LEFT_PIN     GPIO_PIN_10
#define KEY_RIGH_PIN     GPIO_PIN_8
#define KEY_START_PIN    GPIO_PIN_3
#define KEY_SELECT_PIN   GPIO_PIN_2
#define KEY_B_PIN        GPIO_PIN_1
#define KEY_A_PIN        GPIO_PIN_0

#define KEY_BACK 		  (uint16_t)(KEY_BACK_PORT->IDR   & KEY_BACK_PIN)
#define KEY_UP 		    (uint16_t)(KEY_UP_PORT->IDR     & KEY_UP_PIN)
#define KEY_DOWN 		  (uint16_t)(KEY_DOWN_PORT->IDR   & KEY_DOWN_PIN)	
#define KEY_LEFT 		 	(uint16_t)(KEY_LEFT_PORT->IDR   & KEY_LEFT_PIN)  
#define KEY_RIGH 		  (uint16_t)(KEY_RIGH_PORT->IDR   & KEY_RIGH_PIN)  
#define KEY_START 		(uint16_t)(KEY_START_PORT->IDR  & KEY_START_PIN)  
#define KEY_SELECT 		(uint16_t)(KEY_SELECT_PORT->IDR & KEY_SELECT_PIN)   
#define KEY_B 		    (uint16_t)(KEY_B_PORT->IDR      & KEY_B_PIN)
#define KEY_A 		    (uint16_t)(KEY_A_PORT->IDR      & KEY_A_PIN ) 

extern uint16_t key_value;

void Key_Init(void);
uint16_t Key_Scan(void);

#endif
