/*
 * softFIFO.h
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */

#ifndef SOFTFIFO_H_
#define SOFTFIFO_H_
#include "global.h"

#define FIFOBUFF_SIZE  1024						/*��������С,���ݳ�����Ҫ���е���*/

extern volatile uint8_t FIFOBuff[FIFOBUFF_SIZE]; 			//���建���������(FIFO)
extern volatile uint16_t FIFO_Count;           	//FIFO�����ݸ���

extern uint8_t Read_FIFO();
extern void Write_FIFO(uint8_t value);
extern void init_sFIFO();

#endif /* SOFTFIFO_H_ */
