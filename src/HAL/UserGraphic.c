/*
 * UserGraphic.h
 *
 *  Created on: 2014-8-31
 *      Author: zdl
 */

#include "global.h"

tContext g_sContext;

//绘制坐标 相关变量定义
 typedef struct
 {
	 uint32_t X_origin;
	 uint32_t Y_origin;
	 uint32_t X_length;
	 uint32_t Y_length;
 }AXIS;
 static AXIS Axis={0,0,0,0};
 static tRectangle AxisArea={0,0,0,0};

/*
 * 设置坐标参数
 */
 void SetAxis(uint8_t X_O,uint8_t Y_O,uint8_t X_L,uint8_t Y_L)
 {
	 Axis.X_origin=X_O;
	 Axis.Y_origin=Y_O;
	 Axis.X_length=X_L;
	 Axis.Y_length=Y_L;

	 AxisArea.sXMin=Axis.X_origin+1;
	 AxisArea.sXMax=Axis.X_origin+Axis.X_length;
	 AxisArea.sYMin=Axis.Y_origin-1-Axis.Y_length;
	 AxisArea.sYMax=Axis.Y_origin-2;
 }
 /*
  * 绘制坐标轴
  */
 void GrAxisGraph()
 {
 	GrLineDrawV(&g_sContext, Axis.X_origin, Axis.Y_origin, Axis.Y_origin-Axis.Y_length-3);
 	GrLineDrawH(&g_sContext, Axis.X_origin, Axis.X_origin+Axis.X_length+3, Axis.Y_origin);

 	GrLineDraw(&g_sContext, Axis.X_origin-2, Axis.Y_origin-Axis.Y_length-1, Axis.X_origin, Axis.Y_origin-Axis.Y_length-3);
 	GrLineDraw(&g_sContext, Axis.X_origin+2, Axis.Y_origin-Axis.Y_length-1, Axis.X_origin, Axis.Y_origin-Axis.Y_length-3);

 	GrLineDraw(&g_sContext, Axis.X_origin+Axis.X_length+1, Axis.Y_origin-2, Axis.X_origin+Axis.X_length+3, Axis.Y_origin);
 	GrLineDraw(&g_sContext, Axis.X_origin+Axis.X_length+1, Axis.Y_origin+2, Axis.X_origin+Axis.X_length+3, Axis.Y_origin);
 }
 /*
  * 坐标系内打点函数
  */
 void GrPixelGraph(uint16_t lX_Percent,uint16_t lY_Percent)
 {
	 uint32_t lx,ly;
	 if(lX_Percent>=100)
		 lX_Percent=100;
	 if(lY_Percent>=100)
			 lY_Percent=100;
	 lx=Axis.X_origin+1+(Axis.X_length-1)*lX_Percent/100;
	 ly=Axis.Y_origin-1-(Axis.Y_length-1)*lY_Percent/100;
	 DpyPixelDraw((&g_sContext)->pDisplay, lx, ly, (&g_sContext)->ulForeground);
 }
 /*
  * 清除坐标轴内的点
  */
 void GrAxisAreaClear()
 {
	INVERSECOLOR;
	GrRectFill(&g_sContext,&AxisArea);	//擦除待画点区域
	NORMALCOLOR;
 }

 /*
  * 数字转字符串函数
  */
 void Num2String(int16_t i16Number,uint8_t ui8Dot,uint8_t *p)
 {
 		int8_t i=0 ,ds=0;
 		uint8_t pos=0,m=0;
 		uint8_t DispBuff[11]={0};
 		uint8_t tempBuff[11]={0};

 	  if(i16Number<0)
 		{
 		  i16Number=-i16Number;								//处理负数
 		DispBuff[0]='-'; 							//显示负号
 		m=1;										//跟踪缓存位置
 		}
 	  else
 		  m=0; 										//等效为-清除负号
 	  for(i=m;i<11;i++)                       		//拆分数字
 	   {
 	     DispBuff[i]=(i16Number%10);
 	     i16Number /= 10;
 	   }
 	  for(i=10;i>=0;i--)                     		//消隐无效"0"
 	   {
 	     if (DispBuff[i]==0) DispBuff[i]=0;
 	     else
 	     {
 	    	 pos=i;									// 保存当前第一个有效值
 	    	 break;
 	     }
 	   }
 	  tempBuff[0] = DispBuff[0];					// 保存对应的符号位
 	  for(i=10;i>=m;i--)                     		// 高低位数据转换存储位置，高位在前，符号位不变
 	   {
 		  if(i <= pos)
 		  {
 			  tempBuff[pos-i+m] = DispBuff[i]+ 0x30;
 		  }
 		  else
 			  tempBuff[i] = 32;						// 空格（ASCII）
 	   }
 	  if(ui8Dot>0)
 	  {
 		  if(ui8Dot < pos+1-m)							// pos +1 - m 为当前数组数据位数
 		  {
 			  for(i=pos;i>=pos-(ui8Dot-1);i--)
 			  {
 				  tempBuff[i+1]=tempBuff[i];
 			  }
 			  tempBuff[pos-(ui8Dot-1)] = 0x2e;				// 小数点
 		  }
 		  else //if(ui8Dot > pos+m)
 		  {
 			  ds = ui8Dot - (pos+1-m);						// 移位距离(既需要增加几个0)
 			  for(i=pos;i>=m;i--)
 			  {
 				  tempBuff[i+ds+2]= tempBuff[i];		//这里+2是为了增加前置：“0.”
 			  }
 			  for(i=0;i<ds;i++)
 			  {
 				  tempBuff[m+2+i]=0x30;					//这里 小数点后“补0操作”
 			  }
 			  tempBuff[m] = 0x30;						// 添加前置	“0.”
 			  tempBuff[m+1] = 0x2e;
 		  }
 	  }
 	   for(i=0;i<7;i++)
 	   {
 		 if(tempBuff[i]!= ' ')
 		 {
 			 *p=tempBuff[i];
 			 *p++;
 		 }
 		 else
 			 break;
 	   }

 }
