/*
 * wavType.h
 *
 *  Created on: 2014-2-10
 *      Author: Micle
 */

#ifndef WAVTYPE_H_
#define WAVTYPE_H_
#include "global.h"

#define USE_TIME_COUNT			// ʹ����ʾ����ʱ��
//wave�ļ�ͷ ����36���ֽڣ�
//typedef struct WaveHeader
//{
//    uint8_t riff[4];             //��Դ�����ļ���־(4 Byte)
//    uint32_t size;               //���¸���ַ��ʼ���ļ���β���ֽ���(4 Byte)
//    uint8_t wave_flag[4];        //"wave"�ļ���ʶ(4 Byte)
//    uint8_t fmt[4];              //���θ�ʽ��ʶ(4 Byte "fmt ")
//    uint32_t fmt_len;            //�����ֽ�(һ��Ϊ00000010H)(4 Byte)
//    uint16_t tag;                //��ʽ���ֵ࣬Ϊ1ʱ����ʾPCM���Ա���(2 Byte)
//    uint16_t channels;           //ͨ������������Ϊ1��˫����Ϊ2 (2 Byte)
//    uint32_t samp_freq;          //����Ƶ��(4 Byte)
//    uint32_t byte_rate;          //���ݴ����� (ÿ���ֽڣ�����Ƶ�ʡ�ÿ�β�����С) (4 Byte)
//    uint16_t block_align;        //������ֽ��� = channles * bit_samp / 8 (2 Byte)
//    uint16_t bit_samp;           //bits per sample (��������λ�����ֳ�����λ��) (2 Byte)
//} wave_header_t;
// wav��Ƶ�ļ���ʽ ����44���ֽڣ�
typedef union {
    uint8_t wavInfo[44];
    struct {
		uint8_t riff[4];             //��Դ�����ļ���־(4 Byte)
		uint32_t size;               //���¸���ַ��ʼ���ļ���β���ֽ���(4 Byte)
		uint8_t wave_flag[4];        //"wave"�ļ���ʶ(4 Byte)
		uint8_t fmt[4];              //���θ�ʽ��ʶ(4 Byte "fmt ")
		uint32_t fmt_len;            //�����ֽ�(һ��Ϊ00000010H)(4 Byte)
		uint16_t tag;                //��ʽ���ֵ࣬Ϊ1ʱ����ʾPCM���Ա���(2 Byte)
		uint16_t channels;           //ͨ������������Ϊ1��˫����Ϊ2 (2 Byte)
		uint32_t samp_freq;          //����Ƶ��(4 Byte)
		uint32_t byte_rate;          //���ݴ����� (ÿ���ֽڣ�����Ƶ�ʡ�ÿ�β�����С) (4 Byte)
		uint16_t block_align;        //������ֽ��� = channles * bit_samp / 8 (2 Byte)
		uint16_t bit_samp;           //bits per sample (��������λ�����ֳ�����λ��) (2 Byte)
        uint8_t 	data_flag[4];      //���ݱ�ʶ��	 (4 Byte)
        uint32_t 	length;            //������������   (4 Byte)
    } wave_t;
} waveFormat;

//����ṹ��Ҫ�����ļ���״̬��Ϣ�������ļ���3���ַ�ʱ�䡣
typedef struct {
	uint8_t	hour;
	uint8_t	minute;
	uint8_t	second;
} play_time;
enum{
	MONO,STEREO
};//����ѡ��

extern uint8_t rcd_ply_sel ;								// ¼�� - ���� ѡ�� 1: ¼��
extern uint8_t rcd_ply_flag;
extern uint8_t	volume;
#define START 1
#define END 0
// ---����main�����еĺ���---
extern FRESULT WaveOpen(char* fileName,int mono_stereo);// ��һ��wav��Ƶ�ļ�
extern FRESULT WaveMake(char* fileName);				// ����һ��wave��Ƶ�ļ�
// ---����11025Ƶ����Timer�еĺ���---
extern void waveData2DAC(uint8_t style);					// wav��ʽ�ļ����Žӿں���������timer�л�ԭ��Ƶ����ֲ�Ķ��ײ㣩
extern void record_WaveFile(void);						// ¼��wav��ʽ�ļ� �ӿں���������timer�У�¼��wav��Ƶ����ֲ�Ķ��ײ㣩

#define RECORD_STOP 2
#define RECORD_BEGIN 1
#define DISPPLAY_BEGIN 0
#define DISPPLAY_STOP 3

#endif /* WAVTYPE_H_ */
