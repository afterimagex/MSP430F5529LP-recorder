/**********DAC8311.c******************/


#include "DAC.h"
//--------IO�궨��----------
#define SYNC_HIGH 		P1OUT |=BIT5
#define SYNC_LOW  		P1OUT &=~BIT5
#define SCLK_HIGH 		P6OUT |=BIT6
#define SCLK_LOW 		P6OUT &=~BIT6
#define DIN_HIGH 		P2OUT |=BIT2
#define DIN_LOW 		P2OUT &=~BIT2
/******************************************************************************************************
 * ��       �ƣ�DAC_Init()
 * ��       �ܣ���ʼ��DAC
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �������ǳ�ʼ�����IO��״̬
 * ��       ������
 ******************************************************************************************************/
void DAC_Init()
{
	//-----����IOΪ���-----
	P1DIR |=BIT5;
	P6DIR |=BIT6;
	P2DIR |=BIT2;
	//-----����IO��ʼ״̬Ϊ��-----
	SCLK_HIGH;
	SYNC_HIGH;
}
/******************************************************************************************************
 * ��       �ƣ�write2DAC(unsigned int Data)
 * ��       �ܣ���DACд16λ����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �������跢18λ��ͷ��λ0����16λ��DAC��������
 * ��       ������
 ******************************************************************************************************/
void write2DAC(unsigned int Data)
{
	unsigned int Temp,bitN;
	unsigned char i;
	Temp = Data;
	bitN = 1<<(DAC_RESOLUTION-1);
	SYNC_LOW;				//ʹ�ܿ�ʼ
	//-----����00�������Ƿǽ���ģʽ�����ܾ�ֹͣ�����ˣ���-----
	SCLK_HIGH;
	DIN_LOW;				//����0
	SCLK_LOW;

	SCLK_HIGH;
	DIN_LOW;				//����0
	SCLK_LOW;
	//-----���η���DAC_RESOLUTIONλ����-----
	for(i=0;i<DAC_RESOLUTION;i++)
	{
		SCLK_HIGH;
		//-----ͨ��λ�룬�ж����λ��1����0���Ѿ�����ʲô����-----
		if(Temp&bitN)	DIN_HIGH;
		else 			DIN_LOW;
		SCLK_LOW;
		Temp=Temp<<1;		//����һλ����Զ�����λ
	}
	//-----�����ֲ���Ҫ����16λ������Ϊ�������λ����-----
	for(i=0;i<(16-DAC_RESOLUTION-2);i++)
	{
		SCLK_HIGH;
		DIN_HIGH;			//����ȡֵ
		SCLK_LOW;
	}
	SYNC_HIGH;			    //ʹ�ܽ�ֹ������������DAC��
}








