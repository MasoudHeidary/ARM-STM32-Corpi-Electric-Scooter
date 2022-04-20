/*
 * APP.h
 *
 *  Created on: Apr 20, 2022
 *      Author: Masoud Heidary
 *      gitHub: https://github.com/MasoudHeidary/
 *     License: MIT
 */

#ifndef INC_APP_H_
#define INC_APP_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "WS2812.h"
#include "main.h"

/* Includes ------------------------------------------------------------------*/
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

TIM_HandleTypeDef htim17;
DMA_HandleTypeDef hdma_tim17_ch1_up;
#define WS2812_Channel TIM_CHANNEL_1

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* Includes ------------------------------------------------------------------*/
// Lock
#define APP_LockStateLock 0
#define APP_LockStateFree 1
#define APP_LockStateFind 2

// Front Led
#define APP_FrontLedOff 0
#define APP_FrontLedOn	1
#define APP_FrontLedToggle 2
#define APP_FrontLedToggleDelay 150

// Colors
#define APP_ColorOff (WS2812_colorStruct) {0, 0, 0}
#define APP_ColorLock (WS2812_colorStruct) {10, 0, 0}
#define APP_ColorFree (WS2812_colorStruct) {0, 10, 0}
#define APP_ColorFind (WS2812_colorStruct) {0, 0, 10}

// Brake
#define APP_BrakeFreeVoltage 50
#define APP_BrakeFree 0
#define APP_BrakeTaken 1


// RX
#define APP_RXBufferLen 2
#define APP_RXHeader '@'

// TX
#define APP_TXBufferLen 3
#define APP_TXHeader '@'
#define APP_TXDelay 5000

// ADC
#define APP_ADCBufferLen 2


/* Includes ------------------------------------------------------------------*/

struct {
	uint32_t ADCBuffer[APP_ADCBufferLen];
	uint8_t Throttle;
	bool Brake;

	uint8_t RXBuffer[APP_RXBufferLen];
	uint8_t TXBuffer[APP_TXBufferLen];

	uint8_t Lock;
	uint8_t FrontLed;

} APP;
/* Includes ------------------------------------------------------------------*/
void APP_init(void);
void APP_while(void);

void __APP_RX(void);
void __APP_TX(void);
/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

#endif /* INC_APP_H_ */
