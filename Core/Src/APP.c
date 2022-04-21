/*
 * APP.c
 *
 *  Created on: Apr 20, 2022
 *      Author: Masoud Heidary
 *      gitHub: https://github.com/MasoudHeidary/
 *     License: MIT
 */

#include "APP.h"

/**
 * @breif Timer Interrupt, use for WS2812 Driver
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM17) {
		HAL_TIM_PWM_Stop_DMA(&htim17, TIM_CHANNEL_1);
		WS2812_DMACallBack();
	}
}


/**
 * @breif ADC(Analog Digital Converter) Interrupt, use for reading Throttle and Brake values
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		APP.Throttle = (uint8_t) (APP.ADCBuffer[0]);

		APP.Brake = APP.ADCBuffer[1] > APP_BrakeFreeVoltage ?
				APP_BrakeTaken : APP_BrakeFree;
	}
}


/**
 * @breif UART RX Interrupt, use for check RX header {if header is wrong -> reset RX reading}
 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		if (APP.RXBuffer[0] != APP_RXHeader) {
			HAL_UART_Receive_DMA(&huart1, APP.RXBuffer, APP_RXBufferLen);
		}
	}
}


/**
 * @breif UART RX Interrupt, user for reading incoming commands
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		__APP_RX();
		HAL_UART_Receive_DMA(&huart1, APP.RXBuffer, APP_RXBufferLen);
	}
}


/**
 * @breif initialize Application, set values to zero, start DMA and Drivers
 *
 * @param None
 *
 * @retval None
 */
void APP_init(void) {
	APP.Throttle = 0;
	APP.Brake = APP_BrakeTaken;
	APP.Lock = APP_LockStateLock;
	APP.FrontLed = APP_FrontLedOff;

	HAL_GPIO_WritePin(FrontLed_GPIO_Port, FrontLed_Pin, 0);

	WS2812_init();
	WS2812_setAll(APP_ColorOff);
	WS2812_refresh(htim17, TIM_CHANNEL_1);

	HAL_UART_Receive_DMA(&huart1, APP.RXBuffer, APP_RXBufferLen);

	HAL_ADC_Start_DMA(&hadc, APP.ADCBuffer, APP_ADCBufferLen);
}


/**
 * @breif RX Procedure
 *
 * @param None
 *
 * @retval None
 */
void __APP_RX(void) {
	APP.Lock = APP.RXBuffer[1] & 0x03;
	APP.FrontLed = (APP.RXBuffer[1] >> 2) & 0x03;

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

		default:
			WS2812_setAll(APP_ColorOff);
			WS2812_refresh(htim17, WS2812_Channel);
			break;
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


/**
 * @breif TX Procedure
 *
 * @param None
 *
 * @retval None
 */
void __APP_TX(void) {
	if (APP.Lock != APP_LockStateFree)
		return;

	APP.TXBuffer[0] = APP_TXHeader;
	APP.TXBuffer[1] = APP.Brake ? 0 : APP.Throttle;
	APP.TXBuffer[2] = (uint8_t)APP.Brake;

	HAL_UART_Transmit_DMA(&huart1, APP.TXBuffer, APP_TXBufferLen);
}

void APP_while(void) {
	static uint32_t front_led_timer = 0;
	static uint32_t TX_timer = 0;

	if (APP.FrontLed == APP_FrontLedToggle)
		if (HAL_GetTick() - front_led_timer >= APP_FrontLedToggleDelay) {
				HAL_GPIO_TogglePin(FrontLed_GPIO_Port, FrontLed_Pin);
				front_led_timer = HAL_GetTick();
		}

	if (HAL_GetTick() - TX_timer >= APP_TXDelay) {
		__APP_TX();
		TX_timer = HAL_GetTick();
	}

}
