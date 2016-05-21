/*
 * Demo_OS.c
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#include "global.h"

//-----ȫ�ֱ�������-----
uint8_t WheelKey_Prev = 255;
uint8_t ui8WheelKey = 0;
uint8_t ui8Task_Status = 0;

//----�˵��ַ�������----
const char *const cMenuString[] = {
	"=MSP430F5529LP=",
	"1 .DC Motor",
    "2 .Step Motor",
    "3 .Ultrasonic",
    "4 .White LED",
    "5 .Accel Meter",
    "6 .Infrared",
    "7 .Recorder",
    "8 .Elec scale",
    "9 .Triode tracer",
    "10.Music Display",
    "11.Miscell Demo"
};
static void Null();
//----�ṹ���������鶨��----
const Subject Sub[12]={
		{MENU,(*Null),(*Null),(*Null),(*Null)},				//Ԥ������
		{TASK1,(*Null),(*Null),(*Null),(*Null)},
		{TASK2,(*Null),(*Null),(*Null),(*Null)},
		{TASK3,(*Null),(*Null),(*Null),(*Null)},
		{TASK4,(*Null),(*Null),(*Null),(*Null)},
		{TASK5,(*Null),(*Null),(*Null),(*Null)},
		{TASK6,(*Null),(*Null),(*Null),(*Null)},
		{TASK7,(*Record_Graph),(*Record_Begin),(*Record_Main),(*Record_Quit)},
		{TASK8,(*Null),(*Null),(*Null),(*Null)},
		{TASK9,(*Null),(*Null),(*Null),(*Null)},
		{TASK10,(*Null),(*Null),(*Null),(*Null)},
		{TASK11,(*Null),(Null),(*Null),(*Null)}
};

/*
 * �պ���
 */
static void Null()
{

}
/*
 * ���Ʋ˵�����
 */
void DrawMenu( uint8_t ui8SelectNum,const char *const cMenuString[])
{
	uint8_t i8Forward,i8Backward;
	//--------����ѡ�в˵���--------
	GrClearDisplay(&g_sContext);							//����
	GrContextFontSet(&g_sContext, &g_sFontCm16b);			//ѡ������
	INVERSECOLOR;											//�ڵװ���
	GrStringDraw(&g_sContext, cMenuString[ui8SelectNum], AUTO_STRING_LENGTH, 0,31, OPAQUE_TEXT);
	NORMALCOLOR;											//���������Ĭ�ϸĻذ׵׺���
	//--------�����������˵���--------
	GrStringDrawCentered(&g_sContext, cMenuString[0], AUTO_STRING_LENGTH, 63, 7, OPAQUE_TEXT);	//������

	if(ui8SelectNum <= 1)
		i8Forward = TOTAL_MENU;
	else
		i8Forward = ui8SelectNum -1;

	if(ui8SelectNum >= TOTAL_MENU)
		i8Backward = 1;
	else
		i8Backward = ui8SelectNum + 1;

	GrStringDraw(&g_sContext, cMenuString[i8Forward], AUTO_STRING_LENGTH, 0,15, OPAQUE_TEXT);	//����ѡ�в˵����ǰһ���˵���
	GrStringDraw(&g_sContext, cMenuString[i8Backward], AUTO_STRING_LENGTH, 0,47, OPAQUE_TEXT);	//����ѡ�в˵����ǰһ���˵���

	GrFlush(&g_sContext);		//ˢ��COG12864��Ӳ������
}
/*
 * �˵��������к���
 */
void Demo_OS()
{
	ui8ButtonKeyValue &= ~(KEY_LEFT_ENTER+KEY_RIGHT_EXIT);
	ui8Task_Status = MENU;
	//-----���в˵���ʾ����-------
	while((ui8ButtonKeyValue&KEY_LEFT_ENTER) == 0)
	{
		ui8WheelKey = GetWheelKey();
		_nop();
		if(ui8WheelKey != WheelKey_Prev)
		{
			DrawMenu(ui8WheelKey,cMenuString);
			WheelKey_Prev = ui8WheelKey;
		}
		LPM3;
	}
	//-----����Demo����-------
	ui8Task_Status = ui8WheelKey;
	Sub[ui8Task_Status].pfnDemoGraph();							//��ʼ��Demo��ʾ����
	Sub[ui8Task_Status].pfnDemoBegin();							//��ʼ��Demo�����еĸ�������
	ui8ButtonKeyValue &= ~(KEY_LEFT_ENTER+KEY_RIGHT_EXIT);		//ȷ�Ϻ��˳�����������

	while((ui8ButtonKeyValue&KEY_RIGHT_EXIT) == 0)
	{
		Sub[ui8Task_Status].pfnDemoMain();						//Demon�������ѭ��
	}
	Sub[ui8Task_Status].pfnDemoQuit();							//�˳�Demon����ǰ�ָ����Ĵ�������Ϊ��ԭ����
	DrawMenu(ui8Task_Status,cMenuString);						//�ָ�ԭ�˵�����
}
