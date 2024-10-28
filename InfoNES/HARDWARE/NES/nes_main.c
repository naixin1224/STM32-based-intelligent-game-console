#include "NES_main.h" 
#include "NES_ppu.h"
#include "NES_mapper.h"
#include "NES_apu.h"

#include <string.h>
#include <stdlib.h>

#include "ff.h"
#include "st7789.h"
#include "key.h"
#include "in_flash.h"
#include "timer.h"
#include "dac.h"

#define CCMRAM __attribute__((section("ccmram")))//stm32f407 CCMRAM 64K 
 
int      MapperNo;			  //map���
int      NES_scanline;		//NESɨ����
int      VROM_1K_SIZE;
int      VROM_8K_SIZE;

uint8_t  PADdata0;   			//�ֱ�1��ֵ [7:0]��7 ��6 ��5 ��4 Start3 Select2 B1 A0  
uint8_t  PADdata1;   			//�ֱ�2��ֵ [7:0]��7 ��6 ��5 ��4 Start3 Select2 B1 A0 

uint8_t *NES_RAM;			    //����1024�ֽڶ���
__align(1024) uint8_t NES_RAM1[2048] __attribute__((at(0x20000800)));// ����1024�ֽڶ���
uint8_t *NES_SRAM;
uint8_t  NES_SRAM1[0X2000];
NES_header    *RomHeader; 	//rom�ļ�ͷ
NES_header     RomHeader1;
MAPPER        *NES_Mapper;	
MAPPER         NES_Mapper1;
MapperCommRes *MAPx;  
MapperCommRes  MAPx1;
uint8_t       *spr_ram;		      //����RAM,256�ֽ�
uint8_t        spr_ram1[256];
ppu_data      *ppu;			  //ppuָ��
ppu_data       ppu1;
apu_t         *apu; 			    //apuָ��
apu_t          apu1;

uint16_t*          wave_buffers;
uint16_t           wave_buffers11[735];

uint32_t         	 nes_sound_buff_cnt         = 0;   //�������ݼ�����
uint8_t          	 nes_sound_buff_read_select = 0;   //��������ѡ��
uint8_t          	 nes_sound_buff_play_select = 0;   //���Ż�����ѡ��
__IO uint32_t    	 nes_sound_buff_num[2]      = {0};         //(������1��������2)ÿ��ʵ�ʶ�ȡ��������
__IO uint8_t     	 nes_sound_buff_state[2]    = {0};         //(������1��������2)״̬��־λ
int32_t            nes_sound_dat;
int16_t*           nes_sound_buff;
uint16_t           nes_sound_buffs[2][735]; 			//��Ƶ����֡,ռ���ڴ��� 367*4 �ֽ�@22050Hz

uint8_t       *VROM_banks;
uint8_t       *VROM_tiles;
CCMRAM uint8_t        VROM[1024*50];// __attribute__((at(0x20000300)));

uint8_t* romfile;			//NES�ļ�ָ��,ָ������NES�ļ�����ʼ��ַ.
uint8_t system_task_return=0;		//����ǿ�Ʒ��ر�־.

uint8_t nes_load_rom(uint8_t *rom)
{
	uint32_t i;
	uint8_t *p  = rom;
	
	if(RomHeader->flags_1 & 0x04)
	{
		p += (512);		                          //��512�ֽڵ�trainer:
	}
	if(RomHeader->num_8k_vrom_banks > 0)		//����VROM,����Ԥ����
	{
		VROM_banks = p + 16 + (RomHeader->num_16k_rom_banks * 0x4000);
		VROM_tiles = &VROM[0];
		compile(RomHeader->num_8k_vrom_banks*8*1024/16,VROM_banks,VROM_tiles);  
	}else 
	{
		VROM_banks = &VROM[0];
		VROM_tiles = &VROM[1024*8];
	}  	
	VROM_1K_SIZE = RomHeader->num_8k_vrom_banks * 8;
	VROM_8K_SIZE = RomHeader->num_8k_vrom_banks;  
	MapperNo=(RomHeader->flags_1>>4)|(RomHeader->flags_2&0xf0);
	if(RomHeader->flags_2 & 0x0E)MapperNo=RomHeader->flags_1>>4;//���Ը���λ�����ͷ����������� 

	for(i = 0; i < 255; i++)  // ����֧�ֵ�Mapper��
	{		
		if (MapTab[i]==MapperNo)break;		
		if (MapTab[i]==-1)return 2; 
	} 
	switch(MapperNo)
	{
		case 1:  
			MAP1 = &MAP11;
			break;
		case 4:  
		case 6: 
		case 16:
		case 17:
		case 18:
		case 19:
		case 21: 
		case 23:
		case 24:
		case 25:
		case 64:
		case 65:
		case 67:
		case 69:
		case 85:
		case 189:
			MAPx = &MAPx1;
			break;  
		default:
			break;
	}
	return 0;	
}

