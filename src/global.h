/*
 * global.h
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
/*
 * 头文件
 */

//common  file
#include <msp430f5529.h>
#include <string.h>
#include <stdint.h>
//Core library file
#include "hal_ucs.h"
#include "hal_flash.h"
#include "hal_macros.h"
#include "hal_pmap.h"
#include "hal_tlv.h"
#include "hal_pmm.h"
#include "adc12_a.h"
#include "ref.h"
//driverlib
#include "gpio.h"
#include "timer_a.h"
#include "softFIFO.h"
#include "DAC.h"
//file system
#include "diskio.h"
#include "ff.h"
#include "wavType.h"

// demo相关
#include "Template_Driver.h"
#include "Interrupt.h"
#include "grlib.h"
#include "HCI.h"
#include "Demo_OS.h"
#include "Record.h"
#include "UserGraphic.h"


/*
 * 系统相关宏定义
 */
//system Information
#define SYSACLK		32768				// 当前初始化XT1时钟为32768HZ
#define SYSMCLK		25000000			// 当前初始化FLL时钟为25MHZ
#define SYSSMCLK	25000000
#define MAX_WHEEL_VALUE 2450			//轮盘电位器最大采样值

/*
 * 外部函数/变量声明
 */
//system initialize
extern void Ports_init(void);
extern void SysClk_init(void);
extern void LED_init(void);

#endif /* GLOBAL_H_ */
