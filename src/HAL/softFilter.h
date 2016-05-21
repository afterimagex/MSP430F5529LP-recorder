/*
 * softFilter.h
 *
 *  Created on: 2013-1-25
 *      Author: wang jing bing
 */

#ifndef SOFTFILTER_H_
#define SOFTFILTER_H_
#include "global.h"

#define N_FT	64	//定义滤波个数
extern uint16_t filter(uint16_t *ptr,uint8_t n);
extern uint16_t filter_N(uint16_t *ptr);


#endif /* SOFTFILTER_H_ */
