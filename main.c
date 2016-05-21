/*
 * main.c
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#include <global.h>

void System_init()
{
	//-----系统相关初始化-----
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	Ports_init();				//初始化IO
	SysClk_init();				//初始化时钟为25MHz
	LED_init();					//初始化LP板上LED
	Key_init();					//各种按键初始化
}

int main(void)
{
	System_init();
	_enable_interrupts();
	//-----任务标志-----
	ui8Task_Status=TASK7;
	//-----绘制界面-----
//	Record_Graph();
	//-----初始化-----
	Record_Begin();

	while(1)
	{
	//-----运行主函数-----
		Record_Main();
	}
}
