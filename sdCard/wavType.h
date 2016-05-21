/*
 * wavType.h
 *
 *  Created on: 2014-2-10
 *      Author: Micle
 */

#ifndef WAVTYPE_H_
#define WAVTYPE_H_
#include "global.h"

#define USE_TIME_COUNT			// 使用显示播放时间
//wave文件头 （共36个字节）
//typedef struct WaveHeader
//{
//    uint8_t riff[4];             //资源交换文件标志(4 Byte)
//    uint32_t size;               //从下个地址开始到文件结尾的字节数(4 Byte)
//    uint8_t wave_flag[4];        //"wave"文件标识(4 Byte)
//    uint8_t fmt[4];              //波形格式标识(4 Byte "fmt ")
//    uint32_t fmt_len;            //过滤字节(一般为00000010H)(4 Byte)
//    uint16_t tag;                //格式种类，值为1时，表示PCM线性编码(2 Byte)
//    uint16_t channels;           //通道数，单声道为1，双声道为2 (2 Byte)
//    uint32_t samp_freq;          //采样频率(4 Byte)
//    uint32_t byte_rate;          //数据传输率 (每秒字节＝采样频率×每次采样大小) (4 Byte)
//    uint16_t block_align;        //块对齐字节数 = channles * bit_samp / 8 (2 Byte)
//    uint16_t bit_samp;           //bits per sample (样本数据位数，又称量化位数) (2 Byte)
//} wave_header_t;
// wav音频文件格式 （共44个字节）
typedef union {
    uint8_t wavInfo[44];
    struct {
		uint8_t riff[4];             //资源交换文件标志(4 Byte)
		uint32_t size;               //从下个地址开始到文件结尾的字节数(4 Byte)
		uint8_t wave_flag[4];        //"wave"文件标识(4 Byte)
		uint8_t fmt[4];              //波形格式标识(4 Byte "fmt ")
		uint32_t fmt_len;            //过滤字节(一般为00000010H)(4 Byte)
		uint16_t tag;                //格式种类，值为1时，表示PCM线性编码(2 Byte)
		uint16_t channels;           //通道数，单声道为1，双声道为2 (2 Byte)
		uint32_t samp_freq;          //采样频率(4 Byte)
		uint32_t byte_rate;          //数据传输率 (每秒字节＝采样频率×每次采样大小) (4 Byte)
		uint16_t block_align;        //块对齐字节数 = channles * bit_samp / 8 (2 Byte)
		uint16_t bit_samp;           //bits per sample (样本数据位数，又称量化位数) (2 Byte)
        uint8_t 	data_flag[4];      //数据标识符	 (4 Byte)
        uint32_t 	length;            //采样数据总数   (4 Byte)
    } wave_t;
} waveFormat;

//这个结构主要描述文件的状态信息，包括文件名3个字符时间。
typedef struct {
	uint8_t	hour;
	uint8_t	minute;
	uint8_t	second;
} play_time;
enum{
	MONO,STEREO
};//声道选择

extern uint8_t rcd_ply_sel ;								// 录音 - 放音 选择 1: 录音
extern uint8_t rcd_ply_flag;
extern uint8_t	volume;
#define START 1
#define END 0
// ---放于main函数中的函数---
extern FRESULT WaveOpen(char* fileName,int mono_stereo);// 打开一个wav音频文件
extern FRESULT WaveMake(char* fileName);				// 生成一个wave音频文件
// ---放于11025频率下Timer中的函数---
extern void waveData2DAC(uint8_t style);					// wav格式文件播放接口函数（放在timer中还原音频，移植改动底层）
extern void record_WaveFile(void);						// 录制wav格式文件 接口函数（放在timer中，录制wav音频，移植改动底层）

#define RECORD_STOP 2
#define RECORD_BEGIN 1
#define DISPPLAY_BEGIN 0
#define DISPPLAY_STOP 3

#endif /* WAVTYPE_H_ */
