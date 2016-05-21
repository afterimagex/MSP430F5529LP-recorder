/*
 * wavType.c
 *
 *  Created on: 2014-2-10
 *      Author: Micle
 */
#include "wavType.h"
#include "string.h"

#ifdef USE_TIME_COUNT
static uint8_t timeLock = 0;				// �ļ���������
#endif
uint8_t rcd_ply_flag=0;
uint8_t rcd_ply_sel = 0;					// 0 - ���� ѡ�� 1: ¼��
uint8_t	volume=0;
static void diplayWaveTime(uint32_t totalTime,uint32_t tCount);
static int myStrcmp(uint8_t *str1, const char *str2);
/***************************************************************************//**
 * @brief   ��һ��wav��Ƶ�ļ� open *.wav file
 * @param   fileName:�ļ���
 * 			mono_stereo������ѡ�� momo:��������stereo:������
 * @return  None
 * ������WaveOpen("new/micle.wav",MONO);    //������ģʽ ��ȡnew�ļ����е��ļ�micle.wav
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
    waveFormat wave;									   // ��Ƶ�ļ�
                                                           // accessing the SD card

    f_mount(0, &fatfs);                                    /* Register volume work area (never
                                                               *fails) */
   // ��ȡ wave �ļ�ͷ
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
   // �ж��Ƿ��� wave �ļ�
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
       // ��¼�ļ���Ҫ���ŵ�ʱ��
       totalTime = (wave.wave_t.size-44)/wave.wave_t.byte_rate;
   #endif

       // �������ݻ�ԭʱ��Ƶ�ʣ���ֲʱ��Ҫ�Ķ���
       TA2CCR0 = SYSSMCLK/wave.wave_t.samp_freq - 1;
       init_sFIFO();					// ��ֲ����ĵĴ��벿��
       rcd_ply_flag=START;
       for (;;)
       {
       	if(rcd_ply_sel != 0)								// ����ǰ״̬Ϊ¼��ģʽ���򷵻�
       		break;

           rc = f_read(&fil, buff, sizeof(buff), &br);         /* ��ȡ�ļ���һ������������Ϊ�˽�С������û���޳���֡ͷ����Ч���ݣ� */

           switch(mono_stereo)
           {
           case MONO:  //--------��������ȡ��ʽ----------
   			for(i=0;i<512/(wave.wave_t.channels);i=i+wave.wave_t.block_align)
   			{
   				// ��forѭ���з��õ������ݴ����������ݵ�����ӿں������ڴ˴�
   				while(FIFO_Count == FIFOBUFF_SIZE && rcd_ply_sel == 0)
   				{
   					//=====�˴��ɷ�����ʾ����,������Ӱ�첥��Ч��=====
   					diplayWaveTime(totalTime,nowTime/1000);
   					//P1OUT ^= BIT0;								// �ȴ������п������������������ֲʱ��Ҫ�Ķ���
   				}

   				for(j=0;j<wave.wave_t.block_align;j++)		// ���ݡ�����롱��8λ��16λ���������������ռ�õ�λ��
   				{
   					Write_FIFO(buff[i+j]);							// ��������ӿں�������ǰ�����ǽ��������� д��FIFO������ֲʱ��Ҫ�Ķ���
   				}
   			}
           	break;
           case STEREO: //--------��������ȡ��ʽ ����������----------
           	break;
           default:break;
           }
   #ifdef USE_TIME_COUNT
           // ��Լ���� ÿ512 ���ֽڣ���Ҫ����msʱ��
           nowTime += 512000/wave.wave_t.byte_rate;										// �ۼ�ʱ��ֵ
   #endif
           if (rc || !br)			 							// �ж��Ƿ��ļ���ȡ���
           break;                             				 	/* Error or end of file */
       }
       rcd_ply_flag=END;
   #ifdef USE_TIME_COUNT
   	timeLock = 0;											// ʱ�����
   #endif

       rc = f_close(&fil);                                     // accessing the SD card
       f_mount(0, NULL);
       return rc;
}
/***************************************************************************//**
 * @brief   ����һ��wav��Ƶ�ļ�make a *.wav file
 * @param   fileName:��Ҫ�������ļ���
 * @return  None
 * ������WaveMake("new/micle.wav");    //new�ļ����д���һ�� ������ģʽwav 16λ �ļ�micle.wav
 ******************************************************************************/
FRESULT WaveMake(char* fileName)
{
	uint16_t i=0;
	uint32_t nowTime=0,totalByte = 0;		// ʱ�������ֽ�������
	uint8_t buff[512]={0}; //512
    UINT bw=1;
    FRESULT rc;                                            /* Result code */
    FATFS fatfs;                                           /* File system object */
    FIL	fil;
    waveFormat wave; 									   // ��Ƶ�ļ�

    //========================�����ļ�ͷ��Ϣ===================================
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
	wave.wave_t.bit_samp = 16;					   // ��������λ��
														  // accessing the SD card
	f_mount(0, &fatfs);                                    /* Register volume work area (never
//														   *fails) */
	// ��ȡ wave �ļ�ͷ
	rc = f_open(&fil, fileName, FA_WRITE | FA_CREATE_ALWAYS);// �򿪷�ʽ:�½��ļ�������ļ��Ѿ����ڣ����Ǿ��ļ�
	if (rc)													// �ж��Ƿ񴴽��ɹ�
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
   	}while(bw<44);									// �ж��Ƿ������ͷ�ļ�


       //========================��ʼд��¼����Ϣ===================================
	init_sFIFO();									// ��ֲ����ĵĴ��벿��
	rcd_ply_flag=START;
	do
	{
		while(FIFO_Count < 512 && rcd_ply_sel == 1)// �ж�FIFO�������Ƿ�ﵽ512��������д��һ��������
		{
			//=====�˴��ɷ�����ʾ����,������Ӱ�첥��Ч��=====
//			diplayWaveTime(0,nowTime/1000);
		}

		for(i=0;i<512;i++)
			buff[i] = Read_FIFO();
		rc = f_write(&fil, buff, 512, &bw);
		totalByte += 1;								// ͳ�Ƶ�ǰ��д�������������
		if (rc)
		{
		   rc = f_close(&fil);
		   return rc;
		}
#ifdef USE_TIME_COUNT
		nowTime += 512000/wave.wave_t.byte_rate;	// // ��Լ���� ÿ512 ���ֽڣ���Ҫ����msʱ��
#endif
       }while(rcd_ply_sel == 1);

