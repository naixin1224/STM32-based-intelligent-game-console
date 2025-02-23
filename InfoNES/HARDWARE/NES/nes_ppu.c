#include "nes_ppu.h"
#include "string.h"
#include "st7789.h"
 
uint8_t* PPU_patterntables;       //8192  VROM开始地址 图案表 

void set_tile_banks(uint8_t *bank0, uint8_t *bank1, uint8_t *bank2, uint8_t *bank3,
      uint8_t *bank4, uint8_t *bank5, uint8_t *bank6, uint8_t *bank7)
{
	ppu->PPU_tile_banks[0] = bank0;
	ppu->PPU_tile_banks[1] = bank1;
	ppu->PPU_tile_banks[2] = bank2;
	ppu->PPU_tile_banks[3] = bank3;
	ppu->PPU_tile_banks[4] = bank4;
	ppu->PPU_tile_banks[5] = bank5;
	ppu->PPU_tile_banks[6] = bank6;
	ppu->PPU_tile_banks[7] = bank7;
}
//*******************************************************************
  void set_tile_bank(int i, uint8_t *bank)
  {
	  ppu->PPU_tile_banks[i] = bank;	  
  }
   
  void set_name_table(uint8_t bank, int bank_num)
  {
	  ppu->PPU_VRAM_banks[bank] = ppu->PPU_nametables + ((bank_num & 0x03) << 10);
  }
  // 0x2000
  uint32_t NMI_enabled(void)  { return ppu->LowRegs[0] & 0x80; }
  uint32_t sprites_8x16() { return ppu->LowRegs[0] & 0x20; }

  // 0x2001
  uint32_t spr_enabled(void)    { return ppu->LowRegs[1] & 0x10; }
  uint32_t bg_enabled(void)     { return ppu->LowRegs[1] & 0x08; }
  uint32_t spr_clip_left8() { return !(ppu->LowRegs[1] & 0x04); }
  uint32_t bg_clip_left8()  { return !(ppu->LowRegs[1] & 0x02); }
  uint32_t rgb_pal()        { return ppu->LowRegs[1] & 0xE0;}
//  uint8_t  rgb_bak;

  // 0x2002
  uint32_t sprite0_hit()                     { return ppu->LowRegs[2] & 0x40; }
  uint32_t more_than_8_sprites_on_cur_line() { return ppu->LowRegs[2] & 0x20; }
  uint32_t VRAM_accessible()                 { return ppu->LowRegs[2] & 0x10; }

  // by rinao
//  uint8_t* get_patt() { return PPU_patterntables; }
//  uint8_t* get_namt() { return ppu->PPU_nametables; }
//  uint8_t get_pattype(uint8_t bank) { return PPU_patterntype[bank]; }
//  void set_pattype(uint8_t bank, uint8_t data) { PPU_patterntype[bank] = data; }
  
  //*****************************************************************

//  ROM_banks = (uint8_t*)malloc(header.num_16k_rom_banks * (16*1024));
//  VROM_banks = (uint8_t*)malloc(header.num_8k_vrom_banks * (8*1024));
//  VROM_tiles = (uint8_t *)malloc(RomHeader->num_8k_vrom_banks * 8 * 1024 / 16 * BYTES_PER_COMPILED_TILE);
//  compile(header.num_8k_vrom_banks * 8 * 1024 / 16, VROM_banks, VROM_tiles);
  
//  uint8_t* get_ROM_banks()  { return ROM_banks;   }
//  uint8_t* get_VROM_banks() { return VROM_banks;  }
//  uint8_t* get_VROM_tiles() { return VROM_tiles;  }
  
// #define MASK_BANK(bank,mask) (bank) = ((bank) & (mask)) 
// #define VALIDATE_VROM_BANK(bank) \
//    MASK_BANK(bank,VROM_mask); \
//    if((bank) >= (RomHeader->num_8k_vrom_banks * 8)) return; 

//****************************************************************************************************

