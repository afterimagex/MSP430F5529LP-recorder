/*
 * Record.c
 *
 *  Created on: 2014-5-26
 *      Author: zdl
 */
#include "global.h"

char *const Display_Name[]={
		"Music List",
		"1-Beyond.wav",
		"2-LetItG.wav",
		"3-WeAreO.wav",
	    "4-FanFan.wav",
	    "5-OldBoy.wav",
	    "6-Sodagr.wav",
	    "7-WangF1.wav",
	    "8-WangF2.wav",
	    "9-Mayday.wav",
	    "10-StevC.wav",
};

uint8_t Display_Num=0;
uint8_t Music_Enter=0;
void Music_Graph()
{
	ui8Flush_Flag=FLUSH_ON;
	//---------Draw Title----------
	GrClearDisplay(&g_sContext);

	GrFlush(&g_sContext);
	ui8Flush_Flag=FLUSH_OFF;
}

void Music_Begin()
{

	//----------------硬件初始化---------------
	//P3DIR |= BIT4;
	rcd_ply_sel = 255;
	DAC_Init();								//初始化DAC

	//--------初始化ADC12--------
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
				GPIO_PIN0);
	ADC12_A_init(ADC12_A_BASE,				// 初始化ADC12，
			ADC12_A_SAMPLEHOLDSOURCE_SC,
			ADC12_A_CLOCKSOURCE_ADC12OSC,
			ADC12_A_CLOCKDIVIDER_1);
	ADC12_A_enable(ADC12_A_BASE);			// 打开ADC12
	ADC12_A_setupSamplingTimer(ADC12_A_BASE,// 建立采样定时器控制采样保持时间，16个clock
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
	//-------------------初始化SD卡--------------------
//	SDCard_init_FATFS(0);					// 使用文件系统初始化SD卡
		uint8_t st=0;
	  	do
	  	{
	  		st = disk_initialize(0);		// 使用文件系统初始化SD卡
	  		if(st == STA_NOINIT)
			{
				GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
				GrStringDrawCentered(&g_sContext,"SD not Init!!",AUTO_STRING_LENGTH,63,31,1);
				}
	  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
	  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
	  		GrStringDrawCentered(&g_sContext,"             ",AUTO_STRING_LENGTH,63,31,1);
	  		if(st == STA_NOINIT)
	  			GrStringDrawCentered(&g_sContext,"CHECK SD CARD!!",AUTO_STRING_LENGTH,63,31,1);
	  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
	  		LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;LPM3;
	  		GrStringDrawCentered(&g_sContext,"               ",AUTO_STRING_LENGTH,63,31,1);
	  		if(ui8ButtonKeyValue &KEY_RIGHT_EXIT)
	  			break;
	  	}
	  	while(st == STA_NOINIT || st == STA_NODISK);
	//-------------------初始化Timer-------------------

	TIMER_A_configureUpMode(TIMER_A2_BASE,	// This API does not start the timer
			TIMER_A_CLOCKSOURCE_SMCLK,
			TIMER_A_CLOCKSOURCE_DIVIDER_1,
			(SYSSMCLK/SAMPLE_FREQ - 1),		// 定时1/1000=1ms
			TIMER_A_TAIE_INTERRUPT_DISABLE,
			TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
			TIMER_A_DO_CLEAR
				);
		TIMER_A_startCounter(TIMER_A2_BASE,	// 启动定时器
				TIMER_A_UP_MODE
				);

	// 打开播放器
	P4DIR |= BIT3;
	P4OUT |= BIT3;
	if(st != STA_NOINIT)
	ui8ButtonKeyValue &= ~(KEY_LEFT_ENTER+KEY_RIGHT_EXIT);		//确认和退出按键都清零
	ui8Flush_Flag=FLUSH_ON;
	_enable_interrupts();
}

void Music_Main()
{
	static uint8_t preDisplay_Num=255;

	while((ui8ButtonKeyValue&KEY_LEFT_ENTER) == 0 &&(ui8ButtonKeyValue&KEY_RIGHT_EXIT)==0)
	{
		Display_Num = GetWheelKey();
		_nop();
		if(Display_Num != preDisplay_Num)
		{
			DrawMenu(Display_Num,Display_Name);
			preDisplay_Num = Display_Num;

		}
		LPM3;
	}

	if(Music_Enter==1)
	{
		ui8ButtonKeyValue&=~KEY_LEFT_ENTER;
		rcd_ply_sel=0;//播放状态
		GrClearDisplay(&g_sContext);
		GrStringDrawCentered(&g_sContext, Display_Name[Display_Num], AUTO_STRING_LENGTH, 63, 20, OPAQUE_TEXT);

		GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
		GrStringDraw(&g_sContext,"Time",AUTO_STRING_LENGTH,0,53,1);

		GrStringNumberCentered(&g_sContext,0,0,35+22,57);
		GrStringDraw(&g_sContext,":",1,35+9,53,1);
		GrStringNumberCentered(&g_sContext,0,0,35+2,57);
		GrStringDraw(&g_sContext,"|",1,35+34,53,1);

		GrStringNumberCentered(&g_sContext,0,0,35+48,57);
		GrStringDraw(&g_sContext,":",1,35+55,53,1);
		GrStringNumberCentered(&g_sContext,0,0,35+70,57);
		ui8Flush_Flag=FLUSH_ON;
		GrFlush(&g_sContext);
		ui8Flush_Flag=FLUSH_OFF;
		P4OUT |= BIT3;

		WaveOpen(Display_Name[Display_Num],MONO);
		ui8Flush_Flag=FLUSH_ON;
		P4OUT &=~ BIT3;
		DrawMenu(Display_Num,Display_Name);
		GrFlush(&g_sContext);
		rcd_ply_sel = 255;
	}
	else
	{

		preDisplay_Num=255;
		rcd_ply_sel = 255;
	}

}

void Music_Quit()
{
	TA2CCTL0 &= ~CCIE;			// 关定时器中断
	TIMER_A_stop(TIMER_A2_BASE);
	P4OUT &= ~BIT3;				// 关闭喇叭
	ui8Flush_Flag=FLUSH_ON;
}