#ifdef USE_TIME_COUNT
	timeLock = 0;										// ����ǰ�ļ�ʱ�����
#endif
       // Close the file
	rcd_ply_flag=END;
	rc = f_close(&fil);


	//======================����ͷ�ļ����������� ��Ϣ===============================
	wave.wave_t.length = totalByte*512;
	wave.wave_t.size = wave.wave_t.length + 44 - 8;
	// ��ȡ wave �ļ�ͷ
	rc = f_open(&fil, fileName, FA_WRITE);				// �򿪷�ʽ:��д���ļ�
	if (rc)												// �ж��Ƿ񴴽��ɹ�
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
   	}while(bw<44);			// �ж��Ƿ������ͷ�ļ�
       // Close the file
       rc = f_close(&fil);
     //  f_mount(0, NULL);
       return rc;
}
/***************************************************************************
 * @brief    wav��ʽ�ļ����Žӿں���������timer�л�ԭ��Ƶ����ֲ�Ķ��ײ㣩
 * @param   style: �����ļ����� 0��wavԭ���ļ���1�� ¼���ļ�
 * @return  None
 * ������waveData2DAC(ptime,tCount);    //������ʱ�䴦��
 ******************************************************************************/
void waveData2DAC(uint8_t style)
{
	uint16_t temp = 0;
	uint16_t  tempL=0,tempH=0;
	// �˴�������ֲ���������ʽ����ǰΪ��ȡFIFO����DAC��ԭ
	if(FIFO_Count > 2)
	{
		tempL =  Read_FIFO();			//��FIFO�ж�ȡ����
		tempH =  Read_FIFO();
		switch(style)
		{
		case 0:// ԭ����Ƶ�ļ����ݸ�ʽת��
			temp =((tempH<<8) &0xff00)|(tempL & 0x00ff);
			temp = temp + 0x8000;
			temp = (temp>>4) & 0x0FFF;
			break;
		case 1:// ¼���ļ����ݸ�ʽת��
			temp =((tempH<<8) & 0x0f00)|((tempL) & 0x00ff);
			break;
		default:break;
		}
		if(ui8Task_Status==10)
		{

			temp=(temp>>4)>>volume;
		}
		write2DAC(temp);
	 //   P4OUT ^= BIT7;					// �忨P4.7�ܽ����LED�̵�
	}
	else
		;//P4OUT ^= BIT7;					// �忨P4.7�ܽ����LED�̵ƣ����ڲ���FIFO��С�Ƿ��ã�
}
/***************************************************************************
 * @brief   ¼��wav��ʽ�ļ� �ӿں���������timer�У�¼��wav��Ƶ����ֲ�Ķ��ײ㣩
 * @param   ��
 * @return  None
 * ������record_WaveFile(void);    //¼����Ƶ�ļ�
 ******************************************************************************/
void record_WaveFile(void)
{
	uint16_t temp = 0;
	uint16_t  tempL=0,tempH=0;
	if(FIFO_Count < FIFOBUFF_SIZE-2)	 // ��ֹFIFO���
	{
		temp = ADC12_SingleSample();
		// ��ǰADCΪ12λ��������Ҫ����ת��
		temp = (temp<<4) - 0x8000;
		tempL = temp & 0xff;
		tempH = (temp>>8) &0xff;
		Write_FIFO(tempL);
		Write_FIFO(tempH);
//	    P4OUT ^= BIT7;					// �忨P4.7�ܽ����LED�̵�
	}
}
/***************************************************************************
 * @brief   wav��Ƶ�ļ�����ʱ�䴦��
 * @param   totalTime:�ļ�������ʱ��
 * 			tCount���Ѳ���ʱ��(��λs)
 * @return  None
 * ������diplayWaveTime(ptime,tCount);    //������ʱ�䴦��
 ******************************************************************************/
static void diplayWaveTime(uint32_t totalTime,uint32_t tCount)
{
#ifdef USE_TIME_COUNT
//	uint8_t baseX = 35;										// ʱ����ʾλ��
//	uint8_t baseY = 57;
    play_time pTime;
    static uint8_t second=255;									   // ���岥��ʱ��
    if(timeLock == 0)
    {
    	timeLock = 1;										// ����
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
// ======����������Լ����ĺ���ַ����ȽϺ���,�ʺ����������ַ����Ƚ�=====
// �ַ�������ͬ������0�� ��ͬ������1
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



