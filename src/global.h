/*
 * global.h
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
/*
 * ͷ�ļ�
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

// demo���
#include "Template_Driver.h"
#include "Interrupt.h"
#include "grlib.h"
#include "HCI.h"
#include "Demo_OS.h"
#include "Record.h"
#include "UserGraphic.h"


/*
 * ϵͳ��غ궨��
 */
//system Information
#define SYSACLK		32768				// ��ǰ��ʼ��XT1ʱ��Ϊ32768HZ
#define SYSMCLK		25000000			// ��ǰ��ʼ��FLLʱ��Ϊ25MHZ
#define SYSSMCLK	25000000
#define MAX_WHEEL_VALUE 2450			//���̵�λ��������ֵ

/*
 * �ⲿ����/��������
 */
//system initialize
extern void Ports_init(void);
extern void SysClk_init(void);
extern void LED_init(void);

#endif /* GLOBAL_H_ */
