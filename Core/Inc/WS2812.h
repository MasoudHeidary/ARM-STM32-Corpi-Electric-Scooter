/*
 * WS2812.h
 *
 *  Created on: Apr 9, 2022
 *      Author: Masoud Heidary
 *      gitHub: https://github.com/MasoudHeidary/
 *     License: MIT
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>

#include "main.h"

/* defines -------------------------------------------------------------------*/
#define __WS2812_LEDsCount	3
#define __WS2812_DelayBufLen	50

// (LEDs number + 2) * (color size _ 24bit _ 3byte)
#define __WS2812_TimerBufLen	__WS2812_LEDsCount * 24 + __WS2812_DelayBufLen

#define __WS2812_ZeroTime 	16	// 0.4us * 40MHz
#define __WS2812_OneTime	32	// 0.8us * 40MHz

/* variables -----------------------------------------------------------------*/
bool __WS2812_DMAIsReady;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} WS2812_colorStruct;

// for setting colors for each LED
WS2812_colorStruct __WS2812_ColorsBuf [__WS2812_LEDsCount];

// for timer to generate PWM
uint16_t __WS2812_TimerBuf [__WS2812_TimerBufLen];

/* public functions ----------------------------------------------------------*/
// general
void WS2812_init(void);
void WS2812_DMACallBack(void);

// update led colors
void WS2812_refresh(TIM_HandleTypeDef, uint32_t);

// set color
void WS2812_setAll(WS2812_colorStruct);
void WS2812_setOne(WS2812_colorStruct, int);
void WS2812_clearAll(void);
void WS2812_shiftNext(WS2812_colorStruct);
void WS2812_shiftPrevious(WS2812_colorStruct color);
void WS2812_rotateNext(void);
void WS2812_rotatePrevious(void);

/* private functions ----------------------------------------------------------*/
void __WS2812_generateBuffer(void);

/* ----------------------------------------------------------------------------*/

#endif /* INC_WS2812_H_ */
