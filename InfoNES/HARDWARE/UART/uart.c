#include "uart.h"

uint8_t uart_data[5];

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,1000);
	return ch;
}

int fgetc(FILE *f)
{
	uint8_t ch = 0;
	HAL_UART_Receive(&huart1,(uint8_t*)&ch,1,1000);
	return (int)ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		HAL_UART_Transmit(&huart1,uart_data,1,1000);
		HAL_UART_Receive_IT(&huart1,uart_data,1);
	}
}
