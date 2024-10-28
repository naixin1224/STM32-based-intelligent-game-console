#ifndef __NES_MAIN_H
#define __NES_MAIN_H
#include "main.h" 
 
#define NES_SKIP_FRAME 	2		//����ģ������֡��,Ĭ����2֡
 
#define INLINE 	static inline
#define int8 	  char
#define int16 	short
#define int32 	int
#define boolean uint8 

//nes��Ϣͷ�ṹ��
typedef struct
{
	unsigned char id[3]; // 'NES'
	unsigned char ctrl_z; // control-z
	unsigned char num_16k_rom_banks;
	unsigned char num_8k_vrom_banks;
	unsigned char flags_1;
	unsigned char flags_2;
	unsigned char reserved[8];
}NES_header;   

extern int MapperNo;			//map���
extern int NES_scanline;		//ɨ����
extern NES_header *RomHeader;	//rom�ļ�ͷ 
extern int VROM_1K_SIZE;
extern int VROM_8K_SIZE; 
extern uint8_t cpunmi;  				//cpu�жϱ�־  �� 6502.s����
extern uint8_t cpuirq;			
extern uint8_t PADdata;   			//�ֱ�1��ֵ 
extern uint8_t PADdata1;   			//�ֱ�1��ֵ 
extern uint8_t lianan_biao;			//������־ 
#define  CPU_NMI  cpunmi=1;
#define  CPU_IRQ  cpuirq=1;
#define  NES_RAM_SPEED	0 	 	//1:�ڴ�ռ��С  0:�ٶȿ�


void cpu6502_init(void);		//�� cart.s
void run6502(uint32_t); 		   		//�� 6502.s 
uint8_t nes_load_rom(uint8_t *romfile);
uint8_t nes_load(char* pname);
void nes_set_window(void);
void nes_get_gamepadval(void);
void nes_emulate_frame(void);
void debug_6502(uint16_t reg0,uint8_t reg1);
void nes_apu_fill_buffer(int samples,uint16_t* wavebuf); 
void NES_Play(void);

#endif

