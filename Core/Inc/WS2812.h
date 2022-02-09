/*
 * control of WS2812 and maybe WS2812 and other with little change
 * make for ro-box startup
 *
 * @author: Masoud Heidary
 * @address: MasoudHeidaryDeveloper@gmail.com
 */

#pragma once

#include "stm32f0xx_hal.h"

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;

#define WS2812_NUM_LEDS		1
#define WS2812_BUFFURLEN	(WS2812_NUM_LEDS+2)*24

#define  WS2812_TIM_PRESCALE    0  // F_T3  = 72 MHz (13.88ns)
#define  WS2812_TIM_PERIODE   	44  // F_PWM = 800 kHz (1.25us)

#define  WS2812_LOW_TIME        	14  // 29 * 13,9ns = 0.43us
#define  WS2812_HIGH_TIME        	29  // 58 * 13.9ns = 0.81us

//--------------------------------------------------------------
// RGB LED Farbdefinition (3 x 8bit)
//--------------------------------------------------------------
typedef struct {
	uint8_t red;    // 0...255 (als PWM-Wert)
	uint8_t green;  // 0...255 (als PWM-Wert)
	uint8_t blue;   // 0...255 (als PWM-Wert)
} WS2812ColorStruct;

WS2812ColorStruct WS2812_LED_BUFFUR[WS2812_NUM_LEDS];

void DMA_Callback(void);

void WS2812Refresh(void);
void WS2812Clear();
void WS2812SetAll(WS2812ColorStruct Color);
void WS2812SetOne(uint16_t number, WS2812ColorStruct Color);
void WS2812ShiftRight();
void WS2812ShiftLeft();
void WS2812AddRight(WS2812ColorStruct Color);
void WS2812AddLeft(WS2812ColorStruct Color);
