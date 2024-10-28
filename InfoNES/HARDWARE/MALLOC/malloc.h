#ifndef __MALLOC_H
#define __MALLOC_H
#include "main.h"

	  
#ifndef NULL
#define NULL 0
#endif

#define SRAMIN1	 0	//�ڲ��ڴ��1
#define SRAMIN2  1	//�ڲ��ڴ��2

//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM1����
#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			1*1024  						//�������ڴ� 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

//mem2�ڴ�����趨.mem2���ڴ�ش����ڲ�SRAM2����
#define MEM2_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM2_MAX_SIZE			1*1024  						//�������ڴ�900K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
		 	 
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(uint8_t);					//��ʼ��
	uint8_t (*perused)(uint8_t);		  	    	//�ڴ�ʹ����
	uint8_t 	*membase[2];					//�ڴ�� ����3��������ڴ�
	uint16_t *memmap[2]; 					//�ڴ����״̬��
	uint8_t  memrdy[2]; 						//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,uint8_t c,uint32_t count);	 //�����ڴ�
void mymemcpy(void *des,void *src,uint32_t n);//�����ڴ�     
void mem_init(uint8_t memx);					 //�ڴ�����ʼ������(��/�ڲ�����)
uint32_t mem_malloc(uint8_t memx,uint32_t size);		 //�ڴ����(�ڲ�����)
uint8_t mem_free(uint8_t memx,uint32_t offset);		 //�ڴ��ͷ�(�ڲ�����)
uint8_t mem_perused(uint8_t memx);				 //����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(uint8_t memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(uint8_t memx,uint32_t size);			//�ڴ����(�ⲿ����)
void *myrealloc(uint8_t memx,void *ptr,uint32_t size);//���·����ڴ�(�ⲿ����)


#endif


