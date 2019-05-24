/*
 * sensory.c
 *
 *  Created on: 23.05.2019
 *      Author: Malte
 */

#include "sensory.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

void sen_temperatureIni()
{

	/* ADC1 init function */
	  ADC_ChannelConfTypeDef sConfig;

	    /**Common config
	    */
	  hadc1.Instance = ADC1;
	  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1.Init.ContinuousConvMode = ENABLE;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.NbrOfConversion = 2;
	  if (HAL_ADC_Init(&hadc1) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Configure Regular Channel
	    */
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Configure Regular Channel
	    */
	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = ADC_REGULAR_RANK_2;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  GPIO_InitTypeDef GPIO_InitStruct;
	  /* USER CODE BEGIN ADC1_MspInit 0 */

	  /* USER CODE END ADC1_MspInit 0 */
	    /* ADC1 clock enable */
	    __HAL_RCC_ADC1_CLK_ENABLE();

	    /**ADC1 GPIO Configuration
	    PA1     ------> ADC1_IN1
	    PA4     ------> ADC1_IN4
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    /* ADC1 DMA Init */
	    /* ADC1 Init */
	    hdma_adc1.Instance = DMA1_Channel1;
	    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	    hdma_adc1.Init.Mode = DMA_NORMAL;
	    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
	    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
	    {
	      _Error_Handler(__FILE__, __LINE__);
	    }

	    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

	    //DMA interrupt Init
	    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	    /* ADC1 interrupt Init */
	    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	  /* USER CODE BEGIN ADC1_MspInit 1 */

	  /* USER CODE END ADC1_MspInit 1 */


}

void sen_lightIni()
{
	sen_temperatureIni();
}

uint8_t sen_getLight();
uint8_t sen_getTemperature();

void sen_startLightMeasure()
{
		measAvailable = false;
		/**Configure Regular Channel **/
		ADC_ChannelConfTypeDef sConfig;
		sConfig.Channel = ADC_CHANNEL_1;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
		_Error_Handler(__FILE__, __LINE__);
		}

		/**Configure Regular Channel
		*/
		sConfig.Channel = ADC_CHANNEL_4;
		sConfig.Rank = ADC_REGULAR_RANK_2;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
		_Error_Handler(__FILE__, __LINE__);
		}

	HAL_ADC_Start_DMA(&hadc1, dmaBuffer, 50);
}

void sen_startTemperatureMeasure()
{
	measAvailable = false;
	//HAL_ADC_Start_DMA(&hadc1, dmaBuffer, 50);
}

bool sen_temperatureMeasureReady()
{
	return measAvailable;
}
bool sen_lightMeasureReady()
{
	return measAvailable;
}

uint8_t sen_getTemperatureMeasure()
{
	measAvailable = false;
	uint32_t sum = 0;

	for(uint8_t index = 0; index < 50 ;index+=2)
	{
		sum += dmaBuffer[index];
	}
	sum /= 2500;
	return sum;
}
uint8_t sen_getLightMeasure()
{
	measAvailable = false;
	uint32_t sum = 0;

	for(uint8_t index = 1; index < 50 ;index+=2)
	{
		sum += dmaBuffer[index];
	}
	sum /= 2500;
	return sum;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	measAvailable = true;
}
