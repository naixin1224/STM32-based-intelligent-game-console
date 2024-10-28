#include "in_flash.h"

uint8_t IN_GetFlashSector(uint32_t addr)
{
	     if(addr < IN_FLASH_SECTOR_1)   return FLASH_SECTOR_0;
	else if(addr < IN_FLASH_SECTOR_2)   return FLASH_SECTOR_1;
	else if(addr < IN_FLASH_SECTOR_3)   return FLASH_SECTOR_2;
	else if(addr < IN_FLASH_SECTOR_4)   return FLASH_SECTOR_3;
	else if(addr < IN_FLASH_SECTOR_5)   return FLASH_SECTOR_4;
	else if(addr < IN_FLASH_SECTOR_6)   return FLASH_SECTOR_5;
	else if(addr < IN_FLASH_SECTOR_7)   return FLASH_SECTOR_6;
	else if(addr < IN_FLASH_SECTOR_8)   return FLASH_SECTOR_7;
	else if(addr < IN_FLASH_SECTOR_9)   return FLASH_SECTOR_8;
	else if(addr < IN_FLASH_SECTOR_10)  return FLASH_SECTOR_9;
	else if(addr < IN_FLASH_SECTOR_11)  return FLASH_SECTOR_10;
	else                                return FLASH_SECTOR_11;
}

uint8_t IN_EraseFlashSector(uint32_t sector)
{
	uint32_t SectorError;
	FLASH_EraseInitTypeDef EraseInit;
	
	HAL_FLASH_Unlock();  //�ڲ�falsh����
	
	EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;      //������������
	EraseInit.Sector = IN_GetFlashSector(sector);       //��������
	EraseInit.NbSectors = 1;                            //һ��ֻ����һ������
	EraseInit.Banks = 1;                                //��1
	EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;     //��ѹ��Χ2.7V-3.6V
	if(HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK)
	{
		return 1;
	}
	FLASH_WaitForLastOperation(50000);
	
	HAL_FLASH_Lock();               //�ڲ�falsh����
	
	return 0;
}

uint8_t IN_WFlash(uint32_t write_addr, uint32_t *pBuff, uint32_t num)
{
	uint32_t end_addr = write_addr + (num * 4);
	
	if((num == 0) || (write_addr < IN_FLASH_BASE_ADDR) || (write_addr % 4))
	{
		return 1;
	}
	
	HAL_FLASH_Unlock();  //�ڲ�falsh����
	
	while(write_addr < end_addr)
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, *pBuff) != HAL_OK)
		{
			return 2;
		}
		write_addr = write_addr + 4;
		pBuff++;
	}
	
	HAL_FLASH_Lock();    //�ڲ�falsh����
	return 0;
}

uint8_t IN_WriteFlash(uint32_t write_addr, uint32_t *pBuff, uint32_t num)
{
	uint32_t SectorError;
	FLASH_EraseInitTypeDef EraseInit;
	
	uint32_t start_addr = write_addr;
	uint32_t end_addr = write_addr + (num * 4);
	
	if((num == 0) || (write_addr < IN_FLASH_BASE_ADDR) || (write_addr % 4))
	{
		return 1;
	}
	
	HAL_FLASH_Unlock();  //�ڲ�falsh����
	
	while(start_addr < end_addr)
	{
		if(*(__IO uint32_t*)start_addr != 0xffffffff)         //��ȡ���ݲ���0xffffffff�Ͳ���
		{
			EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;      //������������
			EraseInit.Sector = IN_GetFlashSector(start_addr);   //��������
			EraseInit.NbSectors = 1;                            //һ��ֻ����һ������
			EraseInit.Banks = 1;                                //��1
			EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;     //��ѹ��Χ2.7V-3.6V
			if(HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK)
			{
				return 2;
			}
		}else
		{
			start_addr = start_addr + 4;
		}
		FLASH_WaitForLastOperation(50000);
	}
	
	if(FLASH_WaitForLastOperation(50000) == HAL_OK)
	{
		while(write_addr < end_addr)
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, *pBuff) != HAL_OK)
			{
				return 3;
			}
			write_addr = write_addr + 4;
			pBuff++;
		}
	}
	
	HAL_FLASH_Lock();    //�ڲ�falsh����
	
	return 0;
}
