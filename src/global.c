/*
 * global.c
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#include "global.h"
/*
 * ======== Init_Ports ========
 */
void Ports_init (void)
{
	//  COG�������	0/1 on/off
	P3DIR |= BIT2;
	P3OUT |= BIT2;

	// ����shutdown
	P4DIR |= BIT3;
	P4OUT &= ~BIT3;

	// �رհ׹�LED
//	P2DIR |= BIT0;
//	P2OUT |= BIT0;

	// ¼�ÿ�ʼ��ť
	P2DIR &= ~BIT1;
	P2IES &= ~BIT1;
	P2IFG &= ~BIT1;
	P2OUT |= BIT1;
	P2REN |= BIT1;
	P2IE |= BIT1;
	// ¼�ò��Ű�ť
	P1DIR &= ~BIT1;
	P1IES &= ~BIT1;
	P1IFG &= ~BIT1;
	P1OUT |= BIT1;
	P1REN |= BIT1;
	P1IE |= BIT1;
}
/*
 * ======== Init_SYSCLK ========
 */
void SysClk_init(void)
{
	SetVCore(PMMCOREV_3);			// ���ù�����20Mhz��Vcore 1.8V

	P5SEL |= BIT5 + BIT4;			// ��P5.4/P5.5��ΪXT1��������ܽţ��ϵ�Ĭ��ΪIO��ģʽ��
	//P5SEL |= BIT2 + BIT3;			// ��P5.2/P5.3��ΪXT2��������ܽţ��ϵ�Ĭ��ΪIO��ģʽ��

	LFXT_Start(XT1DRIVE_3);			// �����ⲿXT1_L��Ƶʱ��Դ
	//XT2_Start(XT1DRIVE_3);		// �����ⲿ��ƵXT2ʱ��Դ

	SELECT_FLLREF(SELREF__XT1CLK);	// ѡ��FLLʱ��Դ
    // Use 32.768kHz XTAL as reference
	Init_FLL_Settle(25000,762);		// ���ñ�Ƶϵ��  20000,610 25000 762

	SELECT_ACLK(SELA__XT1CLK);		// ѡ��ACLKʱ��Դ
//	SELECT_MCLK(SELM__XT1CLK);		// ѡ��MCLKʱ��Դ
//	SELECT_SMCLK(SELS__XT2CLK);		// ѡ��SMCLKʱ��Դ

//	  P1DIR |= BIT0;                // ACLK set out to pins
//	  P1SEL |= BIT0;
//	  P2DIR |= BIT2;                // SMCLK set out to pins
//	  P2SEL |= BIT2;
//	  P7DIR |= BIT7;                // MCLK set out to pins
//	  P7SEL |= BIT7;
}
/*
 * ======== Init_LED on LP5529 ========
 */
void LED_init(void)
{
    P1OUT &= ~BIT0;					// �忨P1.0�ܽ����LED���
    P1DIR |= BIT0;

    P4OUT &= ~BIT7;					// �忨P4.7�ܽ����LED�̵�
    P4DIR |= BIT7;
}



