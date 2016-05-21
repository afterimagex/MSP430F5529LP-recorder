/*
 * wavType.c
 *
 *  Created on: 2014-2-10
 *      Author: Micle
 */
#include "wavType.h"
#include "string.h"

#ifdef USE_TIME_COUNT
static uint8_t timeLock = 0;				// 文件播放上锁
#endif
uint8_t rcd_ply_flag=0;
uint8_t rcd_ply_sel = 0;					// 0 - 放音 选择 1: 录音
uint8_t	volume=0;
static void diplayWaveTime(uint32_t totalTime,uint32_t tCount);
static int myStrcmp(uint8_t *str1, const char *str2);
/***************************************************************************//**
 * @brief   打开一个wav音频文件 open *.wav file
 * @param   fileName:文件名
 * 			mono_stereo：声道选择 momo:单声道，stereo:立体声
 * @return  None
 * 范例：WaveOpen("new/micle.wav",MONO);    //单声道模式 读取new文件夹中的文件micle.wav
 ******************************************************************************/
FRESULT WaveOpen(char* fileName,int mono_stereo)
{
	uint8_t buff[512]={0}; //512
	uint16_t i=0;
	uint32_t totalTime=0,nowTime=0;
	uint8_t j=0;
    UINT br=1;											   //,bw=1
    FRESULT rc;                                            /* Result code */
    FATFS fatfs;                                           /* File system object */
    FIL	fil;
    waveFormat wave;									   // 音频文件
                                                           // accessing the SD card

    f_mount(0, &fatfs);                                    /* Register volume work area (never
                                                               *fails) */
   // 读取 wave 文件头
   rc = f_open(&fil, fileName, FA_READ|FA_OPEN_EXISTING);
   if(!rc)
   {
	   rc = f_read(&fil, wave.wavInfo, 44, &br);        	/* Read a chunk of file */
   }
   else                                                	//if error
   {
	   f_close(&fil);                                      // accessing the SD card
	   return rc;
   }
   // 判断是否是 wave 文件
   if(myStrcmp(wave.wave_t.riff,"RIFF") ||
		myStrcmp(wave.wave_t.wave_flag,"WAVE")||
		myStrcmp(wave.wave_t.fmt,"fmt ")
		//||myStrcmp(wave.wave_t.data_flag,"data")
		)
   {
	   rc = f_close(&fil);                                     // accessing the SD card
	   return rc;
   }
   #ifdef USE_TIME_COUNT
       // 记录文件需要播放的时间
       totalTime = (wave.wave_t.size-44)/wave.wave_t.byte_rate;
   #endif

       // 配置数据还原时钟频率（移植时需要改动）
       TA2CCR0 = SYSSMCLK/wave.wave_t.samp_freq - 1;
       init_sFIFO();					// 移植需更改的代码部分
       rcd_ply_flag=START;
       for (;;)
       {
       	if(rcd_ply_sel != 0)								// 若当前状态为录音模式，则返回
       		break;

           rc = f_read(&fil, buff, sizeof(buff), &br);         /* 读取文件的一个扇区（这里为了较小代码量没有剔除“帧头”无效数据） */

           switch(mono_stereo)
           {
           case MONO:  //--------单声道读取方式----------
   			for(i=0;i<512/(wave.wave_t.channels);i=i+wave.wave_t.block_align)
   			{
   				// 本for循环中放置的是数据处理函数，数据的输出接口函数放于此处
   				while(FIFO_Count == FIFOBUFF_SIZE && rcd_ply_sel == 0)
   				{
   					//=====此处可放置显示代码,而不会影响播放效果=====
   					diplayWaveTime(totalTime,nowTime/1000);
   					//P1OUT ^= BIT0;								// 等待过程中可添加其他处理函数（移植时需要改动）
   				}

   				for(j=0;j<wave.wave_t.block_align;j++)		// 根据“块对齐”，8位或16位，抉择“量化结果”占用的位数
   				{
   					Write_FIFO(buff[i+j]);							// 数据输出接口函数（当前功能是将缓存数据 写入FIFO）（移植时需要改动）
   				}
   			}
           	break;
           case STEREO: //--------立体声读取方式 可以自增加----------
           	break;
           default:break;
           }
   #ifdef USE_TIME_COUNT
           // 大约计算 每512 个字节，需要多少ms时间
           nowTime += 512000/wave.wave_t.byte_rate;										// 累计时间值
   #endif
           if (rc || !br)			 							// 判断是否文件读取完毕
           break;                             				 	/* Error or end of file */
       }
       rcd_ply_flag=END;
   #ifdef USE_TIME_COUNT
   	timeLock = 0;											// 时间解锁
   #endif

       rc = f_close(&fil);                                     // accessing the SD card
       f_mount(0, NULL);
       return rc;
}
/***************************************************************************//**
 * @brief   创建一个wav音频文件make a *.wav file
 * @param   fileName:需要创建的文件名
 * @return  None
 * 范例：WaveMake("new/micle.wav");    //new文件夹中创建一个 单声道模式wav 16位 文件micle.wav
 ******************************************************************************/
