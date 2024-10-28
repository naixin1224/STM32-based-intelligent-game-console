#ifndef __WAV_H
#define __WAV_H
#include "main.h"
#include "dac.h"

#define WAV_BUFF_SIZE 1024                              //��������С

extern      uint32_t  wav_buff_play_cnt;               //�������ݼ�����
extern      uint8_t   wav_buff_play_select;            //���Ż�����ѡ��
extern      uint8_t   wav_buff_read_select;            //��ȡ��Ż�����ѡ��
extern __IO uint32_t  wav_buff_num[2];                 //(������1��������2)ÿ��ʵ�ʶ�ȡ��������
extern __IO uint8_t   wav_buff_state[2];               //(������1��������2)״̬��־λ
extern      int16_t*  wav_p_buff;                      //ָ�򻺳���ָ��
extern      float     wav_volume;                      //����

//44Byte
typedef struct
{
	//WAV Header
	uint8_t  Riff_ID[4];          //RIFF��־
	uint32_t Riff_Size;           //��¼����RIFF�ļ��Ĵ�С����Riff_ID��Riff_Size����������
	uint8_t  Riff_Format[4];      //WAVE��־
	
	//WAV FMT Chunk
	uint8_t  Fmt_ID[4];           //fmt��־
	uint32_t Fmt_Size;            //�����ʽ����,16����PCM��ʽ
	
	//WAV Format
	uint16_t FormatTag;          //������ʽ���ţ��ֿ�����
	uint16_t Channels;           //����ͨ����1��������2˫ͨ��
	uint32_t SamplesPerSec;      //������
	uint32_t AvgBytesPerSec;     //ÿ�벥�ŵ��ֽ��� = ������*������䵥λ
	uint16_t BlockAlign;         //ÿ��������ֽ��� ������䵥λ=ÿ��ȡ������λ��*����ͨ��/8
	uint16_t BitsPerSample;      //ÿ�����������λ��
	
	//WAV DATA Chunk
	uint8_t  Data_ID[4];         //data��־
	uint32_t Data_Size;          //�ļ�����
}WAV_Info_TypeDef;

typedef struct 
{
	uint16_t file_start;
	uint16_t file_end;

	uint16_t list_start;
	uint16_t list_select;
	uint16_t list_end;

	uint16_t LIST_MAX;
}WAV_ListView_TypeDef;

extern WAV_Info_TypeDef     wav_info;                               //������Ϣ

void WAV_Init(void);
void WAV_Run(void);
void WAV_Play(void);

#endif

