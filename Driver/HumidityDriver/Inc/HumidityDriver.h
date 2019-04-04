/*
 * HumidityDriver.h
 *
 *  Created on: 08.11.2018
 *      Author: Malte
 */

#ifndef HUMIDITYDRIVER_H_
#define HUMIDITYDRIVER_H_

#include "stdint.h"

/* --- DEFINES --- */

/* --- VARIABLES --- */

/* --- FUNCTIONS --- */

/**
 * PA0 -> Ain
 */
void humidity_Ini(void);
uint8_t humidity_estaminateHumidity(uint32_t numMes);
uint16_t humidity_readHumidity(void);

#endif /* HUMIDITYDRIVER_H_ */