#define EXTRACT_4_PIXELS() \
	col = 0; \
	if(pattern_lo & pattern_mask) col |= (0x01 << 6); \
	if(pattern_hi & pattern_mask) col |= (0x02 << 6); \
	pattern_mask >>= 1; \
    if(pattern_lo & pattern_mask) col |= (0x01 << 4); \
	if(pattern_hi & pattern_mask) col |= (0x02 << 4); \
	pattern_mask >>= 1; \
    if(pattern_lo & pattern_mask) col |= (0x01 << 2); \
	if(pattern_hi & pattern_mask) col |= (0x02 << 2); \
	pattern_mask >>= 1; \
    if(pattern_lo & pattern_mask) col |= (0x01 << 0); \
	if(pattern_hi & pattern_mask) col |= (0x02 << 0); \
	*p++= col;

void compile1(int count, uint8_t *src, uint8_t *dest)
{
    uint8_t *p = dest;
    uint8_t col;
	uint8_t pattern_lo;
	uint8_t pattern_hi;
    int i,line,pattern_mask;
    for (i = 0; i < count; i++) {
        for (line = 0; line < 8; line++) {
            pattern_lo = *src;
            pattern_hi = *(src + 8);

            pattern_mask = 0x80; 
			EXTRACT_4_PIXELS();
			pattern_mask >>= 1;
			EXTRACT_4_PIXELS(); 
            src++;
        }
		src += 8;
    }
}
void compile(int count, uint8_t *src, uint8_t *dest)
{
	uint8_t destemp[16];
	uint8_t *p = destemp; 
	uint8_t col;
	uint8_t pattern_lo;
	uint8_t pattern_hi;
  int i,line,pattern_mask;
	uint8_t j;
  for (i = 0; i < count; i++) 
	{
    for (line = 0; line < 8; line++) 
		{
			pattern_lo = *src;
			pattern_hi = *(src + 8);
			pattern_mask = 0x80;  
			EXTRACT_4_PIXELS();
			pattern_mask >>= 1;
			EXTRACT_4_PIXELS(); 
			src++;
    }
		p=destemp;//重新指向数组首地址
		for(j=0;j<16;j++)*dest++=destemp[j];
		src += 8;
  }
}
#define UPDATE_PIXEL() \
  if(data & pattern_mask) *p |= bit; \
  p++;


#define VRAM(addr) \
  ppu->PPU_VRAM_banks[(addr) >> 10][(addr) & 0x3FF]

#define TILE(addr) \
  (ppu->PPU_tile_banks[(addr) >> 10] + ((addr) & 0x3FF) / 16 * 16)

#define TILE_OFFSET(line) \
  ((line) * 2)

/*
scanline start (if background or sprites are enabled):
	v:0000010000011111=t:0000010000011111
*/
#define LOOPY_SCANLINE_START(v,t) \
  { \
    v = (v & 0xFBE0) | (t & 0x041F); \
  }


#define LOOPY_NEXT_LINE(v) \
  { \
    if((v & 0x7000) == 0x7000) /* is subtile y offset == 7? */ \
    { \
      v &= 0x8FFF; /* subtile y offset = 0 */ \
      if((v & 0x03E0) == 0x03A0) /* name_tab line == 29? */ \
      { \
        v ^= 0x0800;  /* switch nametables (bit 11) */ \
        v &= 0xFC1F;  /* name_tab line = 0 */ \
      } \
      else \
      { \
        if((v & 0x03E0) == 0x03E0) /* line == 31? */ \
        { \
          v &= 0xFC1F;  /* name_tab line = 0 */ \
        } \
        else \
        { \
          v += 0x0020; \
        } \
      } \
    } \
    else \
    { \
      v += 0x1000; /* next subtile y offset */ \
    } \
  }


#define LOOPY_NEXT_TILE(v) \
  { \
    if((v & 0x001F) == 0x001F) \
    { \
      v ^= 0x0400; /* switch nametables (bit 10) */ \
      v &= 0xFFE0; /* tile x = 0 */ \
    } \
    else \
    { \
      v++; /* next tile */ \
    } \
  }

#define LOOPY_NEXT_PIXEL(v,x) \
  { \
    if(x == 0x07) \
    { \
      LOOPY_NEXT_TILE(v); \
      x = 0x00; \
    } \
    else \
    { \
      x++; \
    } \
  }
void PPU_Latch_FDFE(uint32_t addr) {}
#define CHECK_MMC2(addr) \
  if(((addr) & 0x0FC0) == 0x0FC0) \
  { \
    if((((addr) & 0x0FF0) == 0x0FD0) || (((addr) & 0x0FF0) == 0x0FE0)) \
    { \
      PPU_Latch_FDFE(addr); \
    } \
  }


