#ifndef _BMP_CODE_H
#define _BMP_CODE_H

#define uint8_t  unsigned char 
#define uint16_t unsigned short int 
#define uint32_t unsigned       int 

typedef struct
{
	uint16_t no;            //���ڽṹ�����
	uint16_t Type;          //�ļ���ʽ
	uint32_t Size;          //�ļ���С
	uint16_t Rd1;           //����
	uint16_t Rd2;           //����
	uint32_t Offset;        //���ļ�ͷ���������ݵ�ƫ��
}BMP_File;

typedef struct 
{
	uint32_t Size;            //�˽ṹ��Ĵ�С
	uint32_t Width;           //ͼ���
	uint32_t Height;          //ͼ���
	uint16_t Planes;				  //��ʾbmpͼƬ��ƽ�����ԣ���Ȼ��ʾ��ֻ��һ��ƽ�棬���Ժ����1
	uint16_t BitCount;        //ͼ����ȣ�������ռ��λ����һ��Ϊ24λ   
	uint32_t Compression;     //˵��ͼ������ѹ�����ͣ�0Ϊ��ѹ��
	uint32_t SizeImage;       //����������ռ��С
	uint32_t XMeter;          //˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0
	uint32_t YMeter;          //˵����ֱ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0
	uint32_t ClrUsed;         //˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
	uint32_t ClrImportant;    //˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0��ʾ����Ҫ
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
