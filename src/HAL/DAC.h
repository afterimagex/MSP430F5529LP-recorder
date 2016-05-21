/*
 * DAC8311.h
 *
 *  Created on: 2013-3-19
 */

#ifndef DAC_H_
#define DAC_H_
#include "global.h"
#define DAC7311

#ifdef DAC7311
#define DAC_RESOLUTION		12
#define DAC_RANGE			(4096-1)
#endif

#ifdef DAC8311
#define DAC_RESOLUTION		14
#define DAC_RANGE			(16384-1)
#endif

#ifdef DAC8411
#define DAC_RESOLUTION		16
#define DAC_RANGE			(65536-1)
#endif
// 14-bits
extern void DAC_Init(void);
extern void write2DAC(unsigned int Data);

#endif /* DAC8311_H_ */
