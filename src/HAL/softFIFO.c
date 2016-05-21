/*
 * softFIFO.c
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */
#include "softFIFO.h"

#define DISABLE_INT			_DINT()			// ���ж�
#define RESTORE_INT			_EINT()			// ���ж�

/***********************���º궨����FIFO�������й�******************************/
uint8_t volatile FIFOBuff[FIFOBUFF_SIZE]={0}; 	//���建���������(FIFO)
static uint16_t FIFO_IndexW=0;      	//�������д��ָ��(ͷָ��)
static uint16_t FIFO_IndexR=0;      	//������ж�ȡָ��(βָ��
volatile uint16_t  FIFO_Count=0;           		//FIFO��δ��ȡ���ݵĸ���
/****************************************************************************
* ��    �ƣ�Write_FIFO()
* ��    �ܣ���һ��ֵѹ����Ӧ���̻������
* ��ڲ�����value:����
* ���ڲ�������
****************************************************************************/
void Write_FIFO(uint8_t value)
{
  if(FIFO_Count>=FIFOBUFF_SIZE) return;		//����������������������ֵ
  DISABLE_INT;
  FIFO_Count++;                       		//������������
  FIFOBuff[FIFO_IndexW] = value;   			//�Ӷ���ͷ��׷���µ�����
  if (++FIFO_IndexW >=  FIFOBUFF_SIZE) 		//ѭ�����У��������ͷָ��Խ��
   {
	  FIFO_IndexW = 0;                  	//����ͷָ��ص�������ʼλ��
   }
  RESTORE_INT;
}
/****************************************************************************
* ��    �ƣ�Read_FIFO()
* ��    �ܣ��ӻ�������ڶ�ȡһ��ֵ
* ��ڲ�������
* ���ڲ�����value������
* ˵    ��: ����һ�θú��������Զ�ɾ�����������һ�����ݡ�
****************************************************************************/
uint8_t Read_FIFO()
{
	uint8_t value=0;
	if(FIFO_Count==0)  return(0);     		//�������ݣ�����0
	DISABLE_INT;							//���в��������в�������ʱ�ж�ִ��ScanIO����������ȫ�ֱ�����ȷ
	FIFO_Count--;                     		//��������������1
	value=FIFOBuff[FIFO_IndexR];    		//�ӻ�����β����ȡһ������
	if (++FIFO_IndexR >= FIFOBUFF_SIZE)		//ѭ�����У��������βָ��Խ��
	{
	FIFO_IndexR = 0;             			//����βָ��ص�������ʼλ��
	}
	RESTORE_INT;                     		//�ָ��ж�����
	return(value);
}
void init_sFIFO()
{
	uint16_t i=0;
	FIFO_Count = 0;
	for(i=0;i<FIFOBUFF_SIZE;i++)
		FIFOBuff[i] = 0;
}



