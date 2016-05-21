/*
 * main.c
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#include <global.h>

void System_init()
{
	//-----ϵͳ��س�ʼ��-----
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	Ports_init();				//��ʼ��IO
	SysClk_init();				//��ʼ��ʱ��Ϊ25MHz
	LED_init();					//��ʼ��LP����LED
	Key_init();					//���ְ�����ʼ��
}

int main(void)
{
	System_init();
	_enable_interrupts();
	//-----�����־-----
	ui8Task_Status=TASK7;
	//-----���ƽ���-----
//	Record_Graph();
	//-----��ʼ��-----
	Record_Begin();

	while(1)
	{
	//-----����������-----
		Record_Main();
	}
}
