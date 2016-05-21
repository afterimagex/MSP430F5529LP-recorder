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
	//-----�κ�����¾�ִ�а����͹��ּ������-----
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

	TA2CCTL0 &= ~CCIE;			// �ض�ʱ���ж�

	switch(Sub[ui8Task_Status].ui8Task_Num)
	{
	case TASK7:
		if((rcd_ply_sel==RECORD_BEGIN)&&(rcd_ply_flag==START))
			record_WaveFile();		// ��ʼ¼��
		else if((rcd_ply_sel==DISPPLAY_BEGIN)&&(rcd_ply_flag==START))
			waveData2DAC(0);		// ����¼��
		else
			_nop();
		break;


	default:break;
	}

	P3OUT ^= BIT4;
	TA2CCTL0 |= CCIE;			//����ʱ���ж�
}


