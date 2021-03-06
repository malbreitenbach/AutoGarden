/*
 * sensory.h
 *
 *  Created on: 23.05.2019
 *      Author: Malte
 */

#ifndef SENSORY_H_
#define SENSORY_H_

#include "stdbool.h"
#include "stm32f1xx_hal.h"

/* --- Variables ---*/
volatile bool measAvailable;
volatile uint16_t dmaBuffer[50];


void sen_temperatureIni();
void sen_lightIni();

uint8_t sen_getLight();
uint8_t sen_getTemperature();

void sen_startLightMeasure();
void sen_startTemperatureMeasure();

bool sen_temperatureMeasureReady();
bool sen_lightMeasureReady();

uint8_t sen_getTemperatureMeasure();
uint8_t sen_getLightMeasure();

#endif /* SENSORY_H_ */
