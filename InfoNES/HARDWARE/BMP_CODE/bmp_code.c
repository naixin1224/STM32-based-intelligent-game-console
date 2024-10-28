#include "bmp_code.h"

BMP_Head bmp_head;
BMP_Align bmp_align;
RGB_565 rgb_565;
RGB_888 rgb_888;

uint8_t BMP_IsAlign(BMP_Head bmp_head,BMP_Align *bmp_align)
{
	uint8_t byte;
	byte = (bmp_head.info.Width*bmp_head.info.BitCount/8)%4;
	if(byte != 0)
	{
		bmp_align->offset_byte = 4;
		bmp_align->width_byte = ((bmp_head.info.Width * (bmp_head.info.BitCount/8)) / 4) * 4;
		return 0;
	}else
	{
		return 1;
	}
}

uint16_t BMP_RGB888To565(uint8_t r,uint8_t g,uint8_t b)
{
	rgb_888.work2.r = r;
	rgb_888.work2.g = g;
	rgb_888.work2.b = b;
	
	rgb_565.work1.r = rgb_888.work1.r;
	rgb_565.work1.g = rgb_888.work1.g;
	rgb_565.work1.b = rgb_888.work1.b;
	
	return rgb_565.rgb565;
}

