/*
 * pump_Driver.h
 *
 *  Created on: 17.10.2018
 *      Author: Malte
 */

#ifndef PUMP_DRIVER_H_
#define PUMP_DRIVER_H_

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "stm32f103xb.h"


typedef enum
{
	PUMP_RESULT_OK = 0U, PUMP_RESULT_ERROR, PUMP_RESULT_OUT_OF_INPUT_RANGE
}PUMP_RESULT;

typedef enum
{
	PUMP_STATE_NO_INI = 0U, PUMP_STATE_STOP, PUMP_STATE_RUNNING
}PUMP_STATE;

PUMP_STATE pumpState;

PUMP_RESULT pumpStart();
PUMP_RESULT pumpStop();
void pumpIni(TIM_TypeDef * tim, uint32_t cha);
PUMP_RESULT pumpSet(uint8_t per);



#endif /* PUMP_DRIVER_H_ */
