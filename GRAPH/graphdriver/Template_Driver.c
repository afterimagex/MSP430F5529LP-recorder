/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// Template_Driver.c - Display driver for any LCD Controller. This file serves as
//						a template for creating new LCD driver files
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// READ ME
//
// This template driver is intended to be modified for creating new LCD drivers
// It is setup so that only Template_DriverPixelDraw() and DPYCOLORTRANSLATE()
// and some LCD size configuration settings in the header file Template_Driver.h
// are REQUIRED to be written. These functions are marked with the string
// "TemplateDisplayFix" in the comments so that a search through Template_Driver.c and
// Template_Driver.h can quickly identify the necessary areas of change.
//
// Template_DriverPixelDraw() is the base function to write to the LCD
// display. Functions like WriteData(), WriteCommand(), and SetAddress()
// are suggested to be used to help implement the Template_DriverPixelDraw()
// function, but are not required. SetAddress() should be used by other pixel
// level functions to help optimize them.
// 
// This is not an optimized driver however and will significantly impact 
// performance. It is highly recommended to first get the prototypes working
// with the single pixel writes, and then go back and optimize the driver.
// Please see application note www.ti.com/lit/pdf/slaa548 for more information
// on how to fully optimize LCD driver files. In short, driver optimizations
// should take advantage of the auto-incrementing of the LCD controller. 
// This should be utilized so that a loop of WriteData() can be used instead
// of a loop of Template_DriverPixelDraw(). The pixel draw loop contains both a
// SetAddress() + WriteData() compared to WriteData() alone. This is a big time 
// saver especially for the line draws and Template_DriverPixelDrawMultiple.
// More optimization can be done by reducing function calls by writing macros,
// eliminating unnecessary instructions, and of course taking advantage of other 
// features offered by the LCD controller. With so many pixels on an LCD screen
// each instruction can have a large impact on total drawing time.
//
//*****************************************************************************


//*****************************************************************************
//
// Include Files
//
//*****************************************************************************

#include "Template_Driver.h"

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
/* Global buffer for the display. This is especially useful on 1BPP, 2BPP, and 4BPP
  displays. This allows you to update pixels while reading the neighboring pixels
  from the buffer instead of a read from the LCD controller. A buffer is not required
  as a read followed by a write can be used instead.*/
uint8_t Template_Memory[(LCD_X_SIZE * LCD_Y_SIZE * BPP) / 8];
uint8_t ui8Flush_Flag=FLUSH_ON;		//默认集中刷新COG12864的硬件缓存

#ifdef USER_CODE			//用于标示哪些是用户写的代码，哪些是模板自带代码
//*****************************************************************************
//
// ms延时
//
//*****************************************************************************
static void delay_COG12864(uint16_t ms)
{
	uint16_t k;
   for(k=0;k<ms;k++)
   	_delay_cycles(SYSCLK);					// 25000即当前时钟设在25MHz
}
/*****************************************************************************
功         能：数据移位 8bit 最高位在前）
适用范围：Template_Driver.c 内部函数
参         数： byte: 要写入的 1 Byte数据
返 回  值：   无
 *****************************************************************************/
static void SendByte(uint8_t byte)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		LCD_SCK_L;
		if((byte<<i)&0x80)
			LCD_SDA_H;
		else
			LCD_SDA_L;
		LCD_SCK_H;
	}
}
/*****************************************************************************
功         能：向COG屏幕中连续写入cnt个数据
适用范围：Template_Driver.c 内部函数
参         数： ptr: 要写入的数据指针
		cnt:指定数据个数
返 回  值：   无
 *****************************************************************************/
void Write_Ndata( uint8_t *ptr ,uint16_t cnt)
{
	uint16_t i=0;
	uint8_t temp=0;
	uint8_t *tPtr;
	tPtr = ptr;
	LCD_CS_L;
	LCD_AO_H;
	for(i=0;i<cnt;i++)		// 连续写入
	{
		temp = *tPtr++;		// 后来写入字体覆盖之前的图形
		SendByte(temp);
	}
	LCD_CS_H;
}