//���ò����ʵĲ���ʱ��
void NES_SetPlay_SamplesPerSec(uint32_t samples)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 168-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1*1000000/samples;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

uint8_t nes_load(char* pname)
{
	FIL fil;
	uint32_t pbuff[128] = {0};
	uint32_t br;
	uint32_t write_addr = IN_FLASH_SECTOR_6;
	uint32_t i;
	uint32_t temp;
	uint32_t size;
	
	nes_sound_buff = (int16_t*)&nes_sound_buffs[0][0];
	NES_SetPlay_SamplesPerSec(22050);//����22050�������������
	
	NES_RAM      = &NES_RAM1[0];                 //2k
	NES_SRAM     = &NES_SRAM1[0];                //8k
	RomHeader    = &RomHeader1;                  //16
	NES_Mapper   = &NES_Mapper1;                 //
	spr_ram      = &spr_ram1[0];                 //256
	ppu          = &ppu1;                        //
	apu          = &apu1;                        //
	wave_buffers = &wave_buffers11[0];           //
	romfile      = (uint8_t*)IN_FLASH_SECTOR_6;
	
	memset(NES_RAM,      0, sizeof(NES_RAM1));
	memset(NES_SRAM,     0, sizeof(NES_SRAM1));
	memset(RomHeader,    0, sizeof(RomHeader1));
	memset(NES_Mapper,   0, sizeof(NES_Mapper1));
	memset(spr_ram,      0, sizeof(spr_ram1));
	memset(ppu,          0, sizeof(ppu1));
	memset(apu,          0, sizeof(apu1));
	memset(wave_buffers, 0, sizeof(wave_buffers11));
	
	/**********************************��ȡROM��flash************************************************/
	if(f_open(&fil,(const TCHAR*)pname, FA_READ) == FR_OK)
	{      
		f_read(&fil,RomHeader,16,&br);
		if((RomHeader->id[0]!= 0x4e)||(RomHeader->id[1]!= 0x45)||(RomHeader->id[2]!= 0x53))
		{
			return 1;  
		}
		size = 16+ (RomHeader->num_16k_rom_banks * 0x4000) + (RomHeader->num_8k_vrom_banks * 0x2000);
		if(size > 128*1024)
		{
			IN_EraseFlashSector(IN_FLASH_SECTOR_6);
			IN_EraseFlashSector(IN_FLASH_SECTOR_7);
		}else
		{
			IN_EraseFlashSector(IN_FLASH_SECTOR_6);
		}
		f_lseek(&fil,0);
		for(i = 0; i < (size/512); i++)
		{
			f_read(&fil,pbuff,512,&br);
			if(!br)
			{
				return 2;
			}
			if(IN_WFlash(write_addr,(uint32_t*)pbuff,br/4) != 0)
			{
				return 3;
			}
			write_addr = write_addr + br;
		}
		temp = size % 512; 
		if(temp)
		{
			f_read(&fil,pbuff,temp,&br);
			if(!br)
			{
				return 4;
			}
			if(IN_WFlash(write_addr,(uint32_t*)pbuff,br/4) != 0)
			{
				return 5;
			}
		}
	}
	f_close(&fil);

	nes_load_rom(romfile);
	cpu6502_init();						          //��ʼ��6502,����λ	 
	Mapper_Init();						          //map��ʼ�� 	 
	PPU_reset();						            //ppu��λ
	apu_init(); 						            //apu��ʼ��
	nes_emulate_frame();				        //����NESģ������ѭ�� 
	
	return 0;
}

//������Ϸ��ʾ����
void NES_set_window(void)
{
	LCD_SetWindows(32,0,288-1,240-1);
	LCD_CS_L;
	LCD_DC_H;
}
extern void KEYBRD_FCPAD_Decode(uint8_t *fcbuf,uint8_t mode);

