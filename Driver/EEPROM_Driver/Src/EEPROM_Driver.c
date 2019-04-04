/*
 * EEPROM_Driver.c
 *
 *  Created on: 18.10.2018
 *      Author: Malte
 */

#include "EEPROM_Driver.h"

/**
 * Initializes EEPROM communication.
 * @param spi_per SPI Peripheral
 * @return Status
 */
EEPROM_STATUS EEPROM_Ini()
{

	/* GPIO Ports Clock Enable*/
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* SPI1 clock enable */
	__HAL_RCC_SPI1_CLK_ENABLE();

	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}
		/**SPI1 GPIO Configuration
		PA5     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PA8     ------> SPI1_SS
		*/

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* SPI1 interrupt Init */
		HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);

		//Enable Interrupt
		SPI1->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE); //Nothing to do -> Interrupt disable
		SPI1->CR2 |= SPI_CR2_ERRIE;

		//Enable SPI
		SPI1->CR1 |= SPI_CR1_SPE;

		//Reset Interrupt Variables
		write_state = EEPROM_WRITE_INACTIVE;
		read_state = EEPROM_READ_INACTIVE;

		//Reset cooldown
		coolD = 0;

		return EEPROM_STATUS_READY;

}


/* ################## READ #################### */

EEPROM_RESULT EEPROM_Read_Byte(uint32_t address, uint8_t *dest)
{
	return EEPROM_RESULT_ERROR;
}

/**
 * Read memory at address. Last Byte is valid-Bit (0x55).
 * @param address Startaddress
 * @param dest Destination (Should be allocated by len + 1)
 * @param len Length of Bytestream
 *
 */
EEPROM_RESULT EEPROM_Read(uint32_t address, uint8_t *dest, uint32_t len)
{
	if(read_state == EEPROM_READ_INACTIVE)
	{
		read_len = len;
		read_state = EEPROM_READ0_WAIT;
		read_adr = address;
		read_dest = dest;
		//Enable Interrupt
		SPI1->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE;

		return EEPROM_RESULT_OK;
	} else
	{
		return EEPROM_RESULT_ERR_RIP;
	}
}

EEPROM_RESULT EEPROM_Read_blocking(uint32_t address, uint8_t *dest, uint32_t len, EEPROM_RESULT *result,uint32_t timeout)
{
	*result = EEPROM_RESULT_ERR_RIP;
	while(EEPROM_Read(address, dest, len) == EEPROM_RESULT_ERR_RIP && timeout-- > 0);
	if(timeout > 0)
	{
		*result = EEPROM_RESULT_OK;
		return EEPROM_RESULT_OK;
	}
	else
		return EEPROM_RESULT_ERR_TIMEOUT;
}


/* ################## WRITE #################### */

EEPROM_RESULT EEPROM_Write_Byte(uint32_t address, uint8_t *source)
{
	return EEPROM_RESULT_OK;
}

EEPROM_RESULT EEPROM_Write(uint32_t address, uint8_t *source, uint32_t len)
{
	if(read_state == EEPROM_READ_INACTIVE)
	{
		write_len = len;
		write_state = EEPROM_WRITE0_WAIT;
		write_adr = address;
		write_sou = source;
		//Enable Interrupt
		SPI1->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE;
		return EEPROM_RESULT_OK;
	} else
	{
		return EEPROM_RESULT_ERR_WIP;
	}
}

EEPROM_RESULT EEPROM_Write_blocking(uint32_t address, uint8_t *source, uint32_t len, EEPROM_RESULT *result, uint32_t timeout)
{
	*result = EEPROM_RESULT_ERR_WIP;
	while(EEPROM_Write(address, source, len) == EEPROM_RESULT_ERR_WIP && timeout-- > 0);
	if(timeout > 0)
	{
		*result = EEPROM_RESULT_OK;
		return EEPROM_RESULT_OK;
	}
	else
		return EEPROM_RESULT_ERR_TIMEOUT;
}


/* ################## ACTIONS #################### */

EEPROM_STATUS EEPROM_WriteE()
{
	return EEPROM_SendCommand(OPCODE_WREN);
}

EEPROM_STATUS EEPROM_ReadStatus()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); //Slave select
	uint8_t pData;
	uint8_t dummy = 0;
	pData = OPCODE_RDSR;
	HAL_SPI_Transmit(&hspi1, &pData, 1, 0xFFFF);
	HAL_SPI_TransmitReceive(&hspi1, &dummy, &pData, 1, 0xFFFF);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); //Slave deSelect


	if(pData & (1<<0))
		return EEPROM_STATUS_WIP;
	else if(!(pData & (1<<1)))
		return EEPROM_STATUS_WRITE_NE;
	else if(read_state != EEPROM_READ_INACTIVE)
			return EEPROM_STATUS_RIP;
	else
		return EEPROM_STATUS_READY;


}

EEPROM_STATUS EEPROM_SendCommand(uint8_t opCode)
{
	selectSlave(0);
	HAL_SPI_Transmit(&hspi1, &opCode, 1, 0xFFFF);
	selectSlave(1);

	return EEPROM_ReadStatus();
}


/* ################## BONUS FUNCTION #################### */

void freeDR()
{
	uint8_t dummy = SPI1->DR;
}

void selectSlave(uint8_t status)
{
	if(status)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); //Slave deselect
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); //Slave select
}

void tickEnableIR(void)
{
	if(coolD > 0)
		coolD--;
	else if(coolD == 0)
	{
		//Enable Interrupt
		SPI1->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE;
		coolD = -1;
	}

}

