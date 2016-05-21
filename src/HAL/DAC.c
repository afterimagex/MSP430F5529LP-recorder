/**********DAC8311.c******************/


#include "DAC.h"
//--------IO宏定义----------
#define SYNC_HIGH 		P1OUT |=BIT5
#define SYNC_LOW  		P1OUT &=~BIT5
#define SCLK_HIGH 		P6OUT |=BIT6
#define SCLK_LOW 		P6OUT &=~BIT6
#define DIN_HIGH 		P2OUT |=BIT2
#define DIN_LOW 		P2OUT &=~BIT2
/******************************************************************************************************
 * 名       称：DAC_Init()
 * 功       能：初始化DAC
 * 入口参数：无
 * 出口参数：无
 * 说       明：就是初始化相关IO的状态
 * 范       例：无
 ******************************************************************************************************/
void DAC_Init()
{
	//-----设置IO为输出-----
	P1DIR |=BIT5;
	P6DIR |=BIT6;
	P2DIR |=BIT2;
	//-----设置IO初始状态为高-----
	SCLK_HIGH;
	SYNC_HIGH;
}
/******************************************************************************************************
 * 名       称：write2DAC(unsigned int Data)
 * 功       能：对DAC写16位数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：共需发18位，头两位0，后16位是DAC量化数据
 * 范       例：无
 ******************************************************************************************************/
void write2DAC(unsigned int Data)
{
	unsigned int Temp,bitN;
	unsigned char i;
	Temp = Data;
	bitN = 1<<(DAC_RESOLUTION-1);
	SYNC_LOW;				//使能开始
	//-----发送00，代表是非节能模式（节能就停止工作了）。-----
	SCLK_HIGH;
	DIN_LOW;				//数据0
	SCLK_LOW;

	SCLK_HIGH;
	DIN_LOW;				//数据0
	SCLK_LOW;
	//-----依次发送DAC_RESOLUTION位数据-----
	for(i=0;i<DAC_RESOLUTION;i++)
	{
		SCLK_HIGH;
		//-----通过位与，判断最高位是1还是0，已决定发什么数据-----
		if(Temp&bitN)	DIN_HIGH;
		else 			DIN_LOW;
		SCLK_LOW;
		Temp=Temp<<1;		//左移一位，永远发最高位
	}
	//-----根据手册需要补齐16位，以下为补齐空闲位数据-----
	for(i=0;i<(16-DAC_RESOLUTION-2);i++)
	{
		SCLK_HIGH;
		DIN_HIGH;			//任意取值
		SCLK_LOW;
	}
	SYNC_HIGH;			    //使能禁止。数据锁存入DAC。
}