/*
 * 数字显示函数
 */
 void GrStringNumberCentered(const tContext *pContext, int16_t Number,uint8_t ui8Dot,uint8_t ui8X, uint8_t ui8Y)
 {
 	uint8_t String[8]={0};
 	Num2String(Number,ui8Dot,String);
 	GrStringDrawCentered(pContext, (const char*)String, AUTO_STRING_LENGTH,ui8X,ui8Y, OPAQUE_TEXT);
 }


 //*********************************************************
 //
 //						以下函数为绘制菜单用
 //
 //*********************************************************

 /*
  * 绘制任务框
  *-------------------------------------------------------------
  *===============
  *|| TASK NAME ||
  *===============
  *-------------------------------------------------------------
  **/
 static const tRectangle sRactangle1={12,0,112,14};
 static const tRectangle sRactangle2={14,2,110,12};
 void GrTaskRectDraw()
 {
 	GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
 	GrStringDrawCentered(&g_sContext, cMenuString[ui8Task_Status]+3, AUTO_STRING_LENGTH, 62, 7, OPAQUE_TEXT);
 	GrRectDraw(&g_sContext, &sRactangle1);
 	GrRectDraw(&g_sContext, &sRactangle2);
 }
 /*
  * 绘制任务坐标轴
  * =====================
  * ↑
  * |
  * |
  * |—————→
  * =====================
  */
 void GrAxisDraw()
 {
 	GrLineDrawV(&g_sContext, 2, 21, 61);
 	GrLineDrawH(&g_sContext, 2, 84, 61);
 	GrLineDraw(&g_sContext, 0, 23, 2, 21);
 	GrLineDraw(&g_sContext, 4, 23, 2, 21);
 	GrLineDraw(&g_sContext, 82, 59, 84, 61);
 	GrLineDraw(&g_sContext, 82, 63, 84, 61);
 }
 /*
  * 绘制柱状图
  * ================================
  * 模式一           模式二
  *  9%          XY
  *  ██          ██
  *  ██ (other)  ██ （Acce）
  * ================================
  */
 int Draw_Histogram(uint8_t ui8Mode)
 {
 	static tRectangle sRactangle4={101,24,123,63};
 	static const tRectangle sRactangle5={100,24,127,63};
 	uint8_t ui8Speed_H = 62;
 	uint8_t ui8DispMode=0;

 	ui8Speed_H =62- ui16WheelValue/85;
 	if (ui8Speed_H < 32)
 		ui8Speed_H = 32;

 	sRactangle4.sYMin = ui8Speed_H;

 	INVERSECOLOR;
 	GrRectFill(&g_sContext,&sRactangle5);
 	NORMALCOLOR;

 	GrStringDrawCentered(&g_sContext, "     ", AUTO_STRING_LENGTH, 108, ui8Speed_H-4, OPAQUE_TEXT);

 	if(ui8Mode==0)
 	{
 		GrStringNumberCentered(&g_sContext, ui8Wheel_Pecent,0,108,ui8Speed_H-4);
 		GrStringDrawCentered(&g_sContext, "%", AUTO_STRING_LENGTH, 121, ui8Speed_H-4, OPAQUE_TEXT);
 		ui8DispMode=0;
 	}
 	else if(ui8Mode==1)
 	{
 	    ui8DispMode=ui8Wheel_Pecent/30;
 		switch(ui8DispMode)
 		{
 		case 0:GrStringDrawCentered(&g_sContext, "Digt", AUTO_STRING_LENGTH, 113, ui8Speed_H-4, OPAQUE_TEXT);break;
 		case 1:GrStringDrawCentered(&g_sContext, "X_AX", AUTO_STRING_LENGTH, 113, ui8Speed_H-4, OPAQUE_TEXT);break;
 		case 2:GrStringDrawCentered(&g_sContext, "Y_AX", AUTO_STRING_LENGTH, 113, ui8Speed_H-4, OPAQUE_TEXT);break;
 		case 3:GrStringDrawCentered(&g_sContext, "Z_AX", AUTO_STRING_LENGTH, 113, ui8Speed_H-4, OPAQUE_TEXT);break;
 		default:break;
 		}

 	}
 	GrRectFill(&g_sContext,&sRactangle4);
 	return (ui8DispMode);
 }
