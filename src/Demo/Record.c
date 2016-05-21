/*
 * Record.c
 *
 *  Created on: 2014-5-26
 *      Author: zdl
 */
#include "global.h"

char *const Record_Name[11]={
		"Record_1.wav",
		"Record_2.wav",
		"Record_3.wav",
	    "Record_4.wav",
	    "Record_5.wav",
	    "Record_6.wav",
	    "Record_7.wav",
	    "Record_8.wav",
	    "Record_9.wav",
	    "Record_10.wav",
	    "Record_11.wav"};

uint8_t Record_Num_Rec=0;
uint8_t Record_Num_Ply=0;
void Record_Graph()
{
	ui8Flush_Flag=FLUSH_ON;
	//---------Draw Title----------
	ui8Flush_Flag=FLUSH_OFF;
}

void Record_Begin()
{
	//----------------Ӳ����ʼ��---------------
//	P3DIR |= BIT4;
	rcd_ply_sel = 255;
	DAC_Init();								//��ʼ��DAC

	//--------��ʼ��ADC12--------
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
				GPIO_PIN0);
	ADC12_A_init(ADC12_A_BASE,				// ��ʼ��ADC12��
			ADC12_A_SAMPLEHOLDSOURCE_SC,
			ADC12_A_CLOCKSOURCE_ADC12OSC,
			ADC12_A_CLOCKDIVIDER_1);
	ADC12_A_enable(ADC12_A_BASE);			// ��ADC12
	ADC12_A_setupSamplingTimer(ADC12_A_BASE,// ����������ʱ�����Ʋ�������ʱ�䣬16��clock
			ADC12_A_CYCLEHOLD_64_CYCLES,
			ADC12_A_CYCLEHOLD_4_CYCLES,
			ADC12_A_MULTIPLESAMPLESDISABLE);
	ADC12_A_memoryConfigure(ADC12_A_BASE,
			ADC12_A_MEMORY_3,
			ADC12_A_INPUT_A0,
			ADC12_A_VREFPOS_AVCC,			// VREF+ = AVCC
			ADC12_A_VREFNEG_AVSS,			// VREF- = AVSS
			ADC12_A_NOTENDOFSEQUENCE);
	//Enable memory buffer 0 interrupt
	ADC12_A_clearInterrupt(ADC12_A_BASE,
			ADC12_A_IFG3 + ADC12_A_IFG0);

	ADC12_A_enableInterrupt(ADC12_A_BASE,
			ADC12IE0);
	 __delay_cycles(75);
	//-------------------��ʼ��SD��--------------------
	//SDCard_init_FATFS(0);					// ʹ���ļ�ϵͳ��ʼ��SD��
	uint8_t st=0;
	do
	{
		st = disk_initialize(0);		// ʹ���ļ�ϵͳ��ʼ��SD��
  		if(st == STA_NOINIT)
		{
			}
  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
  		if(st == STA_NOINIT)
  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
  		if(ui8ButtonKeyValue &KEY_RIGHT_EXIT)
  			  			break;
	}
	while(st == STA_NOINIT || st == STA_NODISK);
	//-------------------��ʼ��Timer-------------------

	TIMER_A_configureUpMode(TIMER_A2_BASE,	// This API does not start the timer
			TIMER_A_CLOCKSOURCE_SMCLK,
			TIMER_A_CLOCKSOURCE_DIVIDER_1,
			(SYSSMCLK/SAMPLE_FREQ - 1),		// ��ʱ1/1000=1ms
			TIMER_A_TAIE_INTERRUPT_DISABLE,
			TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
			TIMER_A_DO_CLEAR
				);
		TIMER_A_startCounter(TIMER_A2_BASE,	// ������ʱ��
				TIMER_A_UP_MODE
				);

	// �򿪲�����
	P4DIR |= BIT3;
	P4OUT |= BIT3;
	if(st != STA_NOINIT)
	ui8ButtonKeyValue &= ~(BIT3+BIT6);		//ȷ�Ϻ��˳�����������
	_enable_interrupts();
}

void Record_Main()
{
	if(rcd_ply_sel == RECORD_BEGIN)
	{
		P1OUT |= BIT0;
		P4OUT &= ~BIT7;
		P4OUT &= ~BIT3;
		WaveMake(Record_Name[Record_Num_Rec]);
		Record_Num_Rec++;
		if(Record_Num_Rec==1)
			Record_Num_Rec=0;
		rcd_ply_sel = 255;
	}
	else if(rcd_ply_sel == DISPPLAY_BEGIN)
	{
		P4OUT |= BIT3;
		P4OUT |= BIT7;
		WaveOpen(Record_Name[Record_Num_Ply],MONO);
		Record_Num_Ply++;
		if(Record_Num_Ply==5)
			Record_Num_Ply=0;
		P4OUT &= ~BIT7;
		rcd_ply_sel = 255;
	}
	else
	{
		P4OUT &= ~BIT3;
		LPM3;LPM3;LPM3;LPM3;LPM3;
		P4OUT ^= BIT7;
		P1OUT &= ~BIT0;
	}
}

void Record_Quit()
{
	TA2CCTL0 &= ~CCIE;			// �ض�ʱ���ж�
	TIMER_A_stop(TIMER_A2_BASE);
	P1OUT &= ~BIT2;				// �ر�����
	ui8Flush_Flag=FLUSH_ON;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
	unsigned int i;
	for(i=0;i<15000;i++);	//��ʱ����
	if((P2IN&BIT1)!=1)
	{
		if(P1OUT&BIT0)
			rcd_ply_sel = RECORD_STOP;
		else
			rcd_ply_sel = RECORD_BEGIN;
		P2IFG &= ~BIT1;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
	unsigned int i;
	for(i=0;i<15000;i++);	//��ʱ����
	if((P1IN&BIT1)!=1)
	{
		rcd_ply_sel = DISPPLAY_BEGIN;
		P1IFG &= ~BIT1;
	}
}
