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

void CORPI_init(void) {
	WS2812_init();

	WS2812_setAll((WS2812_colorStruct ) { 3, 0, 0 });
	WS2812_refresh(htim17, TIM_CHANNEL_1);
}
