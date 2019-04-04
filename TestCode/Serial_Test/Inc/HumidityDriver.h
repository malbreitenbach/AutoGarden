/*
 * HumidityDriver.h
 *
 *  Created on: 08.11.2018
 *      Author: Malte
 */

#ifndef HUMIDITYDRIVER_H_
#define HUMIDITYDRIVER_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"

/* --- DEFINES --- */

/* --- VARIABLES --- */
GPIO_TypeDef * port_hum;
uint16_t pin_hum;
ADC_TypeDef * adc_hum;
/* --- FUNCTIONS --- */

/**
 * PA0 -> Ain
 */
void humidity_Ini(GPIO_TypeDef * port, uint16_t pin, ADC_TypeDef * adc, uint32_t channel);
uint8_t humidity_estaminateHumidity(uint32_t numMes);
uint16_t humidity_readHumidity(void);

#endif /* HUMIDITYDRIVER_H_ */
