/*
 * APP.h
 *
 *  Created on: Mar 10, 2020
 *      Author: MH
 */
#pragma once

#ifndef INC_APP_H_
#define INC_APP_H_

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
#include "WS2812.h"
#include "stm32f0xx_hal.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
//Constant
//Public
#define True 1
#define False 0
#define High 1
#define Low 0

//Colors
#define ColorLock (WS2812ColorStruct) { 255,0,0 }
#define Colorfree (WS2812ColorStruct) { 0,255,0 }
#define ColorRese (WS2812ColorStruct) { 0,0,255 }
#define ColorOff  (WS2812ColorStruct) { 0,0,0 }

//Key State
#define KeyDelay 750
#define KeyLedLight 250
#define KeyLockCounter 5
#define KeyReserveCounter 3
#define KeyFireCounter 2
#define KeyOutCounter 6
#define LowGas 50
#define KeyFree 0
#define KeyLock 1
#define KeyRese 2
#define KeyFire 3	//for blink Front Led

//TX
#define TXHeader '$'
#define TXBufferLen 3
#define TXDelay 5000
#define BitOfBrake 6
#define BitOfBrakeLen 2
#define BitOfKeyState 4
#define BitOfKeyStateLen 2

//Brake
#define BrakeFreeVoltage 50
#define BrakeTakeVal 1
#define BrakeFreeVal 0

//RX
#define ReceiveHeader '$'
#define ReceiveBufferLength 2

//Lock
#define LockStateFree 0
#define LockStateLock 1
#define LockStateRese 2

//front LED
#define FrontLedOn 1
#define FrontLedOff 0
#define FrontLedFire 2
#define FrontLedFireDelay 250

/////////////////////////////////////////////////////////////////////////////////////
//VAR
struct CORPI {
	uint32_t ADCValue[2]; //GAS-BRAKE
	uint8_t KeyState;
	uint8_t Lock;
	uint8_t FrontLed;
} Corpi;

/////////////////////////////////////////////////////////////////////////////////////
//Functions
//Public
void Starter(void);
void SendData(void);
void ReceiveData(void);
void ReceiveRefresh(void);
void RealTime(void);
void FrontLedFireFunction(void);

//Private
uint8_t BitRead(uint8_t X, uint8_t number);
void BitWrite(uint8_t *X, uint8_t number, uint8_t Value);
void BitNumber(uint8_t *X, uint8_t number, uint8_t Value, uint8_t ValueLength);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_APP_H_ */
