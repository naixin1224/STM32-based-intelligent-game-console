/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25qxx.h" 

/* Definitions of physical drive number for each drive */
#define DEV_FLASH		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_SD		  1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		  2	/* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	
	switch (pdrv) {
	case DEV_FLASH :
		stat = W25QXX_FatFs_Init();
		break;
	case DEV_SD :
		break;
	case DEV_USB :
		break;
	default :
		break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) 
	{
		case DEV_FLASH:
			stat = W25QXX_FatFs_Status();
			break;
		case DEV_SD:
			break;
		case DEV_USB:
			break;
		default :
			break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_FLASH :
		res = (DRESULT)W25QXX_FatFs_Read(buff,sector,count);
		break;
	case DEV_SD :
		break;
	case DEV_USB :
		break;
	default :
		break;
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_FLASH :
		res = (DRESULT)W25QXX_FatFs_Write(buff, sector, count);
		break;
	case DEV_SD :
		break;
	case DEV_USB :
		break;
	default :
		break;
	}

	return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) 
	{
		case DEV_FLASH :
			res = (DRESULT)W25QXX_FatFs_Ioctl(cmd, buff);
			break;
		case DEV_SD :
			break;
		case DEV_USB :
			break;
		default:
			break;
	}
	return res;
}

DWORD get_fattime (void)
{				 
	return ((2023-1980)<<25) | (8<<21) | (15<<16) | (23<<11) | (1<<5) | (26/2);
}			 

