/*
 * APP.c
 *
 *  Created on: Apr 20, 2022
 *      Author: Masoud Heidary
 *      gitHub: https://github.com/MasoudHeidary/
 *     License: MIT
 */

#include "APP.h"

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM17) {
		HAL_TIM_PWM_Stop_DMA(&htim17, TIM_CHANNEL_1);
		WS2812_DMACallBack();
	}
}

/* RX Interrupts ------------------------------------------------------------------*/
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		if (APP.RXBuffer[0] != APP_RXHeader) {
			HAL_UART_Receive_DMA(&huart1, APP.RXBuffer, APP_RXBufferLen);
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		__APP_RX();
		HAL_UART_Receive_DMA(&huart1, APP.RXBuffer, APP_RXBufferLen);
	}
}

void APP_init(void) {
	HAL_GPIO_WritePin(FrontLed_GPIO_Port, FrontLed_Pin, 0);

	WS2812_init();

	WS2812_setAll(APP_ColorOff);
	WS2812_refresh(htim17, TIM_CHANNEL_1);

	HAL_UART_Receive_DMA(&huart1, APP.RXBuffer, APP_RXBufferLen);
}

void __APP_RX(void) {
	APP.Lock =
			(APP.RXBuffer[APP_RXProtocol_PositionLock]
			>> APP_RXProtocol_ShiftLock)
			& APP_RXProtocol_AndLock;

	APP.FrontLed =
			(APP.RXBuffer[APP_RXProtocol_PositionFrontLed]
			>> APP_RXProtocol_ShiftFrontLed)
			& APP_RXProtocol_AndFrontLed;


	switch (APP.Lock) {
		case APP_LockStateLock:
			WS2812_setAll(APP_ColorLock);
			WS2812_refresh(htim17, WS2812_Channel);
			break;

		case APP_LockStateFree:
			WS2812_setAll(APP_ColorFree);
			WS2812_refresh(htim17, WS2812_Channel);
			break;

		case APP_LockStateFind:
			WS2812_setAll(APP_ColorFind);
			WS2812_refresh(htim17, WS2812_Channel);
			break;

//		default:
//			WS2812_setAll(APP_ColorOff);
//			WS2812_refresh(htim17, WS2812_Channel);
//			break;
	}

	switch (APP.FrontLed) {
		case APP_FrontLedOff:
			HAL_GPIO_WritePin(FrontLed_GPIO_Port, FrontLed_Pin, 0);
			break;

		case APP_FrontLedOn:
			HAL_GPIO_WritePin(FrontLed_GPIO_Port, FrontLed_Pin, 1);
			break;

		case APP_FrontLedToggle:
			break;

		default:
			HAL_GPIO_WritePin(FrontLed_GPIO_Port, FrontLed_Pin, 0);
			break;
	}
}

void APP_while(void) {
	static uint32_t front_led_timer = 0;

	if (APP.FrontLed == APP_FrontLedToggle)
		if (HAL_GetTick() - front_led_timer >= APP_FrontLedToggleDelay) {
				HAL_GPIO_TogglePin(FrontLed_GPIO_Port, FrontLed_Pin);
				front_led_timer = HAL_GetTick();
		}

}