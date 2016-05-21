/*
 * softFIFO.h
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */

#ifndef SOFTFIFO_H_
#define SOFTFIFO_H_
#include "global.h"

#define FIFOBUFF_SIZE  1024						/*缓冲区大小,根据程序需要自行调整*/

extern volatile uint8_t FIFOBuff[FIFOBUFF_SIZE]; 			//定义缓冲队列数组(FIFO)
extern volatile uint16_t FIFO_Count;           	//FIFO中数据个数

extern uint8_t Read_FIFO();
extern void Write_FIFO(uint8_t value);
extern void init_sFIFO();

#endif /* SOFTFIFO_H_ */
