#include "nes_mapper.h"

// Mapper 225
void MAP225_Reset()
{
  // set CPU bank pointers
  //set_CPU_banks(0,1,2,3);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }
}

void MAP225_MemoryWrite(uint16_t addr, uint8_t data)
{
  uint8_t prg_bank = (addr & 0x0F80) >> 7;
  uint8_t chr_bank = addr & 0x003F;

  set_PPU_bank0(chr_bank*8+0);
  set_PPU_bank1(chr_bank*8+1);
  set_PPU_bank2(chr_bank*8+2);
  set_PPU_bank3(chr_bank*8+3);
  set_PPU_bank4(chr_bank*8+4);
  set_PPU_bank5(chr_bank*8+5);
  set_PPU_bank6(chr_bank*8+6);
  set_PPU_bank7(chr_bank*8+7);

  if(addr & 0x2000)
  {
	 set_mirroring(0,0,1,1);//ˮƽ����InfoNES_Mirroring( 0 );   //0011
	 //set_mirroring(NES_PPU::MIRROR_HORIZ);
  }
  else
  {
	 set_mirroring(0,1,0,1);//��ֱ����InfoNES_Mirroring( 1 );   //0101
	 //set_mirroring(NES_PPU::MIRROR_VERT);
  }

  if(addr & 0x1000)
  {
    // 16KBbank
    if(addr & 0x0040)
    {
      set_CPU_bank4(prg_bank*4+2);
      set_CPU_bank5(prg_bank*4+3);
      set_CPU_bank6(prg_bank*4+2);
      set_CPU_bank7(prg_bank*4+3);
    }
    else
    {
      set_CPU_bank4(prg_bank*4+0);
      set_CPU_bank5(prg_bank*4+1);
      set_CPU_bank6(prg_bank*4+0);
      set_CPU_bank7(prg_bank*4+1);
    }
  }
  else
  {
    set_CPU_bank4(prg_bank*4+0);
    set_CPU_bank5(prg_bank*4+1);
    set_CPU_bank6(prg_bank*4+2);
    set_CPU_bank7(prg_bank*4+3);
  }
}
void MAP225_Init()
{
	NES_Mapper->Reset = MAP225_Reset;
	NES_Mapper->Write=MAP225_MemoryWrite; 
}