#endif
//*****************************************************************************
//
// Suggested functions to help facilitate writing the required functions below
//
//*****************************************************************************

// Writes data to the LCD controller
static void
WriteData(uint16_t usData)
{
	/* Write data to the LCD controller. For instance this can be bit banged 
	with 6800 or 8080 protocol or this could be the SPI routine for a SPI LCD */
#ifdef USER_CODE
	LCD_CS_L;									//打开使能
	LCD_AO_H;									//写数据，AO（RS）为高电平
	SendByte(usData);
	LCD_CS_H;
#endif

}

// Writes a command to the LCD controller
static void
WriteCommand(uint8_t ucCommand)
{
   /* This function is typically very similar (sometimes the same) as WriteData()
   The difference is that this is for the LCD to interpret commands instead of pixel
   data. For instance in 8080 protocol, this means pulling the DC line low.*/
#ifdef USER_CODE
	LCD_CS_L;									//打开使能
	LCD_AO_L;									//写指令，AO（RS）为低电平
	SendByte(ucCommand);
	LCD_CS_H;
#endif
}

#ifdef USER_CODE
static void COGClear()// (移植所需改动函数)
{
	uint8_t x,y;
	 for(y=0;y<LCD_X_SIZE/8;y++)
	  {
		WriteCommand(0xb0+y);
		WriteCommand(0x10);
	  	WriteCommand(0x00);
        for(x=0;x<LCD_X_SIZE;x++)
        {
        	WriteData(0);
        }
	  }
}
#endif
// Sets the pixel address of the LCD driver
void SetAddress(int16_t lX, int16_t lY)
{
    /* This function typically writes commands (using WriteCommand()) to the 
	LCD to tell it where to move the cursor for the next pixel to be drawn. */
#ifdef USER_CODE
		WriteCommand(0xb0+lY);
		WriteCommand(0x10+(lX/16));
		WriteCommand(0x00+(lX%16));
#endif
}


// Initializes the pins required for the GPIO-based LCD interface.
// This function configures the GPIO pins used to control the LCD display
// when the basic GPIO interface is in use. On exit, the LCD controller
// has been reset and is ready to receive command and data writes.
static void
InitGPIOLCDInterface(void)
{
    /* Initialize the hardware to configure the ports properly for communication */
#ifdef USER_CODE

	P4DIR |= BIT0;
	P3DIR |= BIT6+BIT7;
	P8DIR |= BIT2;
	P1DIR |= BIT2;

#endif
}

// Initialize DisplayBuffer.
// This function initializes the display buffer and discards any cached data.
static void
InitLCDDisplayBuffer(void *pvDisplayData, uint16_t ulValue)
{
#ifdef USER_CODE

	uint16_t i=0,j=0;
	 for(j=0;j<LCD_Y_SIZE/8;j++)
	   for(i=0;i<LCD_X_SIZE;i++)
	   	   Template_Memory[j*128+i]=ulValue;

#endif
}

// Initializes the display driver.
// This function initializes the LCD controller
//
// TemplateDisplayFix
void
Template_DriverInit(void)
{
	/*Initialize the LCD controller.
	This typically looks like:
	
    InitGPIOLCDInterface();
    InitLCDDisplayBuffer();
	// Init LCD controller parameters
	// Enable LCD
	// Init Backlight
	// Clear Screen
	*/
	InitGPIOLCDInterface();
	InitLCDDisplayBuffer(0,0);
#ifdef USER_CODE
	LCD_CS_L;
	LCD_RST_L;
	delay_COG12864(20);
	LCD_RST_H;
	delay_COG12864(20);

	WriteCommand(0xe2);	//system reset
	delay_COG12864(200);
	WriteCommand(0x24);	//SET VLCD RESISTOR RATIO
	WriteCommand(0xa2);	//BR=1/9
	WriteCommand(0xa0);	//set seg direction（列地址从左到右递增）
	WriteCommand(0xc8);	//set com direction (设置屏幕显示自上而下（0xc8），还是至下而上（0xc0）)
	WriteCommand(0x2f);	//set power control
	WriteCommand(0x40);	//set scroll line
	WriteCommand(0x81);	//SET ELECTRONIC VOLUME
	WriteCommand(0x20);	//set pm: 通过改变这里的数值来改变电压
	//WriteCommand(0xa7);	//set inverse display	   a6 off, a7 on
	//WriteCommand(0xa4);	//set all pixel on
	WriteCommand(0xaf);	//set display enable
	COGClear();
#endif
}