/*
 * 坐标轴依次打点函数
 */
 static uint8_t ui8Xial = 3;
 void Draw_Waveform(uint16_t *ui16DataPecent,uint8_t ui8Num)
 {

 	uint8_t ui8Erase_Xial =3;
 	uint8_t ui8Speed_Yial[5] = {60,60,60,60,60};
 	static uint8_t ui8PreSpeed_Yial[5] = {60,60,60,60,60};
 	uint8_t i;

 	for(i=0;i<ui8Num;i++)
 	{
 		ui8Speed_Yial[i] = 60 - ui16DataPecent[i]/3;
 		if (ui8Speed_Yial[i] < 24)
 			ui8Speed_Yial[i] = 24;					//防止画出界限
 		else if(ui8Speed_Yial[i] > 60)
 			ui8Speed_Yial[i] = 60;
 	}

 	ui8Xial++;	//X坐标右移

 	INVERSECOLOR;
 	if(ui8Xial>80)
 	{
 		GrLineDrawV(&g_sContext, 81, 24,60);	//擦除待画点区域
 		ui8Xial = 3;		//X坐标回头
 		GrLineDrawV(&g_sContext, ui8Xial, 24,60);	//擦除待画点区域

 	}
 	ui8Erase_Xial = ui8Xial + 1;
 	GrLineDrawV(&g_sContext, ui8Xial, 24,60);	//擦除待画点区域
 	NORMALCOLOR;
 	for(i=0;i<ui8Num;i++)
 	{
 		if(ui8Xial==3)
 		GrLineDrawV(&g_sContext, ui8Erase_Xial, ui8Speed_Yial[i], ui8Speed_Yial[i]);
 		else
 		GrLineDraw(&g_sContext,ui8Xial,ui8PreSpeed_Yial[i],ui8Erase_Xial,ui8Speed_Yial[i]);
 	}
 	for(i=0;i<ui8Num;i++)
 	{
 		ui8PreSpeed_Yial[i]=ui8Speed_Yial[i];
 	}
 }
 /*
  * 擦除显示区域
  */
 void EarseDispArea()
 {
 	int8_t i;
 	ui8Xial = 3;	//画图坐标重置
 	INVERSECOLOR;
 	for(i=4;i<=81;i++)
 		GrLineDrawV(&g_sContext, i, 24,60);	//擦除待画点区域
 	NORMALCOLOR;
 }
