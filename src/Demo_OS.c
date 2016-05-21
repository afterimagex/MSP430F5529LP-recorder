/*
 * Demo_OS.c
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#include "global.h"

//-----全局变量定义-----
uint8_t WheelKey_Prev = 255;
uint8_t ui8WheelKey = 0;
uint8_t ui8Task_Status = 0;

//----菜单字符串定义----
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
//----结构体任务数组定义----
const Subject Sub[12]={
		{MENU,(*Null),(*Null),(*Null),(*Null)},				//预留备用
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
 * 空函数
 */
static void Null()
{

}
/*
 * 绘制菜单函数
 */
void DrawMenu( uint8_t ui8SelectNum,const char *const cMenuString[])
{
	uint8_t i8Forward,i8Backward;
	//--------画待选中菜单项--------
	GrClearDisplay(&g_sContext);							//清屏
	GrContextFontSet(&g_sContext, &g_sFontCm16b);			//选择字体
	INVERSECOLOR;											//黑底白字
	GrStringDraw(&g_sContext, cMenuString[ui8SelectNum], AUTO_STRING_LENGTH, 0,31, OPAQUE_TEXT);
	NORMALCOLOR;											//正常情况均默认改回白底黑字
	//--------画其他三个菜单项--------
	GrStringDrawCentered(&g_sContext, cMenuString[0], AUTO_STRING_LENGTH, 63, 7, OPAQUE_TEXT);	//标题栏

	if(ui8SelectNum <= 1)
		i8Forward = TOTAL_MENU;
	else
		i8Forward = ui8SelectNum -1;

	if(ui8SelectNum >= TOTAL_MENU)
		i8Backward = 1;
	else
		i8Backward = ui8SelectNum + 1;

	GrStringDraw(&g_sContext, cMenuString[i8Forward], AUTO_STRING_LENGTH, 0,15, OPAQUE_TEXT);	//画待选中菜单项的前一个菜单项
	GrStringDraw(&g_sContext, cMenuString[i8Backward], AUTO_STRING_LENGTH, 0,47, OPAQUE_TEXT);	//画待选中菜单项的前一个菜单项

	GrFlush(&g_sContext);		//刷新COG12864的硬件缓存
}
/*
 * 菜单任务运行函数
 */
void Demo_OS()
{
	ui8ButtonKeyValue &= ~(KEY_LEFT_ENTER+KEY_RIGHT_EXIT);
	ui8Task_Status = MENU;
	//-----运行菜单显示程序-------
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
	//-----运行Demo程序-------
	ui8Task_Status = ui8WheelKey;
	Sub[ui8Task_Status].pfnDemoGraph();							//初始化Demo显示界面
	Sub[ui8Task_Status].pfnDemoBegin();							//初始化Demo例程中的各种配置
	ui8ButtonKeyValue &= ~(KEY_LEFT_ENTER+KEY_RIGHT_EXIT);		//确认和退出按键都清零

	while((ui8ButtonKeyValue&KEY_RIGHT_EXIT) == 0)
	{
		Sub[ui8Task_Status].pfnDemoMain();						//Demon程序的主循环
	}
	Sub[ui8Task_Status].pfnDemoQuit();							//退出Demon程序前恢复各寄存器配置为“原样”
	DrawMenu(ui8Task_Status,cMenuString);						//恢复原菜单界面
}
