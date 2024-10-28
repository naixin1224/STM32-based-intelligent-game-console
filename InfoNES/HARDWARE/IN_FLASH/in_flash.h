#ifndef _IN_FLASH_H
#define _IN_FLASH_H
#include "main.h"

#define IN_FLASH_BASE_ADDR  ((uint32_t)0x08000000)

#define IN_FLASH_SECTOR_0   ((uint32_t)0x08000000) //16k
#define IN_FLASH_SECTOR_1   ((uint32_t)0x08004000) //16k
#define IN_FLASH_SECTOR_2   ((uint32_t)0x08008000) //16k
#define IN_FLASH_SECTOR_3   ((uint32_t)0x0800c000) //16k
#define IN_FLASH_SECTOR_4   ((uint32_t)0x08010000) //64k
#define IN_FLASH_SECTOR_5   ((uint32_t)0x08020000) //128k
#define IN_FLASH_SECTOR_6   ((uint32_t)0x08040000) //128k
#define IN_FLASH_SECTOR_7   ((uint32_t)0x08060000) //128k
#define IN_FLASH_SECTOR_8   ((uint32_t)0x08080000) //128k
#define IN_FLASH_SECTOR_9   ((uint32_t)0x080a0000) //128k
#define IN_FLASH_SECTOR_10  ((uint32_t)0x080c0000) //128k
#define IN_FLASH_SECTOR_11  ((uint32_t)0x080e0000) //128k

uint8_t IN_EraseFlashSector(uint32_t sector);
uint8_t IN_WFlash(uint32_t write_addr, uint32_t *pBuff, uint32_t num);
uint8_t IN_WriteFlash(uint32_t write_addr, uint32_t *pBuff, uint32_t num);

#endif
