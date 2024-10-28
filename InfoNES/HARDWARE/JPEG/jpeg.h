#ifndef _JPEG_H
#define _JPEG_H
#include "ff.h"

typedef struct
{
	FIL *fp;
	uint8_t *fbuf;
	uint32_t wfbuf;
}IODEV;

int JPEG_Code(char *filepath);
void JPEG_Seek(void);

#endif