void PPU_reset(void)
{
  // reset registers
/* memset(LowRegs, 0x00, sizeof(LowRegs));
  HighReg0x4014 = 0x00;

  // clear sprite RAM
  memset(ppu->spr_ram, 0x00, sizeof(ppu->spr_ram));

  // clear palettes
  memset(bg_pal,  0x00, sizeof(bg_pal));
  memset(spr_pal, 0x00, sizeof(spr_pal));

  // clear solid buffer
  memset(ppu->solid_buf, 0x00, sizeof(ppu->solid_buf));

  // clear pattern tables
  
//  memset(PPU_patterntables, 0x00, sizeof(PPU_patterntables));
  memset(PPU_patterntype, 0x00, sizeof(PPU_patterntype));

  memset(ppu->PPU_tile_tables, 0, sizeof(ppu->PPU_tile_tables));

  // clear internal name tables
  memset(ppu->PPU_nametables, 0x00, sizeof(ppu->PPU_nametables));

  // clear VRAM page table
  memset(ppu->PPU_VRAM_banks, 0x00, sizeof(ppu->PPU_VRAM_banks));

  memset(ppu->PPU_tile_banks, 0, sizeof(ppu->PPU_tile_banks));*/

  // set up PPU memory space table
  PPU_patterntables=VROM_banks;//不确定正确
	
  ppu->PPU_VRAM_banks[0x00] = PPU_patterntables + (0*0x400);
  ppu->PPU_VRAM_banks[0x01] = PPU_patterntables + (1*0x400);
  ppu->PPU_VRAM_banks[0x02] = PPU_patterntables + (2*0x400);
  ppu->PPU_VRAM_banks[0x03] = PPU_patterntables + (3*0x400);
    
  ppu->PPU_VRAM_banks[0x04] = PPU_patterntables + (4*0x400);
  ppu->PPU_VRAM_banks[0x05] = PPU_patterntables + (5*0x400);
  ppu->PPU_VRAM_banks[0x06] = PPU_patterntables + (6*0x400);
  ppu->PPU_VRAM_banks[0x07] = PPU_patterntables + (7*0x400);

  // point nametables at internal name table 0
  ppu->PPU_VRAM_banks[0x08] = ppu->PPU_nametables;
  ppu->PPU_VRAM_banks[0x09] = ppu->PPU_nametables;
  ppu->PPU_VRAM_banks[0x0A] = ppu->PPU_nametables;
  ppu->PPU_VRAM_banks[0x0B] = ppu->PPU_nametables;
  
  ppu->PPU_tile_tables=VROM_tiles;//不确定正确
                                                     //BYTES_PER_COMPILED_TILE=16
  ppu->PPU_tile_banks[0x00] = ppu->PPU_tile_tables + (0*0x400);
  ppu->PPU_tile_banks[0x01] = ppu->PPU_tile_tables + (1*0x400);
  ppu->PPU_tile_banks[0x02] = ppu->PPU_tile_tables + (2*0x400);
  ppu->PPU_tile_banks[0x03] = ppu->PPU_tile_tables + (3*0x400);
  ppu->PPU_tile_banks[0x04] = ppu->PPU_tile_tables + (4*0x400);
  ppu->PPU_tile_banks[0x05] = ppu->PPU_tile_tables + (5*0x400);
  ppu->PPU_tile_banks[0x06] = ppu->PPU_tile_tables + (6*0x400);
  ppu->PPU_tile_banks[0x07] = ppu->PPU_tile_tables + (7*0x400);

  ppu->read_2007_buffer = 0x00;
  ppu->in_vblank = 0;
  ppu->bg_pattern_table_addr = 0;
  ppu->spr_pattern_table_addr = 0;
  ppu->ppu_addr_inc = 0;
  ppu->loopy_v = 0;
  ppu->loopy_t = 0;
  ppu->loopy_x = 0;
  ppu->toggle_2005_2006 = 0;
  ppu->spr_ram_rw_ptr = 0;
  ppu->read_2007_buffer = 0;
  ppu->current_frame_line = 0;
              
  if(RomHeader->flags_1 &0x01)set_mirroring(0,1,0,1);//垂直镜像	//  PPU_set_mirroring(); 设置镜像
  else set_mirroring(0,0,1,1);//水平镜像

}
//***********************************************************************************************
void set_mirroring(uint32_t nt0, uint32_t nt1, uint32_t nt2, uint32_t nt3)//设置垂直水平镜像
{
  ppu->PPU_VRAM_banks[0x08] = ppu->PPU_nametables + (nt0 << 10); // * 0x0400
  ppu->PPU_VRAM_banks[0x09] = ppu->PPU_nametables + (nt1 << 10);
  ppu->PPU_VRAM_banks[0x0A] = ppu->PPU_nametables + (nt2 << 10);
  ppu->PPU_VRAM_banks[0x0B] = ppu->PPU_nametables + (nt3 << 10);
//  name_table_switched = TRUE;
}

