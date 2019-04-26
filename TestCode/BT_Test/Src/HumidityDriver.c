/*
 * HumidityDriver.c
 *
 *  Created on: 08.11.2018
 *      Author: Malte
 */

#include "HumidityDriver.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"

//Prototypes
void enableADC(void);

/**
 * GPIOA, GPIO_PIN_0, ADC1, ADC_CHANNEL_0
 * __HAL_RCC_GPIOA_CLK_ENABLE();
 * __HAL_RCC_ADC1_CLK_ENABLE();
 *
 */
void humidity_Ini(GPIO_TypeDef * port, uint16_t pin, ADC_TypeDef * adc, uint32_t channel)
{

	port_hum = port;
	pin_hum = pin;
	adc_hum = adc;

	GPIO_InitTypeDef analogPin;

	analogPin.Mode = GPIO_MODE_ANALOG;
	analogPin.Pin = pin_hum;
	analogPin.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(port_hum, &analogPin);

	ADC_ChannelConfTypeDef sConfig;
	ADC_HandleTypeDef hadc1;

	hadc1.Instance = adc_hum;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadc1);
	/**Configure Regular Channel
	*/
	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);


}

void enableADC()
{
	adc_hum->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
	adc_hum->CR2 |= ADC_CR2_ADON; //Start Continous Reading
}


uint8_t humidity_estaminateHumidity(uint32_t numMes)
{
	enableADC(); //WAKEUP
	uint32_t mes_all = 0;
	for(uint32_t cnt = 0; cnt < numMes; cnt++)
	{
		mes_all += humidity_readHumidity();
	}
	adc_hum->CR2 &= ~(ADC_CR2_CONT | ADC_CR2_ADON); //Disable

	return 100 - (((mes_all/numMes)*100)/0xFFF);
}

uint16_t humidity_readHumidity(void)
{
	//enableADC(); //WAKEUP
	//ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(adc_hum->SR & ADC_SR_EOC)); //Wait until reading has finished
	return (uint16_t)adc_hum->DR;
}