//*****************************************************************************
//
// All the following functions (below) for the LCD driver are required by grlib
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
// TemplateDisplayFix
static void
Template_DriverPixelDraw(void *pvDisplayData, int16_t lX, int16_t lY,
                                   uint16_t ulValue)
{  
  /* This function already has checked that the pixel is within the extents of  
  the LCD screen and the color ulValue has already been translated to the LCD.
  This function typically looks like:
  
  // Interpret pixel data (if needed)

  // Update buffer (if applicable)
  // Template_Memory[lY * LCD_Y_SIZE + (lX * BPP / 8)] = , |= , &= ...
  // Template memory must be modified at the bit level for 1/2/4BPP displays

  // SetAddress(MAPPED_X(lX, lY), MAPPED_Y(lX, lY));
  // WriteData(ulValue);
  */
#ifdef USER_CODE
		uint8_t ulPageAddress, ulPixelHeight;
		uint16_t ulBufferLocation;

		ulPageAddress=lY/8;
		ulPixelHeight = 0x01 << (lY%8);
		ulBufferLocation=ulPageAddress*128+lX;


		if(ulValue!=0)
		{
		  Template_Memory[ulBufferLocation] |= ulPixelHeight;//  1 黑
		}
		// Black Pixel
		else
		{
		  Template_Memory[ulBufferLocation] &= ~ulPixelHeight;// 0白
		}

		if (ui8Flush_Flag==FLUSH_OFF)
		{
			SetAddress(lX,ulPageAddress);
			WriteData(Template_Memory[ulBufferLocation]);
		}
#endif
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Template_DriverPixelDrawMultiple(void *pvDisplayData, int16_t lX,
                                           int16_t lY, int16_t lX0, int16_t lCount,
                                           int16_t lBPP,
                                           const uint8_t *pucData,
                                           const uint16_t *pucPalette)
{
	uint16_t ulByte;

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        // The pixel data is in 1 bit per pixel format
        case 1:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                ulByte = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
					Template_DriverPixelDraw(pvDisplayData, lX++, lY, 
											((uint16_t *)pucPalette)[(ulByte >> (7 - lX0)) & 1]);
                }
                
                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn
            
            break;
        }

		// The pixel data is in 2 bit per pixel format
        case 2:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                ulByte = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 4) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
					Template_DriverPixelDraw(pvDisplayData, lX++, lY, 
											((uint16_t *)pucPalette)[(ulByte >> (6 - (lX0 << 1))) & 3]);
                }
                
                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn
            
            break;
		}
        // The pixel data is in 4 bit per pixel format
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch(lX0 & 1)
            {
                case 0:
                  
                    while(lCount)
                    {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the palette
                        ulByte = (*pucData >> 4);    
                        ulByte = (*(uint16_t *)(pucPalette + ulByte));
                        // Write to LCD screen
                        Template_DriverPixelDraw(pvDisplayData, lX++, lY, ulByte);
                        
                        // Decrement the count of pixels to draw
                        lCount--;
                        
                        // See if there is another pixel to draw
                        if(lCount)
                        {
                case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette
                            ulByte = (*pucData++ & 15);
                            ulByte = (*(uint16_t *)(pucPalette + ulByte));
                            // Write to LCD screen
                            Template_DriverPixelDraw(pvDisplayData, lX++, lY, ulByte);

                            // Decrement the count of pixels to draw
                            lCount--;
                        }
                    }
            }
            // The image data has been drawn.
           
            break;
        }

        // The pixel data is in 8 bit per pixel format
        case 8:
        {
            // Loop while there are more pixels to draw
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                ulByte = *pucData++;
                ulByte = (*(uint16_t *)(pucPalette + ulByte));
                // Write to LCD screen
                Template_DriverPixelDraw(pvDisplayData, lX++, lY, ulByte);
            }
            // The image data has been drawn
            break;
        }
    }
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Template_DriverLineDrawH(void *pvDisplayData, int16_t lX1, int16_t lX2,
                                   int16_t lY, uint16_t ulValue)
{
  /* Ideally this function shouldn't call pixel draw. It should have it's own
  definition using the built in auto-incrementing of the LCD controller and its 
  own calls to SetAddress() and WriteData(). Better yet, SetAddress() and WriteData()
  can be made into macros as well to eliminate function call overhead. */
  
  do
  {
    Template_DriverPixelDraw(pvDisplayData, lX1, lY, ulValue);
  }
  while(lX1++ < lX2);
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Template_DriverLineDrawV(void *pvDisplayData, int16_t lX, int16_t lY1,
                                   int16_t lY2, uint16_t ulValue)
{
  do
  {
    Template_DriverPixelDraw(pvDisplayData, lX, lY1, ulValue);
  }
  while(lY1++ < lY2);
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void
Template_DriverRectFill(void *pvDisplayData, const tRectangle *pRect,
                                  uint16_t ulValue)
{
  int16_t x0 = pRect->sXMin;
  int16_t x1 = pRect->sXMax;
  int16_t y0 = pRect->sYMin;
  int16_t y1 = pRect->sYMax;
  
  while(y0++ <= y1)
  {
    Template_DriverLineDrawH(pvDisplayData, x0, x1, y0, ulValue);
  }
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint16_t
Template_DriverColorTranslate(void *pvDisplayData,
                                        uint32_t ulValue)
{
	/* The DPYCOLORTRANSLATE macro should be defined in TemplateDriver.h */
	
    //
    // Translate from a 24-bit RGB color to a color accepted by the LCD.
    //
    return(DPYCOLORTRANSLATE(ulValue));
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Template_DriverFlush(void *pvDisplayData)
{
  // Flush Buffer here. This function is not needed if a buffer is not used,
  // or if the buffer is always updated with the screen writes.
//	int16_t i=0,j=0;
//	for(i =0; i< LCD_Y_SIZE; i++)
//	for(j =0; j< (LCD_X_SIZE * BPP + 7) / 8; j++)
//		Template_DriverPixelDraw(pvDisplayData, j, i, Template_Memory[i * LCD_Y_SIZE + j]);
#ifdef USER_CODE
	if(ui8Flush_Flag==FLUSH_ON)
	{
			int i=0;
			for(i =0; i< LCD_Y_SIZE/8; i++)
			{
				WriteCommand(0xb0+i);						  //地址归零
				WriteCommand(0x10);
				WriteCommand(0x00);
				Write_Ndata(&Template_Memory[i*128],LCD_X_SIZE);//连续写128个数
			}
	}
	else
	{

	}
#endif
}

//*****************************************************************************
//
//! Send command to clear screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This function does a clear screen and the Display Buffer contents
//! are initialized to the current background color.
//!
//! \return None.
//
//*****************************************************************************
static void
Template_DriverClearScreen (void *pvDisplayData, uint16_t ulValue)
{
	// This fills the entire display to clear it
	// Some LCD drivers support a simple command to clear the display
	int16_t y0 = 0;
	while(y0 <= (LCD_Y_SIZE - 1))
	{
		Template_DriverLineDrawH(pvDisplayData, 0, LCD_X_SIZE - 1, y0, ulValue);
		y0++;
	}
}

//*****************************************************************************
//
//! The display structure that describes the driver for the blank template.
//
//*****************************************************************************
const tDisplay g_sTemplate_Driver =
{
    sizeof(tDisplay),
    Template_Memory,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    LCD_Y_SIZE,
    LCD_X_SIZE,
#else
    LCD_X_SIZE,
    LCD_Y_SIZE,
#endif
    Template_DriverPixelDraw,
    Template_DriverPixelDrawMultiple,
    Template_DriverLineDrawH,
    Template_DriverLineDrawV,
    Template_DriverRectFill,
    Template_DriverColorTranslate,
    Template_DriverFlush,
    Template_DriverClearScreen
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
