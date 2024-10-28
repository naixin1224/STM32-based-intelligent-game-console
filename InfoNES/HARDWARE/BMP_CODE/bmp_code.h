#ifndef _BMP_CODE_H
#define _BMP_CODE_H

#define uint8_t  unsigned char 
#define uint16_t unsigned short int 
#define uint32_t unsigned       int 

typedef struct
{
	uint16_t no;            //用于结构体对齐
	uint16_t Type;          //文件格式
	uint32_t Size;          //文件大小
	uint16_t Rd1;           //保留
	uint16_t Rd2;           //保留
	uint32_t Offset;        //从文件头到像素数据的偏移
}BMP_File;

typedef struct 
{
	uint32_t Size;            //此结构体的大小
	uint32_t Width;           //图像宽
	uint32_t Height;          //图像高
	uint16_t Planes;				  //表示bmp图片的平面属性，显然显示器只有一个平面，所以恒等于1
	uint16_t BitCount;        //图像深度，像素所占的位数，一般为24位   
	uint32_t Compression;     //说明图像数据压缩类型，0为不压缩
	uint32_t SizeImage;       //像素数据所占大小
	uint32_t XMeter;          //说明水平分辨率，用像素/米表示，一般为0
	uint32_t YMeter;          //说明垂直分辨率，用像素/米表示，一般为0
	uint32_t ClrUsed;         //说明位图实际使用的彩色表中的颜色索引数
	uint32_t ClrImportant;    //说明对图像显示有重要影响的颜色索引的数目，如果是0表示都重要
}BMP_Info;

typedef struct 
{
	BMP_File file;
	BMP_Info info;
}BMP_Head;

typedef union
{
	uint16_t rgb565;
	struct
	{
		uint16_t b:5;
		uint16_t g:6;
		uint16_t r:5;
	}work1;
	struct
	{
		uint16_t rgb_L:8;
		uint16_t rgb_H:8;
	}work2;
}RGB_565;

typedef union
{
	uint32_t rgb888;
	struct
	{
		uint32_t n1:3;
		uint32_t b:5;
		uint32_t n2:2;
		uint32_t g:6;
		uint32_t n3:3;
		uint32_t r:5;
		uint32_t n4:8;
	}work1;
	struct
	{
		uint32_t b:8;
		uint32_t g:8;
		uint32_t r:8;
		uint32_t n1:8;
	}work2;
}RGB_888;

typedef struct
{
	uint32_t width_byte;
	uint8_t  offset_byte;
}BMP_Align;

extern BMP_Head bmp_head;
extern BMP_Align bmp_align;
extern RGB_565 rgb_565;
extern RGB_888 rgb_888;

uint8_t BMP_IsAlign(BMP_Head bmp_head,BMP_Align *bmp_align);
uint16_t BMP_RGB888To565(uint8_t r,uint8_t g,uint8_t b);


#endif
