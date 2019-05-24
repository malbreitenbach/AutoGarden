/*
 * dht11_Driver.h
 *
 *  Created on: 16.10.2018
 *      Author: Malte
 */

#ifndef DHT11_DRIVER_H_
#define DHT11_DRIVER_H_

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"
#include "stm32f103xb.h"

#define TIMEOUT 100000
#define TOLLERANCE 30 //us

TIM_TypeDef *timer;
GPIO_TypeDef *port;

uint16_t pin;

typedef enum{
	RESPONSE_CORRECT = 0U, RESPONSE_NO_ACTION, RESPONSE_WRONG_ANSWER
}RESPONSE;

typedef enum{
	DHT11_RESULT_CORRECT = 0U, DHT11_RESULT_ERROR, DHT11_RESULT_TIMEOUT
}DHT11_RESULT;

void dht11_ini(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, TIM_TypeDef *tim);
DHT11_RESULT dht11_read(uint8_t *temp, uint8_t *humidity);
DHT11_RESULT dht11_measure(uint8_t *temp, uint8_t *humidity);





#endif /* DHT11_DRIVER_H_ */
