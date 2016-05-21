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
	//  COG背光控制	0/1 on/off
	P3DIR |= BIT2;
	P3OUT |= BIT2;

	// 功放shutdown
	P4DIR |= BIT3;
	P4OUT &= ~BIT3;

	// 关闭白光LED
//	P2DIR |= BIT0;
//	P2OUT |= BIT0;

	// 录用开始按钮
	P2DIR &= ~BIT1;
	P2IES &= ~BIT1;
	P2IFG &= ~BIT1;
	P2OUT |= BIT1;
	P2REN |= BIT1;
	P2IE |= BIT1;
	// 录用播放按钮
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
	SetVCore(PMMCOREV_3);			// 设置工作在20Mhz的Vcore 1.8V

	P5SEL |= BIT5 + BIT4;			// 将P5.4/P5.5设为XT1晶振输入管脚（上电默认为IO口模式）
	//P5SEL |= BIT2 + BIT3;			// 将P5.2/P5.3设为XT2晶振输入管脚（上电默认为IO口模式）

	LFXT_Start(XT1DRIVE_3);			// 启动外部XT1_L低频时钟源
	//XT2_Start(XT1DRIVE_3);		// 启动外部高频XT2时钟源

	SELECT_FLLREF(SELREF__XT1CLK);	// 选择FLL时钟源
    // Use 32.768kHz XTAL as reference
	Init_FLL_Settle(25000,762);		// 设置倍频系数  20000,610 25000 762

	SELECT_ACLK(SELA__XT1CLK);		// 选择ACLK时钟源
//	SELECT_MCLK(SELM__XT1CLK);		// 选择MCLK时钟源
//	SELECT_SMCLK(SELS__XT2CLK);		// 选择SMCLK时钟源

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
    P1OUT &= ~BIT0;					// 板卡P1.0管脚外接LED红灯
    P1DIR |= BIT0;

    P4OUT &= ~BIT7;					// 板卡P4.7管脚外接LED绿灯
    P4DIR |= BIT7;
}



