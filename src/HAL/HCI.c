/*
 * HCI.c
 *
 *  Created on: 2014-4-28
 *      Author: rainf_000
 */
#include"global.h"

//---------------------相关宏定义-------------------------

#define   	NUM_OF_RESULTS   	4

//---------------------相关变量定义-----------------------
uint16_t 	ui16WheelValue = 0;
uint8_t 	ui8ButtonKeyValue = 0;
uint8_t ui8Wheel_Pecent = 0;

//---------------------局部函数声明---------------------
static void ButtomKey_init();
static void P23_Onclick();
static void P26_Onclick();


void WheelADC_init()
{
	//	--------初始化ADC12--------
		GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
					GPIO_PIN4);
		ADC12_A_init(ADC12_A_BASE,				// 初始化ADC12，
				ADC12_A_SAMPLEHOLDSOURCE_SC,
				ADC12_A_CLOCKSOURCE_ADC12OSC,
				ADC12_A_CLOCKDIVIDER_1);
		ADC12_A_enable(ADC12_A_BASE);			// 打开ADC12
		ADC12_A_setupSamplingTimer(ADC12_A_BASE,// 建立采样定时器控制采样保持时间，16个clock
				ADC12_A_CYCLEHOLD_64_CYCLES,
				ADC12_A_CYCLEHOLD_4_CYCLES,
				ADC12_A_MULTIPLESAMPLESDISABLE);
		ADC12_A_memoryConfigure(ADC12_A_BASE,
				ADC12_A_MEMORY_0,
				ADC12_A_INPUT_A4,
				ADC12_A_VREFPOS_AVCC,				// VREF+ = AVCC
				ADC12_A_VREFNEG_AVSS,				// VREF- = AVSS
				ADC12_A_ENDOFSEQUENCE);
		//Enable memory buffer 0 interrupt
		ADC12_A_clearInterrupt(ADC12_A_BASE,
				ADC12_A_IFG0);
		ADC12_A_enableInterrupt(ADC12_A_BASE,
		        ADC12IE0);
		 __delay_cycles(75);
}

//=======放在AD12中断中调用========
void Wheel_ADC()
{
	static uint8_t index = 0;
	static uint16_t results[NUM_OF_RESULTS] ={0};
	results[index] = ADC12MEM0;             // Move results
	index++;                              // Increment results index, modulo; Set Breakpoint1 here
	if (index == NUM_OF_RESULTS)
	{
		uint8_t i;
		uint16_t ui16Sum = 0;

		for(i=0;i<NUM_OF_RESULTS;i++)
		 {
			 ui16Sum += results[i];
		 }

		ui16WheelValue = ui16Sum/NUM_OF_RESULTS;
		if(ui16WheelValue>MAX_WHEEL_VALUE)
			ui16WheelValue=MAX_WHEEL_VALUE;
		ui8Wheel_Pecent = ui16WheelValue/(MAX_WHEEL_VALUE/100);
		if (ui8Wheel_Pecent > 100)
			ui8Wheel_Pecent = 100;
		volume=10-ui8Wheel_Pecent/10;
		index = 0;
		ADC12CTL0 &= ~(ADC12ENC+ADC12SC);              // Disable conversions
	}
 }

//=======放在WDT中断中调用========
void Wheel_ADC_Begin()
{
//	ADC12CTL0 |= ADC12ENC+ADC12SC; 					//Enable conversions+	Start conversion
    ADC12_A_startConversion(ADC12_A_BASE,
        ADC12_A_MEMORY_0,
        ADC12_A_SINGLECHANNEL);
}

//===========判断滚轮键值，需要时调用即可=============
uint8_t GetWheelKey()
{
	return (ui16WheelValue/UNIT_KEY+1);
}

static void ButtomKey_init()
{
	//-----配合机械按键，启用内部上拉电阻-----
	P2REN |= BIT3+BIT6;				        //启用P2.3 P2.6内部上下拉电阻
	P2OUT |= BIT3+BIT6;			            //将电阻设置为上拉
	//-----配置P2.3中断参数-----
//	P2DIR &= ~(BIT3+BIT6);                  // P2.3 P2.6设为输入(可省略)
//	P2IES |= BIT3+BIT6;                     // P2.3 P2.6设为下降沿中断
//	P2IE  |= BIT3+BIT6;                     // 允许P2.3 P2.6中断
}

void WDT_init()
{
	WDTCTL = WDT_ADLY_16;
	SFRIE1 |= WDTIE;
}
void Key_init()
{
	WheelADC_init();
	ButtomKey_init();
	WDT_init();
}
/*
//==========中断法判断机械按键===========
void P2_IODect()
{
	unsigned int Push_Key=0;
	//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
	Push_Key=P2IFG&(~P1DIR);
	//-----延时一段时间，避开机械抖动区域-----
	__delay_cycles(10000);							//消抖延时
	//----判断按键状态是否与延时前一致-----
	if((P2IN&Push_Key)==0) 	                	//如果该次按键确实有效
   {
	 //----判断具体哪个IO被按下，调用该IO的事件处理函数-----
	  switch(Push_Key){
	 case BIT3: 	P23_Onclick();		break;
	 case BIT6: 	P26_Onclick();		break;
	 default:							break;		//任何情况下均加上default
	 }
   }
}*/
//===========定时扫描法判断机械按键，需要放在WDT中断中调用============
void P2_IODect()
{
	static unsigned char KEYP23_Now=0;	//变量值出函数时需保留
	unsigned char KEYP23_Past=0;
	static unsigned char KEYP26_Now=0;	//变量值出函数时需保留
	unsigned char KEYP26_Past=0;

	KEYP23_Past=KEYP23_Now;
	KEYP26_Past=KEYP26_Now;

	//-----查询P23的输入寄存器-----
	if(P2IN&BIT3)
		KEYP23_Now=1;
	else
		KEYP23_Now=0;

	//-----前一次高电平、后一次低电平，说明按键按下-----
	if((KEYP23_Past==1)&&(KEYP23_Now==0))
		P23_Onclick();

	//-----查询P26的输入寄存器-----
	if(P2IN&BIT6)
		KEYP26_Now=1;
	else
		KEYP26_Now=0;

	//-----前一次高电平、后一次低电平，说明按键按下-----
	if((KEYP26_Past==1)&&(KEYP26_Now==0))
		P26_Onclick();
}

static void P26_Onclick()
{
	ui8ButtonKeyValue |= KEY_LEFT_ENTER;
	if(Sub[ui8Task_Status].ui8Task_Num==TASK7)
	{
		if(rcd_ply_sel==255)
			rcd_ply_sel=1;
		else if(rcd_ply_sel==1)
				rcd_ply_sel=0;
			else
				rcd_ply_sel=255;
	}


}


static void P23_Onclick()
{

		ui8ButtonKeyValue |= KEY_RIGHT_EXIT;
		if(Sub[ui8Task_Status].ui8Task_Num==TASK7)
		{
				rcd_ply_sel=255;
		}

}

//
uint16_t ADC12_SingleSample(void)
{
	uint16_t temp=0;
	ADC12_A_startConversion(ADC12_A_BASE,
			ADC12_A_MEMORY_3,
			ADC12_A_SINGLECHANNEL);
	while(!(ADC12_A_getInterruptStatus(ADC12_A_BASE,ADC12_A_IFG3)));
	temp = ADC12_A_getResults(ADC12_A_BASE,ADC12_A_MEMORY_3);
	ADC12_A_clearInterrupt(ADC12_A_BASE,ADC12_A_IFG3);
	return temp;
}
