#include "stm32f0xx_hal.h"
#include "WS2812.h"

uint16_t WS2812_TIM_BUFFUR[WS2812_BUFFURLEN];
uint8_t DMAReady = 1;

void DMA_Callback(void) {
	DMAReady = 1;
}

/*
 * change '0','1' to signal ( pulse 16bit )
 * check buffer of led color "WS2812_LED_BUFFER" bit to bit
 * and make a signal(16bit) for every 1bit
 *
 * to the end we have a 50us delay
 */
void calcBuf(void) {
	uint32_t n;
	uint32_t pos;
	WS2812ColorStruct led;

	pos = 0;
	// set timings for all LEDs
	for (n = 0; n < WS2812_NUM_LEDS; n++) {
		led = WS2812_LED_BUFFUR[n];

		//greens bit:0-7
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x80) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x40) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x20) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x10) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x08) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x04) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x02) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.green & 0x01) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;

		// red bit:0-7
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x80) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x40) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x20) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x10) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x08) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x04) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x02) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.red & 0x01) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;

		// blue bit:0-7
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x80) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x40) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x20) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x10) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x08) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x04) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x02) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
		WS2812_TIM_BUFFUR[pos++] =
				((led.blue & 0x01) != 0) ? WS2812_HIGH_TIME : WS2812_LOW_TIME;
	}

	//set (2*24)(bit->word(16bit)) for low signal and delay
	for (n = 0; n < 48; n++) {
		WS2812_TIM_BUFFUR[pos++] = 0;
	}
}

//start DMA and set colors to leds
void startDMA(void) {
	DMAReady = 0;
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *) WS2812_TIM_BUFFUR,
	WS2812_BUFFURLEN);
}

//refresh signal buffer with led buffer
//and send signals to leds
void WS2812Refresh(void) {
//	while (!DMAReady)
//		;
	calcBuf();
	startDMA();
}

//turn off all leds
void WS2812Clear() {
	for (uint32_t i = 0; i < WS2812_NUM_LEDS; i++)
		WS2812_LED_BUFFUR[i] = (WS2812ColorStruct ) { 0, 0, 0 };
	WS2812Refresh();
}

//turn all leds to one color
void WS2812SetAll(WS2812ColorStruct Color) {
	for (int i = 0; i < WS2812_NUM_LEDS; i++)
		WS2812_LED_BUFFUR[i] = Color;
}

//set one led to a color : number start 0
void WS2812SetOne(uint16_t number, WS2812ColorStruct Color) {
	if (number < WS2812_NUM_LEDS)
		WS2812_LED_BUFFUR[number] = Color;
}

//shift led color one to right
void WS2812ShiftRight() {
	for (int i = WS2812_NUM_LEDS - 1; i > 0; i--)
		WS2812_LED_BUFFUR[i] = WS2812_LED_BUFFUR[i - 1];
}

//shifted led colors one ro left
void WS2812ShiftLeft() {
	for (int i = 0; i < WS2812_NUM_LEDS - 1; i++)
		WS2812_LED_BUFFUR[i] = WS2812_LED_BUFFUR[i + 1];
}

//shift the colors to right and add color
void WS2812AddRight(WS2812ColorStruct Color) {
	WS2812ShiftRight();
	WS2812_LED_BUFFUR[0] = Color;
}

//shift the colors to left and add color
void WS2812AddLeft(WS2812ColorStruct Color) {
	WS2812ShiftLeft();
	WS2812_LED_BUFFUR[WS2812_NUM_LEDS - 1] = Color;
}
