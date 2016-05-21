/*
 * Music_Display.h
 *
 *  Created on: 2014-5-26
 *      Author: zdl
 */

#ifndef MUSIC_DISPLAY_H_
#define MUSIC_DISPLAY_H_

//-----OS专用函数-----
extern void Music_Graph();
extern void Music_Begin();
extern void Music_Main();
extern void Music_Quit();

//-----音频文件采样率-----
#define SAMPLE_FREQ 	11025

extern uint8_t Music_Enter;

#endif /* MUSIC_DISPLAY_H_ */