//��ȡ��Ϸ�ֱ�����
void NES_get_gamepadval(void)
{
	PADdata0 = 0;  //7(��) 6(��) 5(��) 4(��) 3(��ʼ) 2(ѡ��) 1(B) 0(A)
	
	if(!KEY_UP)     PADdata0 |= 0x10;
	if(!KEY_DOWN)   PADdata0 |= 0x20;
	if(!KEY_LEFT)   PADdata0 |= 0x40;
	if(!KEY_RIGH)   PADdata0 |= 0x80;
	if(!KEY_START)  PADdata0 |= 0x08;
	if(!KEY_SELECT) PADdata0 |= 0x04;
	if(!KEY_B)      PADdata0 |= 0x02;
	if(!KEY_A)      PADdata0 |= 0x01;   
	if(!KEY_BACK)   system_task_return = 1;
}

//NESģ������ѭ��
void nes_emulate_frame(void)
{
	uint8_t NES_frame=0;
	system_task_return = 0;
	NES_set_window();//���ô���
	while(1)
	{	
		// LINES 0-239
		PPU_start_frame();
		for(NES_scanline = 0; NES_scanline< 240; NES_scanline++)
		{
			run6502(113*256);
			NES_Mapper->HSync(NES_scanline);
			//ɨ��һ��		  
			if(NES_frame==0)scanline_draw(NES_scanline);
			else do_scanline_and_dont_draw(NES_scanline); 
		}  
		NES_scanline=240;
		run6502(113*256);//����1��
		NES_Mapper->HSync(NES_scanline); 
		start_vblank(); 
		if(NMI_enabled()) 
		{
			cpunmi=1;
			run6502(7*256);//�����ж�
		}
		NES_Mapper->VSync();
		// LINES 242-261    
		for(NES_scanline=241; NES_scanline<262; NES_scanline++)
		{
			run6502(113*256);	  
			NES_Mapper->HSync(NES_scanline);		  
		}
		end_vblank();
		NES_get_gamepadval();       //ÿ3֡��ѯһ��USB
		apu_soundoutput();          //�����Ϸ����	 
		NES_frame++;
		
		if(NES_frame>NES_SKIP_FRAME)
		{
			NES_frame=0;          //��֡ 
		}
		if(system_task_return) 
		{
			break;                //TPAD����  
		}
	}
}

//��6502.s���汻����
void debug_6502(uint16_t reg0,uint8_t reg1)
{
	
}

//NES��Ƶ�����I2S����
void nes_apu_fill_buffer(int samples,uint16_t* wavebuf)
{
	uint16_t i;
	
	while(nes_sound_buff_state[nes_sound_buff_read_select] == 1);
	for(i = 0; i < APU_PCMBUF_SIZE; i++)
	{
		nes_sound_buffs[nes_sound_buff_read_select][i]   = wavebuf[i];
	}
	nes_sound_buff_num[nes_sound_buff_read_select] = APU_PCMBUF_SIZE;
	nes_sound_buff_state[nes_sound_buff_read_select] = 1;
	
	if(nes_sound_buff_read_select == 0)
	{
		nes_sound_buff_read_select = 1;
	}else
	{
		nes_sound_buff_read_select = 0;
	}
}

//���ж������Զ�������
void NES_Play(void)
{
	if(nes_sound_buff_state[nes_sound_buff_play_select] == 1)
	{	
		nes_sound_dat = *(nes_sound_buff + nes_sound_buff_cnt);
		nes_sound_dat = (nes_sound_dat+0x7fff) >> 7;
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,nes_sound_dat);
		
		nes_sound_buff_cnt += 1;//ÿ��ȡ�����±�ƫ����
		if(nes_sound_buff_cnt >= nes_sound_buff_num[nes_sound_buff_play_select])
		{
			nes_sound_buff_cnt = 0;
			nes_sound_buff_state[nes_sound_buff_play_select] = 0;
			nes_sound_buff_play_select++;
			if(nes_sound_buff_play_select >= 2)
			{
				nes_sound_buff_play_select = 0;
			}
			nes_sound_buff = (int16_t*)&nes_sound_buffs[nes_sound_buff_play_select][0];
		}
	}
}


