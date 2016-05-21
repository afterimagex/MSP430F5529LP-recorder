/*
 * Interrupt.c
 *
 *  Created on: 2014-4-30
 *      Author: Administrator
 */
#include"global.h"


//uint8_t P2Use_Flag = 1;
//==========WDT================
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR()
{
	//-----任何情况下均执行按键和滚轮检测任务-----
	P2REN |= BIT3+BIT6;
	__delay_cycles(75);
	P2_IODect();
	P2REN &= ~(BIT3+BIT6);
	if(Sub[ui8Task_Status].ui8Task_Num !=TASK7)
	Wheel_ADC_Begin();

	LPM3_EXIT;

}


//==========ADC12================
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  6: Wheel_ADC();break;             	// Vector  6:  ADC12IFG0

  default: break;
  }
}

// Timer2 A0 interrupt service routine
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer2_A0_ISR (void)
{
	//_DINT();

	TA2CCTL0 &= ~CCIE;			// 关定时器中断

	switch(Sub[ui8Task_Status].ui8Task_Num)
	{
	case TASK7:
		if((rcd_ply_sel==RECORD_BEGIN)&&(rcd_ply_flag==START))
			record_WaveFile();		// 开始录音
		else if((rcd_ply_sel==DISPPLAY_BEGIN)&&(rcd_ply_flag==START))
			waveData2DAC(0);		// 播放录音
		else
			_nop();
		break;


	default:break;
	}

	P3OUT ^= BIT4;
	TA2CCTL0 |= CCIE;			//开定时器中断
}