FRESULT WaveMake(char* fileName)
{
	uint16_t i=0;
	uint32_t nowTime=0,totalByte = 0;		// 时间与总字节数变量
	uint8_t buff[512]={0}; //512
    UINT bw=1;
    FRESULT rc;                                            /* Result code */
    FATFS fatfs;                                           /* File system object */
    FIL	fil;
    waveFormat wave; 									   // 音频文件

    //========================创建文件头信息===================================
    strcpy(wave.wave_t.riff,"RIFF");
    strcpy(wave.wave_t.wave_flag,"WAVE");
    strcpy(wave.wave_t.fmt,"fmt ");
    strcpy(wave.wave_t.data_flag,"data");

	wave.wave_t.fmt_len=0x10;
	wave.wave_t.tag = 1;
	wave.wave_t.channels = 1;
	wave.wave_t.samp_freq = 11025;
	wave.wave_t.byte_rate = 22050;
	wave.wave_t.block_align = 2;
	wave.wave_t.bit_samp = 16;					   // 样本数据位数
														  // accessing the SD card
	f_mount(0, &fatfs);                                    /* Register volume work area (never
//														   *fails) */
	// 读取 wave 文件头
	rc = f_open(&fil, fileName, FA_WRITE | FA_CREATE_ALWAYS);// 打开方式:新建文件，如果文件已经存在，覆盖旧文件
	if (rc)													// 判断是否创建成功
	{
	  rc = f_close(&fil);
	  return rc;
	}
	do
   	{
   		// Write to wav file header
   		rc = f_write(&fil, wave.wavInfo, 44, &bw);
   		if (rc)
   		{
   		   rc = f_close(&fil);
   		   return rc;
   		}
   	}while(bw<44);									// 判断是否更改完头文件


       //========================开始写入录音信息===================================
	init_sFIFO();									// 移植需更改的代码部分
	rcd_ply_flag=START;
	do
	{
		while(FIFO_Count < 512 && rcd_ply_sel == 1)// 判断FIFO中数据是否达到512个（正好写入一个扇区）
		{
			//=====此处可放置显示代码,而不会影响播放效果=====
//			diplayWaveTime(0,nowTime/1000);
		}

		for(i=0;i<512;i++)
			buff[i] = Read_FIFO();
		rc = f_write(&fil, buff, 512, &bw);
		totalByte += 1;								// 统计当前已写入的中扇区个数
		if (rc)
		{
		   rc = f_close(&fil);
		   return rc;
		}
#ifdef USE_TIME_COUNT
		nowTime += 512000/wave.wave_t.byte_rate;	// // 大约计算 每512 个字节，需要多少ms时间
#endif
       }while(rcd_ply_sel == 1);

#ifdef USE_TIME_COUNT
	timeLock = 0;										// 给当前文件时间解锁
#endif
       // Close the file
	rcd_ply_flag=END;
	rc = f_close(&fil);


	//======================更改头文件“数据量” 信息===============================
	wave.wave_t.length = totalByte*512;
	wave.wave_t.size = wave.wave_t.length + 44 - 8;
	// 读取 wave 文件头
	rc = f_open(&fil, fileName, FA_WRITE);				// 打开方式:仅写入文件
	if (rc)												// 判断是否创建成功
	{
	  rc = f_close(&fil);
	  return rc;
	}
	do
   	{
		// Write to wav file header
		rc = f_write(&fil, wave.wavInfo, 44, &bw);
		if (rc)
		{
		   rc = f_close(&fil);
		   return rc;
		}
   	}while(bw<44);			// 判断是否更改完头文件
       // Close the file
       rc = f_close(&fil);
     //  f_mount(0, NULL);
       return rc;
}
/***************************************************************************
 * @brief    wav格式文件播放接口函数（放在timer中还原音频，移植改动底层）
 * @param   style: 播放文件类型 0：wav原声文件，1： 录音文件
 * @return  None
 * 范例：waveData2DAC(ptime,tCount);    //播放器时间处理
 ******************************************************************************/
