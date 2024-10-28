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
	
	HAL_FLASH_Unlock();  //内部falsh解锁
	
	EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;      //擦除类型扇区
	EraseInit.Sector = IN_GetFlashSector(sector);       //擦除扇区
	EraseInit.NbSectors = 1;                            //一次只擦除一个扇区
	EraseInit.Banks = 1;                                //块1
	EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;     //电压范围2.7V-3.6V
	if(HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK)
	{
		return 1;
	}
	FLASH_WaitForLastOperation(50000);
	
	HAL_FLASH_Lock();               //内部falsh上锁
	
	return 0;
}

uint8_t IN_WFlash(uint32_t write_addr, uint32_t *pBuff, uint32_t num)
{
	uint32_t end_addr = write_addr + (num * 4);
	
	if((num == 0) || (write_addr < IN_FLASH_BASE_ADDR) || (write_addr % 4))
	{
		return 1;
	}
	
	HAL_FLASH_Unlock();  //内部falsh解锁
	
	while(write_addr < end_addr)
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, *pBuff) != HAL_OK)
		{
			return 2;
		}
		write_addr = write_addr + 4;
		pBuff++;
	}
	
	HAL_FLASH_Lock();    //内部falsh上锁
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
	
	HAL_FLASH_Unlock();  //内部falsh解锁
	
	while(start_addr < end_addr)
	{
		if(*(__IO uint32_t*)start_addr != 0xffffffff)         //读取数据不是0xffffffff就擦除
		{
			EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;      //擦除类型扇区
			EraseInit.Sector = IN_GetFlashSector(start_addr);   //擦除扇区
			EraseInit.NbSectors = 1;                            //一次只擦除一个扇区
			EraseInit.Banks = 1;                                //块1
			EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;     //电压范围2.7V-3.6V
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
	
	HAL_FLASH_Lock();    //内部falsh上锁
	
	return 0;
}
