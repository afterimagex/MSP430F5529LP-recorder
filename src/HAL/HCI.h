/*
 * HCI.h
 *
 *  Created on: 2014-4-28
 *      Author: rainf_000
 */

#ifndef HCI_H_
#define HCI_H_

#define 	KEY_LEFT_ENTER 		(BIT3)
#define 	KEY_RIGHT_EXIT 		(BIT6)

//-----�ⲿ��������-----
extern void Key_init();
extern uint8_t GetWheelKey();
extern void P2_IODect();
extern void P2_IODect_Timer();
extern void Wheel_ADC_Begin();
extern void Wheel_ADC();
extern void WheelADC_init();
extern uint16_t ADC12_SingleSample(void);

//-----�ⲿ��������-----
extern uint16_t ui16WheelValue;
extern uint8_t ui8ButtonKeyValue;
extern uint8_t ui8Wheel_Pecent;
#endif /* HCI_H_ */
