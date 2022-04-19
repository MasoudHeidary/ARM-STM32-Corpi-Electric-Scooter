/*
 * WS2812.c
 *
 *  Created on: Apr 9, 2022
 *      Author: Masoud Heidary
 *      gitHub: https://github.com/MasoudHeidary/
 *     License: MIT
 */
#include "WS2812.h"


/**
 * @breif initialize WS2812 Driver
 *
 * @param None
 *
 * @retval None
 */
void WS2812_init(void) {
	__WS2812_DMAIsReady = true;
}


/**
 * @breif use in DMA callback
 *
 * @param None
 *
 * @retval None
 */
void WS2812_DMACallBack(void) {
	__WS2812_DMAIsReady = true;
}


/**
 * @breif generate PWM pulses from color buffer
 *
 * @param None
 *
 * @retval None
 */
void __WS2812_generateBuffer(void) {
	// buffer position
	long pos = 0;

	for(int i=0; i< __WS2812_LEDsCount; i++) {
		WS2812_colorStruct color = __WS2812_ColorsBuf[i];

		// green bit:0-7
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x80) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x40) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x20) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x10) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x08) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x04) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x02) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.green & 0x01) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;

		// red bit:8-15
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x80) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x40) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x20) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x10) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x08) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x04) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x02) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.red & 0x01) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;


		// blue 16-23
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x80) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x40) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x20) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x10) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x08) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x04) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x02) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
		__WS2812_TimerBuf[pos++] =
				((color.blue & 0x01) != 0) ? __WS2812_OneTime : __WS2812_ZeroTime;
	}

	// delay
	for(int i=0; i<__WS2812_DelayBufLen; i++)
		__WS2812_TimerBuf[pos++] = 0;
}


/**
 * @breif generate PWM signal and set colors in LEDs
 *
 * @param timer TIM_HandleTypeDef timer to generate PWM
 * @param channel uint32_t timer channel to generate PWM
 *
 * @retval None
 */
void WS2812_refresh(TIM_HandleTypeDef timer, uint32_t channel) {
	__WS2812_generateBuffer();

	// wait until DMA being ready
	while (!__WS2812_DMAIsReady);

	__WS2812_DMAIsReady = false;
	HAL_TIM_PWM_Start_DMA(&timer, channel, (uint32_t *) __WS2812_TimerBuf, __WS2812_TimerBufLen);
}


/**
 * @breif set color to all LEDs
 *
 * @param color WS2812_colorStruct
 *
 * @retval None
 */
void WS2812_setAll(WS2812_colorStruct color) {
	for(int i=0; i < __WS2812_LEDsCount; i++) {
		__WS2812_ColorsBuf[i] = color;
	}
}


/**
 * @breif turn off all the LEDs
 *
 * @param None
 *
 * @retval None
 */
void WS2812_clearAll(void) {
	for(int i=0; i < __WS2812_LEDsCount; i++) {
		__WS2812_ColorsBuf[i] = (WS2812_colorStruct){0, 0, 0};
	}
}


/**
 * @breif set color the led_number'th LED
 *
 * @param color WS2812_colorStruct to set
 * @param led_number - start from 0
 *
 * @retval None
 */
void WS2812_setOne(WS2812_colorStruct color, int led_number) {
	if(led_number < __WS2812_LEDsCount)
		__WS2812_ColorsBuf[led_number] = color;
}


/**
 * @breif shift colors
 * D0, D1, D2 -> color, D0, D1
 *
 * @param color WS2812_colorStruct insert in shift
 *
 * @retval None
 */
void WS2812_shiftNext(WS2812_colorStruct color) {
	for (int i = __WS2812_LEDsCount - 1; i > 0; i--)
		__WS2812_ColorsBuf[i] = __WS2812_ColorsBuf[i - 1];

	WS2812_setOne(color, 0);
}


/**
 * @breif shift colors
 * D0, D1, D2 -> D1, D2, color
 *
 * @param color WS2812_colorStruct insert in shift
 *
 * @retval None
 */
void WS2812_shiftPrevious(WS2812_colorStruct color) {
	for (int i = 0; i < __WS2812_LEDsCount - 1; i++)
		__WS2812_ColorsBuf[i] = __WS2812_ColorsBuf[i+1];

	WS2812_setOne(color, __WS2812_LEDsCount - 1);

}


/**
 * @breif rotate colors
 * 0, 1, 2 -> 2, 0, 1
 *
 * @param None
 *
 * @retval None
 */
void WS2812_rotateNext(void) {
	WS2812_colorStruct color = __WS2812_ColorsBuf[__WS2812_LEDsCount - 1];
	WS2812_shiftNext(color);
}


/**
 * @breif rotate colors
 * 0, 1, 2 -> 1, 2, 0
 *
 * @param None
 *
 * @retval None
 */
void WS2812_rotatePrevious(void) {
	WS2812_colorStruct color = __WS2812_ColorsBuf[0];
	WS2812_shiftPrevious(color);
}