void waveData2DAC(uint8_t style)
{
	uint16_t temp = 0;
	uint16_t  tempL=0,tempH=0;
	// 此处用于移植更改输出方式，当前为读取FIFO后送DAC还原
	if(FIFO_Count > 2)
	{
		tempL =  Read_FIFO();			//从FIFO中读取数据
		tempH =  Read_FIFO();
		switch(style)
		{
		case 0:// 原声音频文件数据格式转换
			temp =((tempH<<8) &0xff00)|(tempL & 0x00ff);
			temp = temp + 0x8000;
			temp = (temp>>4) & 0x0FFF;
			break;
		case 1:// 录音文件数据格式转换
			temp =((tempH<<8) & 0x0f00)|((tempL) & 0x00ff);
			break;
		default:break;
		}
		if(ui8Task_Status==10)
		{

			temp=(temp>>4)>>volume;
		}
		write2DAC(temp);
	 //   P4OUT ^= BIT7;					// 板卡P4.7管脚外接LED绿灯
	}
	else
		;//P4OUT ^= BIT7;					// 板卡P4.7管脚外接LED绿灯（用于测试FIFO大小是否够用）
}
/***************************************************************************
 * @brief   录制wav格式文件 接口函数（放在timer中，录制wav音频，移植改动底层）
 * @param   无
 * @return  None
 * 范例：record_WaveFile(void);    //录制音频文件
 ******************************************************************************/
void record_WaveFile(void)
{
	uint16_t temp = 0;
	uint16_t  tempL=0,tempH=0;
	if(FIFO_Count < FIFOBUFF_SIZE-2)	 // 防止FIFO溢出
	{
		temp = ADC12_SingleSample();
		// 当前ADC为12位，所以需要数据转换
		temp = (temp<<4) - 0x8000;
		tempL = temp & 0xff;
		tempH = (temp>>8) &0xff;
		Write_FIFO(tempL);
		Write_FIFO(tempH);
//	    P4OUT ^= BIT7;					// 板卡P4.7管脚外接LED绿灯
	}
}
/***************************************************************************
 * @brief   wav音频文件播放时间处理
 * @param   totalTime:文件播放总时长
 * 			tCount：已播放时间(单位s)
 * @return  None
 * 范例：diplayWaveTime(ptime,tCount);    //播放器时间处理
 ******************************************************************************/
static void diplayWaveTime(uint32_t totalTime,uint32_t tCount)
{
#ifdef USE_TIME_COUNT
//	uint8_t baseX = 35;										// 时间显示位置
//	uint8_t baseY = 57;
    play_time pTime;
    static uint8_t second=255;									   // 定义播放时间
    if(timeLock == 0)
    {
    	timeLock = 1;										// 锁定
        pTime.hour = (uint8_t)(totalTime / 3600);
        pTime.minute = (uint8_t)((totalTime / 60) % 60);
        pTime.second =  (uint8_t)(totalTime % 60);
//		GrStringDrawCentered(&g_sContext, "  ", AUTO_STRING_LENGTH, baseX+48, baseY, OPAQUE_TEXT);
//		GrStringNumberCentered(&g_sContext,pTime.minute,0,baseX+48,baseY);
//		GrStringDrawCentered(&g_sContext, "  ", AUTO_STRING_LENGTH, baseX+70, baseY, OPAQUE_TEXT);
//		GrStringNumberCentered(&g_sContext,pTime.second,0,baseX+70,baseY);
    }
    pTime.minute = (uint8_t)((tCount / 60) % 60);
    pTime.second =  (uint8_t)(tCount % 60);
	if(pTime.second != second)
	{

//		GrStringDrawCentered(&g_sContext, "  ", AUTO_STRING_LENGTH, baseX+2 , baseY, OPAQUE_TEXT);
//		GrStringNumberCentered(&g_sContext,pTime.minute,0,baseX+2,baseY);
//		GrStringDrawCentered(&g_sContext, "  ", AUTO_STRING_LENGTH, baseX+22, baseY, OPAQUE_TEXT);
//		GrStringNumberCentered(&g_sContext,pTime.second,0,baseX+22,baseY);
		second = pTime.second;
	}

#endif // end of #ifdef USE_TIME_COUNT
}
// ======这里加入了自己更改后的字符串比较函数,适合联合体内字符串比较=====
// 字符串：相同，返回0， 不同，返回1
static int myStrcmp(uint8_t *str1, const char *str2)
{
    while (*str1==*str2)
    {
        if(*str1=='\0')
            return 0;
        str1++;
        str2++;
        if(*str2=='\0')
            return 0;
     }
     return 1;
}



