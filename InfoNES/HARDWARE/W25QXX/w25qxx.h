#ifndef __W25QXX_H
#define __W25QXX_H			    
#include "main.h"  

#define SPI1_DR *(__IO uint32_t*)0x4001300c
#define SPI1_SR *(__IO uint32_t*)0x40013008
#define SPI2_DR *(__IO uint32_t*)0x4000380c
#define SPI2_SR *(__IO uint32_t*)0x40003808

#define W25QXX_CS_PORT      GPIOB
#define W25QXX_CS_PIN       GPIO_PIN_14
#define W25QXX_CS_H         W25QXX_CS_PORT->BSRR  = W25QXX_CS_PIN;
#define W25QXX_CS_L         W25QXX_CS_PORT->BSRR  = (uint32_t)W25QXX_CS_PIN << 16U

#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

#define FLASH_BLOCK_SIZE   	8     	    //���С����λ����
#define FLASH_SECTOR_SIZE 	512			    //������С����λ�ֽ�  			    
#define	FLASH_SECTOR_COUNT  2048*4	    //������

extern uint16_t W25QXX_TYPE;					  //����W25QXXоƬ�ͺ�		   

////////////////////////////////////////////////////////////////////////////////// 
//ָ���
#define W25X_WriteEnable		  0x06 
#define W25X_WriteDisable		  0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		  0x0B 
#define W25X_FastReadDual		  0x3B 
#define W25X_PageProgram		  0x02 
#define W25X_BlockErase			  0xD8 
#define W25X_SectorErase		  0x20 
#define W25X_ChipErase			  0xC7 
#define W25X_PowerDown			  0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void W25QXX_Init(void);
uint16_t  W25QXX_ReadID(void);  	    		//��ȡFLASH ID
uint8_t	 W25QXX_ReadSR(void);        		  //��ȡ״̬�Ĵ��� 
void W25QXX_Write_SR(uint8_t sr);  			  //д״̬�Ĵ���
void W25QXX_Write_Enable(void);  		      //дʹ�� 
void W25QXX_Write_Disable(void);		      //д����
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);              //��ȡflash
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);           //д��flash
void W25QXX_Erase_Chip(void);    	  	          //��Ƭ����
void W25QXX_Erase_Sector(uint32_t Dst_Addr);	  //��������
void W25QXX_Wait_Busy(void);           	        //�ȴ�����
void W25QXX_PowerDown(void);        	          //�������ģʽ
void W25QXX_WAKEUP(void);				                //����

uint8_t W25QXX_FatFs_Init(void);
uint8_t W25QXX_FatFs_Status(void);
uint8_t W25QXX_FatFs_Read(uint8_t *buff, uint32_t sector, uint32_t count);
uint8_t W25QXX_FatFs_Write(const uint8_t *buff, uint32_t sector, uint32_t count);
uint8_t W25QXX_FatFs_Ioctl(uint8_t cmd, void *buff);

#endif

