/*
 * UserGraphic.c
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#ifndef USERGRAPHIC_H_
#define USERGRAPHIC_H_

extern void SetAxis(uint8_t X_O,uint8_t Y_O,uint8_t X_L,uint8_t Y_L);
extern void GrAxisGraph();
extern void GrPixelGraph(uint16_t lX_Percent,uint16_t lY_Percent);
extern void GrAxisAreaClear();
extern void Num2String(int16_t i16Number,uint8_t ui8Dot,uint8_t *p);
extern void GrStringNumberCentered(const tContext *pContext, int16_t Number,uint8_t Dot,uint8_t ui8X, uint8_t ui8Y);
extern void GrTaskRectDraw();
extern void GrAxisDraw();
extern int  Draw_Histogram(uint8_t ui8Mode);
extern void Draw_Waveform(uint16_t *ui16DataPecent,uint8_t ui8Num);
extern void EarseDispArea();
//extern void COG12864_playFixedNum(const tContext *pContext,uint8_t row, uint8_t col,int32_t Digit,uint8_t dot, uint8_t fix,uint8_t style);

extern tContext g_sContext;

// ª≠Õºœ‡πÿ
#define NORMALCOLOR 	GrContextForegroundSet(&g_sContext, ClrWhite);	GrContextBackgroundSet(&g_sContext, ClrBlack)
#define INVERSECOLOR 	GrContextForegroundSet(&g_sContext, ClrBlack);	GrContextBackgroundSet(&g_sContext, ClrWhite)

#endif /* USERGRAPHIC_H_ */