void PPU_start_frame(void)
{
  ppu->current_frame_line = 0;

  if(spr_enabled() || bg_enabled())
  {
    ppu->loopy_v = ppu->loopy_t;
  }
}

uint8_t getBGColor() { return ppu->bg_pal[0]; }

void do_scanline_and_draw(uint8_t* buf)
{
  uint16_t i;
	if(!bg_enabled())
  {
    // set to background color设置背景颜色
//    memset(buf, bg_pal[0], NES_BACKBUF_WIDTH);//NES_BACKBUF_WIDTH=256+(2*8)
	  for(i=0;i<(256+16);i++)
	  {
		  buf[i]=ppu->bg_pal[0];
	  }
	  
  }
  if(spr_enabled() || bg_enabled())  
  {
    LOOPY_SCANLINE_START(ppu->loopy_v, ppu->loopy_t);

    if(bg_enabled())
    {
      // draw background画背景
      render_bg(buf);
    }
    else
    {
      // clear out solid buffer清除固体缓冲区
     memset(ppu->solid_buf, 0x00, sizeof(ppu->solid_buf));
    }

    if(spr_enabled())
    {
      // draw sprites绘制精灵
      render_spr(buf);
    }

    LOOPY_NEXT_LINE(ppu->loopy_v);
  }
  ppu->current_frame_line++;  
}

uint16_t draw_line=0;
uint32_t draw_color_cnt=0; 
uint16_t draw_color[512*32] = {0};
void scanline_draw(int LineNo)
{
	uint16_t i; 
	do_scanline_and_draw(ppu->dummy_buffer);	
	draw_line++;
	
	if((draw_line <= 8) || (draw_line > 232))
	{
		for(i = 8; i < 264; i++)
		{
			draw_color[draw_color_cnt] = 0;
			draw_color_cnt++;
		}
		if(draw_color_cnt >= (512*32-1024))
		{
			draw_color_cnt = 0;
			LCD_DMAPushColor((uint8_t*)&draw_color[0],(512*32-1024)*2);
		}
		if(draw_line >= 240)
		{
			draw_line = 0;
		}
	}else
	{
		for(i = 8; i < 264; i++)
		{
			draw_color[draw_color_cnt] = NES_Palette[ppu->dummy_buffer[i]];
			draw_color_cnt++;
		}
		if(draw_color_cnt >= (512*32-1024))
		{
			draw_color_cnt = 0;
			LCD_DMAPushColor((uint8_t*)&draw_color[0],(512*32-1024)*2);
		}
	}
}

void do_scanline_and_dont_draw(int LineNo)
{
	uint16_t i;
	// mmc2 / punchout -- we must simulate the ppu for every line
//	if(parent_nes->ROM->get_mapper_num() == 9)
//	{
//		do_scanline_and_draw(ppu->dummy_buffer);
//	}
//	else
	{
		// if sprite 0 flag not set and sprite 0 on current line
		if((!sprite0_hit()) && 
			(ppu->current_frame_line >= ((uint32_t)(spr_ram[0]+1))) && 
			(ppu->current_frame_line <  ((uint32_t)(spr_ram[0]+1+(sprites_8x16()?16:8))))
			)
		{
			// render line to dummy buffer
			do_scanline_and_draw(ppu->dummy_buffer);
			
		}
		else
		{
			if(spr_enabled() || bg_enabled())
			{
				LOOPY_SCANLINE_START(ppu->loopy_v, ppu->loopy_t);
				LOOPY_NEXT_LINE(ppu->loopy_v);
			}
			ppu->current_frame_line++;
		}
	}

} 
void start_vblank()
{
  ppu->in_vblank = 1;

  // set vblank register flag
  ppu->LowRegs[2] |= 0x80;
}

