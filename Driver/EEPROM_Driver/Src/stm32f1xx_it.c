/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "EEPROM_Driver.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  //Cooldown
  tickEnableIR();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

	//Clear Interrupt Flag
	HAL_NVIC_ClearPendingIRQ(SPI1_IRQn);

	/* --- Check for Error Flags --- */
	if(SPI1->SR & SPI_SR_OVR)
	{
		__HAL_SPI_CLEAR_OVRFLAG(&hspi1);
	}

	/* --- Check if still active --- */
	if((read_state == EEPROM_READ_INACTIVE && write_state == EEPROM_WRITE_INACTIVE) || coolD > 0)
	{
		SPI1->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE); //Nothing to do -> Interrupt disable
		selectSlave(1);
		return;
	}

	/* --- Read Operation --- */
	if((write_state == EEPROM_WRITE_INACTIVE || write_state == EEPROM_WRITE0_WAIT) && read_state != EEPROM_READ_INACTIVE) //Fuehre ReadOperation nur durch, wenn momentan nichts geschrieben wird
	{
		switch(read_state)
		{
			case EEPROM_READ_INACTIVE:
				//Nothing to read
				break;
			case EEPROM_READ0_WAIT:
					selectSlave(0);
					read_state = EEPROM_READ1_OPCODE;
				break;
			case EEPROM_READ1_OPCODE:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = OPCODE_READ;
					read_state = EEPROM_READ1A_ADR;
				}
				break;
			case EEPROM_READ1A_ADR:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = (read_adr>>16) & 0xFF;
					read_state = EEPROM_READ1B_ADR;
				}
				break;
			case EEPROM_READ1B_ADR:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = (read_adr>>8) & 0xFF;
					read_state = EEPROM_READ1C_ADR;
				}
				break;
			case EEPROM_READ1C_ADR:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = read_adr & 0xFF;
					read_state = EEPROM_READ2A_RECEIVE;
				}
				break;
			case EEPROM_READ2A_RECEIVE:
				freeDR(); //Discard Byte received while last Address Byte
				SPI1->DR = 0x00; //Pull for Data
				read_state = EEPROM_READ2B_RECEIVE;
				break;
			case EEPROM_READ2B_RECEIVE:
				if((SPI1->SR & SPI_SR_RXNE) && read_state)
				{
					*(read_dest++) = SPI1->DR;
					if(--read_len <= 0)
						read_state = EEPROM_READ3_EXIT;
					else
						SPI1->DR = 0x00; //Pull for Data
				}
				break;
			case EEPROM_READ3_EXIT:
				read_state = EEPROM_READ_INACTIVE;
				coolD = 10;
				break;
		}

	}

	/* --- Write Operation --- */
	else
	{
		switch(write_state)
		{
			case EEPROM_WRITE_INACTIVE:
				//Nothing to Transmit
				break;
			case EEPROM_WRITE0_WAIT:
					write_state = EEPROM_WRITE1_OPCODE;
					selectSlave(0);
				break;
			case EEPROM_WRITE1_OPCODE:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = OPCODE_WRITE;
					write_state = EEPROM_WRITE1A_ADR;
				}
				break;
			case EEPROM_WRITE1A_ADR:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = (write_adr>>16)&0xFF;
					write_state = EEPROM_WRITE1B_ADR;
				}
				break;
			case EEPROM_WRITE1B_ADR:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = (write_adr>>8)&0xFF;
					write_state = EEPROM_WRITE1C_ADR;
				}
				break;
			case EEPROM_WRITE1C_ADR:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = (write_adr)&0xFF;
					write_state = EEPROM_WRITE2_SEND;
				}
				break;
			case EEPROM_WRITE2_SEND:
				if(SPI1->SR & SPI_SR_TXE)
				{
					freeDR();
					SPI1->DR = *(write_sou++);
					if(--write_len <= 0)
						write_state = EEPROM_WRITE3_EXIT;
				}
				break;
			case EEPROM_WRITE3_EXIT:
				write_state = EEPROM_WRITE_INACTIVE;
				coolD = 10;
				break;
		}

	}





  /* USER CODE END SPI1_IRQn 0 */
  //HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
