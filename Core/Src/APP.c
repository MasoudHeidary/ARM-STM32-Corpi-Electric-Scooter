/*
 * APP.c
 *
 *  Created on: Mar 10, 2020
 *      Author: MasoudHeidaryDeveloper@gmail.com
 */

#include "APP.h"

uint8_t ReceiveBuffer[ReceiveBufferLength];
uint8_t TXBuffer[TXBufferLen] = { 0 };

void Starter(void) {

	//turn on led on board
	HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, High);

	//turn on ADC reading
	HAL_ADC_Start_DMA(&hadc, Corpi.ADCValue, ReceiveBufferLength);

	//turn on RX DMA
	HAL_UART_Receive_DMA(&huart1, ReceiveBuffer, ReceiveBufferLength);

	//set WS2812 Color
	WS2812SetAll( ColorLock);
	WS2812Refresh();

	//set default
	Corpi.ADCValue[0] = 0;
	Corpi.ADCValue[1] = 0;
	Corpi.Lock = LockStateLock;
	Corpi.KeyState = KeyFree;
	Corpi.FrontLed = FrontLedOff;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == KEY_Pin) {
		if ((uint8_t) Corpi.ADCValue[0] > LowGas)
			return;

		HAL_GPIO_WritePin( FRONT_LIGHT_GPIO_Port, FRONT_LIGHT_Pin, Low);

		uint8_t counter = 0;
		uint32_t Time = HAL_GetTick();

		while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == High) {
			if (HAL_GetTick() - Time >= KeyDelay) {

				HAL_GPIO_WritePin(KEY_LED_GPIO_Port, KEY_LED_Pin, High);
				HAL_Delay(KeyLedLight);
				HAL_GPIO_WritePin(KEY_LED_GPIO_Port, KEY_LED_Pin, Low);

				Time = HAL_GetTick();
				counter++;
				if (counter == KeyOutCounter)
					break;
			}
		}
		switch (counter) {
		case KeyLockCounter:
			Corpi.KeyState = KeyLock;
			break;
		case KeyReserveCounter:
			Corpi.KeyState = KeyRese;
			break;
		case KeyFireCounter:
			Corpi.KeyState = KeyFire;
			break;
//		default:
//			Corpi.KeyState = KeyFree;
		}

	}
}

//a Bit Read in uint8_t
uint8_t BitRead(uint8_t X, uint8_t number) {
	return (X >> number) & (0X01);
}

//a Bit Write in uint8_t
void BitWrite(uint8_t *X, uint8_t number, uint8_t Value) {
	if (BitRead(*X, number) == Value)
		return;

	if (Value == 1)
		*X += 1 << number;
	else if (Value == 0)
		*X -= 1 << number;
}

//Write any number in a part of a number
void BitNumber(uint8_t *X, uint8_t number, uint8_t Value, uint8_t ValueLength) {
	for (uint8_t i = 0; i < ValueLength; i++) {
		BitWrite(X, number + i, BitRead(Value, i));
	}
}

//send Data by DMA
void SendData(void) {
	TXBuffer[0] = TXHeader;
	TXBuffer[1] = (uint8_t) Corpi.ADCValue[0];

	uint8_t Brake = (uint8_t) Corpi.ADCValue[1];
	// bit:6,7 for Brake Value
	if (Brake <= BrakeFreeVoltage)
		BitNumber(&(TXBuffer[2]), BitOfBrake, BrakeFreeVal, BitOfBrakeLen);
	else
		BitNumber(&(TXBuffer[2]), BitOfBrake, BrakeTakeVal, BitOfBrakeLen);

	uint8_t KeyState = Corpi.KeyState;
	Corpi.KeyState = KeyFree;
	//bit:4,5 for Key State
	BitNumber(&(TXBuffer[2]), BitOfKeyState, KeyState, BitOfKeyStateLen);

	HAL_UART_Transmit_DMA(&huart1, TXBuffer, 3);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		ReceiveData();
		ReceiveRefresh();
		HAL_UART_Receive_DMA(&huart1, ReceiveBuffer, ReceiveBufferLength);
	}
}

uint8_t M = 0b11111111;
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		if (ReceiveBuffer[0] != ReceiveHeader) {
			HAL_UART_Receive_DMA(&huart1, &(ReceiveBuffer[0]),
			ReceiveBufferLength);
		}
	}
}

void ReceiveData(void) {
	if (ReceiveBuffer[0] != ReceiveHeader)
		return;

	uint8_t buf = ReceiveBuffer[1];
	Corpi.Lock = (buf >> 6) & 0b00000011;
	Corpi.FrontLed = (buf >> 4) & 0b00000011;
}

void ReceiveRefresh(void) {
	switch (Corpi.Lock) {

	case LockStateFree:
		WS2812SetAll( Colorfree);
		WS2812Refresh();
		break;

	case LockStateLock:
		WS2812SetAll( ColorLock);
		WS2812Refresh();
		break;

	case LockStateRese:
		WS2812SetAll( ColorRese);
		WS2812Refresh();
		break;
	}

	switch (Corpi.FrontLed) {
	case FrontLedOn:
		HAL_GPIO_WritePin( FRONT_LIGHT_GPIO_Port, FRONT_LIGHT_Pin, High);
		break;

	case FrontLedOff:
		HAL_GPIO_WritePin(FRONT_LIGHT_GPIO_Port, FRONT_LIGHT_Pin, Low);
		break;

	}
}

//real time main function
void RealTime(void) {

	Starter();

	uint32_t FrontLedFireTime = HAL_GetTick();
	uint32_t TXTimer = HAL_GetTick();

	while ( True) {

		if (Corpi.FrontLed == FrontLedFire)
			if (HAL_GetTick() - FrontLedFireTime >= FrontLedFireDelay) {
				FrontLedFireTime = HAL_GetTick();
				HAL_GPIO_TogglePin(FRONT_LIGHT_GPIO_Port, FRONT_LIGHT_Pin);
			}

		if (Corpi.Lock != LockStateLock)
			if (HAL_GetTick() - TXTimer >= TXDelay) {
				SendData();
				TXTimer = HAL_GetTick();
			}
	}
}