void end_vblank()
{
  ppu->in_vblank = 0;

  // reset vblank register flag and sprite0 hit flag1
  ppu->LowRegs[2] &= 0x3F;
}


// these functions read from/write to VRAM using loopy_v
uint8_t read_2007()
{
  uint16_t addr;
  uint8_t temp;

  addr = ppu->loopy_v;
  ppu->loopy_v += ppu->ppu_addr_inc;

  addr &= 0x3FFF;

  if(addr >= 0x3000)
  {
    // is it a palette entry?
    if(addr >= 0x3F00)
    {
      // palette

      // handle palette mirroring
      if(0x0000 == (addr & 0x0010))
      {
        // background palette
        return ppu->bg_pal[addr & 0x000F];
      }
      else
      {
        // sprite palette
        return ppu->spr_pal[addr & 0x000F];
      }
    }

    // handle mirroring
    addr &= 0xEFFF;
  }

  temp = ppu->read_2007_buffer;
  ppu->read_2007_buffer = VRAM(addr);

  return temp;
}

void write_2007(uint8_t data)
{
  uint16_t addr;

  addr = ppu->loopy_v;
  ppu->loopy_v += ppu->ppu_addr_inc;

  addr &= 0x3FFF;

  if(addr >= 0x3000)
  {
    // is it a palette entry?
    if(addr >= 0x3F00)
    {
      // palette
      data &= 0x3F;

      if(0x0000 == (addr & 0x000F)) // is it THE 0 entry?0的条目吗
      {
        ppu->bg_pal[0] = ppu->spr_pal[0] = data;
      }
      else if(0x0000 == (addr & 0x0010))
      {
        // background palette
        ppu->bg_pal[addr & 0x000F] = data;
      }
      else
      {
        // sprite palette
        ppu->spr_pal[addr & 0x000F] = data;
      }

      return;
    }

    // handle mirroring处理镜像
    addr &= 0xEFFF;
  }

  VRAM(addr) = data;
  if (addr < 0x2000) {
#if	NES_RAM_SPEED==1  //1:内存占用小 0:速度快	  
		
	  *(TILE(addr) + (addr & 0xf )) = data;//************************	 
#else  
	  update_tile(addr, data);
#endif	  
  }
}

#define UPDATE_4_PIXELS() \
	col = 0; \
	if(data & pattern_mask) col |= (bit << 6); \
	pattern_mask >>= 1; \
	if(data & pattern_mask) col |= (bit << 4); \
	pattern_mask >>= 1; \
	if(data & pattern_mask) col |= (bit << 2); \
	pattern_mask >>= 1; \
	if(data & pattern_mask) col |= (bit << 0); \
	*p++ |= col;

void update_tile(int byte_offset, uint8_t data)
{
	//int tileNum = byte_offset >> 4;
	int line = byte_offset & 0xf;

	uint8_t *p = TILE(byte_offset) + TILE_OFFSET(line & 0x7);
	uint8_t bit;
	uint8_t pattern_mask;
	int col;

	if (line < 8) {
		// low pattern
		bit = 0x01;

		*(uint16_t *)p &= 0xaaaa;

	} else {
		// high pattern
		bit = 0x02;
		*(uint16_t *)p &= 0x5555;
	}

	pattern_mask = 0x80;


	UPDATE_4_PIXELS();
	pattern_mask >>= 1;
	UPDATE_4_PIXELS();

}

uint8_t PPU_ReadFromPort(uint16_t addr)
{
  uint8_t temp;
	
	
  //switch(addr)
  switch(addr & 0x7)
  {
    //case 0x2002:
    case 0x2:            
        // clear toggle
        ppu->toggle_2005_2006 = 0;

        temp = ppu->LowRegs[2];

        // clear v-blank flag
        ppu->LowRegs[2] &= 0x7F;

        return temp;
      
//      break;

    //case 0x2007:
    case 0x7:
      return read_2007();
//      break;

  }

  return ppu->LowRegs[addr & 0x0007];
}

