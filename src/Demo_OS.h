/*
 * Demo_OS.h
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#ifndef DEMO_OS_H_
#define DEMO_OS_H_
#include "global.h"

//菜单控制
#define TOTAL_MENU 11
#define	UNIT_KEY		(MAX_WHEEL_VALUE/(TOTAL_MENU-1))	//2
//任务结构体
typedef struct{
	uint8_t ui8Task_Num;
	void (*pfnDemoGraph)();
	void (*pfnDemoBegin)();
	void (*pfnDemoMain)();
	void (*pfnDemoQuit)();
} Subject;
//任务号枚举
enum {MENU=0,TASK1=1,TASK2,TASK3,TASK4,TASK5,TASK6,TASK7,TASK8,TASK9,TASK10,TASK11};
//全局变量与外部函数声明
extern const Subject Sub[12];
extern uint8_t ui8Task_Status;
extern const char *const cMenuString[];
extern void DrawMenu( uint8_t ui8SelectNum,const char *const cMenuString[]);
extern void Demo_OS();

#endif /* DEMON_OS_H_ */
