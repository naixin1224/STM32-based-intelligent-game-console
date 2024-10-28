#ifndef __WAV_H
#define __WAV_H
#include "main.h"
#include "dac.h"

#define WAV_BUFF_SIZE 1024                              //缓冲区大小

extern      uint32_t  wav_buff_play_cnt;               //播放数据计数器
extern      uint8_t   wav_buff_play_select;            //播放缓冲区选择
extern      uint8_t   wav_buff_read_select;            //读取存放缓冲区选择
extern __IO uint32_t  wav_buff_num[2];                 //(缓冲区1，缓冲区2)每次实际读取数据数量
extern __IO uint8_t   wav_buff_state[2];               //(缓冲区1，缓冲区2)状态标志位
extern      int16_t*  wav_p_buff;                      //指向缓冲区指针
extern      float     wav_volume;                      //音量

//44Byte
typedef struct
{
	//WAV Header
	uint8_t  Riff_ID[4];          //RIFF标志
	uint32_t Riff_Size;           //记录整个RIFF文件的大小，除Riff_ID和Riff_Size这两个变量
	uint8_t  Riff_Format[4];      //WAVE标志
	
	//WAV FMT Chunk
	uint8_t  Fmt_ID[4];           //fmt标志
	uint32_t Fmt_Size;            //编码格式类型,16代表PCM形式
	
	//WAV Format
	uint16_t FormatTag;          //声音格式代号，字块总数
	uint16_t Channels;           //声音通道，1单声道，2双通道
	uint32_t SamplesPerSec;      //采样率
	uint32_t AvgBytesPerSec;     //每秒播放的字节数 = 采样率*区块对其单位
	uint16_t BlockAlign;         //每个样点的字节数 区块对其单位=每个取样所需位数*声音通道/8
	uint16_t BitsPerSample;      //每个样点的数据位数
	
	//WAV DATA Chunk
	uint8_t  Data_ID[4];         //data标志
	uint32_t Data_Size;          //文件长度
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

extern WAV_Info_TypeDef     wav_info;                               //音乐信息

void WAV_Init(void);
void WAV_Run(void);
void WAV_Play(void);

#endif