void  PPU_WriteToPort(uint8_t data,uint16_t addr )
{
  
//	uint32_t t;
	ppu->LowRegs[addr & 0x0007] = data;

  //switch(addr)
  switch(addr & 0x7)
  {
    //case 0x2000:
    case 0:
	{
      ppu->bg_pattern_table_addr  = (data & 0x10) ? 0x1000 : 0x0000;
      ppu->spr_pattern_table_addr = (data & 0x08) ? 0x1000 : 0x0000;
      ppu->ppu_addr_inc = (data & 0x04) ? 32 : 1;

	  // t:0000110000000000=d:00000011
      ppu->loopy_t = (ppu->loopy_t & 0xF3FF) | (((uint16_t)(data & 0x03)) << 10);

      break;
	}

	// Rick, lazy updating stuff
	//case 0x2001:
//    case 1:
//		bg_enabled();
//		
//		spr_enabled();
//	  break;

    //case 0x2003:
    case 3:
      ppu->spr_ram_rw_ptr = data;
      break;

    //case 0x2004:
    case 4:
      spr_ram[ppu->spr_ram_rw_ptr++] = data;
      break;

    //case 0x2005:
    case 5:
      ppu->toggle_2005_2006 = !ppu->toggle_2005_2006;

      if(ppu->toggle_2005_2006)
      {
        // first write
        
        // t:0000000000011111=d:11111000
        ppu->loopy_t = (ppu->loopy_t & 0xFFE0) | (((uint16_t)(data & 0xF8)) >> 3);

        // x=d:00000111
        ppu->loopy_x = data & 0x07;

      }
      else
      {
        // second write

        // t:0000001111100000=d:11111000
        ppu->loopy_t = (ppu->loopy_t & 0xFC1F) | (((uint16_t)(data & 0xF8)) << 2);
	      
        // t:0111000000000000=d:00000111
        ppu->loopy_t = (ppu->loopy_t & 0x8FFF) | (((uint16_t)(data & 0x07)) << 12);
 
      }

      break;

    //case 0x2006:
    case 6:
      ppu->toggle_2005_2006 = !ppu->toggle_2005_2006;

      if(ppu->toggle_2005_2006)
      {
        // first write

	      // t:0011111100000000=d:00111111
	      // t:1100000000000000=0
        ppu->loopy_t = (ppu->loopy_t & 0x00FF) | (((uint16_t)(data & 0x3F)) << 8);
      }
      else
      {
        // second write
        // t:0000000011111111=d:11111111
        ppu->loopy_t = (ppu->loopy_t & 0xFF00) | ((uint16_t)data);

	      // v=t
        ppu->loopy_v = ppu->loopy_t;
      }

      break;

    //case 0x2007:
    case 7:
      write_2007(data);
      break;
  }
}

//*****************************************************************************
#define DRAW_BG_PIXEL() \
  col = attrib_bits; \
 \
  if(pattern_lo & pattern_mask) col |= 0x01; \
  if(pattern_hi & pattern_mask) col |= 0x02; \
 \
  if(col & 0x03) \
  { \
    *p = ppu->bg_pal[col]; \
    /* set solid flag */ \
    *solid = BG_WRITTEN_FLAG; \
  } \
  else \
  { \
    *p =  ppu->bg_pal[0]; \
    /* set solid flag */ \
    *solid = 0; \
  } \
  solid++; \
  p++; \
//***************************************************************************************
#define NEW_BG_PIXEL() \
	if (col) {\
		col |= attrib_bits; \
		*solid = BG_WRITTEN_FLAG; \
	} else { \
		col = 0; \
		*solid = 0; \
	} \
    *p = ppu->bg_pal[col]; \
	solid++; \
	p++; \


void render_bg(uint8_t* buf)
{
  uint8_t *p;
  uint32_t i;
  uint8_t *solid;
  uint8_t *data;
  uint32_t col;

  uint32_t tile_x; // pixel coords within nametable像素坐标内nametable
  uint32_t tile_y;
  uint32_t name_addr;
  uint32_t line;
  uint32_t pattern_addr;

  uint32_t attrib_addr;
  uint32_t attrib_bits;
	
#if	NES_RAM_SPEED==1  //1:内存占用小 0:速度快	   
		uint8_t pattern_lo;
	    uint8_t pattern_hi;
	    uint8_t pattern_mask;
#else                     
		int col2;
#endif		

  tile_x = (ppu->loopy_v & 0x001F);
  tile_y = (ppu->loopy_v & 0x03E0) >> 5;

  name_addr = 0x2000 + (ppu->loopy_v & 0x0FFF);

  attrib_addr = 0x2000 + (ppu->loopy_v & 0x0C00) + 0x03C0 + ((tile_y & 0xFFFC)<<1) + (tile_x>>2);
  if(0x0000 == (tile_y & 0x0002))
    if(0x0000 == (tile_x & 0x0002))
      attrib_bits = (VRAM(attrib_addr) & 0x03) << 2;
    else
      attrib_bits = (VRAM(attrib_addr) & 0x0C);
  else
    if(0x0000 == (tile_x & 0x0002))
      attrib_bits = (VRAM(attrib_addr) & 0x30) >> 2;
    else
      attrib_bits = (VRAM(attrib_addr) & 0xC0) >> 4;

  p     = buf       + (SIDE_MARGIN - ppu->loopy_x);
  solid = ppu->solid_buf + (SIDE_MARGIN - ppu->loopy_x); // set "solid" buffer ptr

  
  line = (ppu->loopy_v & 0x7000) >> 12;

  // draw 33 tiles
  for(i = 33; i; i--)
  {
	
    pattern_addr = ppu->bg_pattern_table_addr + ((int32)VRAM(name_addr) << 4) + line;

 //   CHECK_MMC2(pattern_addr);//*******************************************************

#if	NES_RAM_SPEED==1  //1:内存占用小 0:速度快	   
		data = TILE(pattern_addr) + line;
			
		pattern_lo = *data;
		pattern_hi = *(data + 8);
		pattern_mask = 0x80;

		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
		pattern_mask >>= 1;
		DRAW_BG_PIXEL();
#else                     
		data = TILE(pattern_addr) + TILE_OFFSET(line);

		col2 = *data++;
		col = col2 >> 6; 
		NEW_BG_PIXEL();
		col = (col2 >> 4) & 0x03;
		NEW_BG_PIXEL();
		col = (col2 >> 2) & 0x03;
		NEW_BG_PIXEL();
		col = col2 & 0x03;
		NEW_BG_PIXEL();
		
		col2 = *data++;
		col = col2 >> 6; 
		NEW_BG_PIXEL();
		col = (col2 >> 4) & 0x03;
		NEW_BG_PIXEL();
		col = (col2 >> 2) & 0x03;
		NEW_BG_PIXEL();
		col = col2 & 0x03;
		NEW_BG_PIXEL();                    
#endif	

    tile_x++;
    name_addr++;

    // are we crossing a dual-tile boundary?我们穿越dual-tile边界吗?
    if(0x0000 == (tile_x & 0x0001))
    {
      // are we crossing a quad-tile boundary?
      if(0x0000 == (tile_x & 0x0003))
      {
        // are we crossing a name table boundary?
        if(0x0000 == (tile_x & 0x001F))
        {
          name_addr ^= 0x0400; // switch name tables
          attrib_addr ^= 0x0400;
          name_addr -= 0x0020;
          attrib_addr -= 0x0008;
          tile_x -= 0x0020;
        }

        attrib_addr++;
      }

      if(0x0000 == (tile_y & 0x0002))
        if(0x0000 == (tile_x & 0x0002))
          attrib_bits = (VRAM(attrib_addr) & 0x03) << 2;
        else
          attrib_bits = (VRAM(attrib_addr) & 0x0C);
      else
        if(0x0000 == (tile_x & 0x0002))
          attrib_bits = (VRAM(attrib_addr) & 0x30) >> 2;
        else
          attrib_bits = (VRAM(attrib_addr) & 0xC0) >> 4;
    }
  }

//  if(bg_clip_left8())
//  {
//    // clip left 8 pixels夹了8个像素SIDE_MARGIN = 8,	  
//	memset(buf + SIDE_MARGIN,  bg_pal[0], 8);
//    memset(solid + SIDE_MARGIN, 0, sizeof(solid[0])*8);
//  }
} 
void render_spr(uint8_t* buf)
{
	int32 s;              // sprite #
	int32  spr_x;         // sprite coordinates
	uint32_t spr_y;
	uint8_t* spr;           // pointer to sprite RAM entry
	uint8_t* p;             // draw pointer
	int line;
	//uint32_t *solid;
	uint8_t *solid;
	uint32_t priority;
	uint32_t tile_addr;
	int32 inc_x;           // drawing vars
	int32 start_x, end_x;
	int32 x,y;             // in-sprite coords
	uint32_t col;
	uint32_t num_sprites = 0;
	uint8_t *t;
	
	uint32_t spr_height;
	
#if	NES_RAM_SPEED==1  //1:内存占用小 0:速度快
	   
		uint8_t pattern_lo;
	    uint8_t pattern_hi;
#else                     
		uint32_t pattern;
#endif	

	spr_height = sprites_8x16() ? 16 : 8;

	//for(s = 0; s < 64; s++)
	for(s = 0, spr = spr_ram; s < 64; s++, spr+=4)
	{
		
		spr_y = spr[0]+1;
		
		// on current scanline对当前扫描线?
		if((spr_y > ppu->current_frame_line) || ((spr_y+(spr_height)) <= ppu->current_frame_line))
			continue;
		
		num_sprites++;
		if(num_sprites > 8)
		{
			/*if(!show_more_than_8_sprites)*/ break;//**************************************
		}
		
		// get x coord
		spr_x = spr[3];
		
		start_x = 0;
		end_x = 8;
		
		// clip right
		if((spr_x + 7) > 255)
		{
			end_x -= ((spr_x + 7) - 255);
		}
		
		// clip left
		if((spr_x < 8) && (spr_clip_left8()))
		{
			if(0 == spr_x) continue;
			start_x += (8 - spr_x);
		}
		
		y = ppu->current_frame_line - spr_y;
		
//		CHECK_MMC2(spr[1] << 4);//**************************************************************************
		
		// calc offsets into buffers
		p = &buf[SIDE_MARGIN + spr_x + start_x];
		solid = &ppu->solid_buf[SIDE_MARGIN + spr_x + start_x];
		
		// flip horizontally?
		if(spr[2] & 0x40) // yes
		{
			inc_x = -1;
			start_x = (8-1) - start_x;
			end_x = (8-1) - end_x;
		}
		else
		{
			inc_x = 1;
		}
		
		// flip vertically?
		if(spr[2] & 0x80) // yes
		{
			y = (spr_height-1) - y;
		}
		line = y & 7;
		
		// get priority bit
		priority = spr[2] & 0x20;
		
		tile_addr = spr[1] << 4;
		if(sprites_8x16()) {
			if(spr[1] & 0x01) {
				tile_addr += 0x1000;
				if(y < 8) tile_addr -= 16;
			} else {
				if(y >= 8) tile_addr += 16;
			}
		} else {
			tile_addr += ppu->spr_pattern_table_addr;
		}
		
#if	NES_RAM_SPEED==1  //1:内存占用小 0:速度快		
			t = TILE(tile_addr) + line; 
		
		    pattern_lo = *t;
            pattern_hi = *(t + 8);

#else 				
		// read 16bits = 2bits x 8pixels
		t = TILE(tile_addr) + TILE_OFFSET(line);
		pattern = ((uint32_t)*t << 8) | *(t + 1);   		

#endif	
		
		for(x = start_x; x != end_x; x += inc_x)
		{
			//uint8_t col = 0x00;
			
			// if a sprite has drawn on this pixel, don't draw anything如果一个雪碧吸引了这个像素,不画任何东西
			if(!((*solid) & SPR_WRITTEN_FLAG))
			{

#if	NES_RAM_SPEED==1  //1:内存占用小 0:速度快	
				
			    col = ((pattern_hi>>(7-x)<<1)&2)|((pattern_lo>>(7-x))&1);				
#else 	
				col = pattern >> ((7 - (x & 7)) * 2);
#endif				
				col &= 0x03;

				if (col) {
					col |= (spr[2] & 0x03) << 2;
					
					// set sprite 0 hit flag
					if(!s)
					{
						if((*solid) & BG_WRITTEN_FLAG)
						{
							ppu->LowRegs[2] |= 0x40;
						}
					}
					
					(*solid) |= SPR_WRITTEN_FLAG;
					if(priority)
					{
						//(*solid) |= SPR_WRITTEN_FLAG;
						if(!((*solid) & BG_WRITTEN_FLAG))
						{
							*p =  ppu->spr_pal[col];
						}
					}
					else
					{
						
						*p = ppu->spr_pal[col];
					}
				}
			}
			
			p++;
			solid++;
		}
	}

	if(num_sprites >= 8)
	{
		ppu->LowRegs[2] |= 0x20;
	}
	else
	{
		ppu->LowRegs[2] &= 0xDF;
	}
}








