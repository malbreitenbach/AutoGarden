/*
 * BT_Serial.c
 *
 *  Created on: 29.04.2019
 *      Author: Malte
 */


#include "BT_Serial.h"

/**
 * Sends message to BT-Device, computes the checksum
 * @return err message, if busy
 */
err sendPDU(PDU* pdu)
{
	if(!(transferInProgress_BTS))
	{
		transferInProgress_BTS = true;
		nextMessage_BTS = *pdu;
		DMA1_Channel4->CCR &= ~(DMA_CCR_EN); //Channel disable
		DMA1_Channel4->CNDTR = sizeof(PDU);
		DMA1_Channel4->CCR |= (DMA_CCR_EN); //Channel enable
		return ERR_OK;
	}else
	{
		return ERR_BTS_ALREADY_IN_PROGRESS;
	}
}

/**
 * Additional ->
 * __HAL_RCC_GPIOA_CLK_ENABLE();
 * __HAL_RCC_ADC1_CLK_ENABLE();
 * __HAL_RCC_USART1_CLK_ENABLE();
 * __HAL_RCC_DMA1_CLK_ENABLE();
 *
 * Uses UART1 and DMA1_C4 & C5
 */
err iniBT_Serial(void)
{
	UART_HandleTypeDef huart1;

  /* USART1 init function */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
	  _Error_Handler(__FILE__, __LINE__);
	}

	//Pin Config
	GPIO_InitTypeDef GPIO_InitStruct;
	/* PA9 = alternate function push/pull output */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* PA10 = floating input */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART1->CR3 |= USART_CR3_DMAT;
	USART1->CR3 |= USART_CR3_DMAR;

	/* --- DMA --- */
	//DMA1_C4 -> USART1_TX
	//DMA1_C5 -> USART1_RX
	DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR); //Peripheral Address TX
	DMA1_Channel5->CPAR = (uint32_t)&(USART1->DR); //Peripheral Address RX

	DMA1_Channel4->CMAR = (uint32_t)&nextMessage_BTS; //Memory Address to store Data to be transmitted
	DMA1_Channel5->CMAR = (uint32_t)&receiveBuffer_BTS; //Memory Address to store received Data

	DMA1_Channel4->CNDTR = sizeof(PDU); //8 transfers per Iteration
	DMA1_Channel5->CNDTR = sizeof(PDU_fromSmart); //8 transfers per Iteration

	DMA1_Channel4->CCR 	=  (0 << DMA_CCR_PL_Pos) | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE; //no Prio, MSIZE = 8b, PSIZE= 8b, MemInc, noCirc, ReadFromMem, TransferCompleteInterruptEnable
	DMA1_Channel5->CCR 	=  (3 << DMA_CCR_PL_Pos) | DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_CIRC; //high Prio, MSIZE = 8b, PSIZE= 8b, MemInc, Circ, ReadFromPer, TransferCompleteInterruptEnable

	USART1->SR &= ~(USART_SR_TC);
	//DMA1_Channel4->CCR |= DMA_CCR_EN; //Channel transmit enable
	DMA1_Channel5->CCR |= DMA_CCR_EN; //Channel receive  enable

	transferInProgress_BTS = false;

	//enable Interrupt
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	//Enable USART
	USART1->CR1 |= USART_CR1_UE;

	return ERR_OK;
}

uint8_t getBytesLeft()
{
	return DMA1_Channel5->CNDTR;
}



void resetReceiveBuffer()
{
	DMA1->IFCR |= DMA_IFCR_CTCIF5 | DMA_IFCR_CGIF5; //Reset Interrupt
	DMA1_Channel5->CNDTR = sizeof(PDU_fromSmart); //jump to start of the buffer
}

void bt_timeout()
{
	static bool MessageReceiving = false;
	static uint8_t lastPosition = 0;
	static uint8_t timeout = 0;

	uint8_t bytesLeft = getBytesLeft();
	if(MessageReceiving)
	{

		if(lastPosition < bytesLeft || bytesLeft == 0)
		{
			//Message transfer complete
			MessageReceiving = false;
		}else if(lastPosition == bytesLeft)
		{
			//Timeout, 1ms without receiving
			if(timeout++ > 20)
			{
				MessageReceiving = false;
				timeout = 0;
				lastPosition = sizeof(PDU_fromSmart) + 1;
				resetReceiveBuffer();
			}
		}else
		{
			//Message still transmitting
			lastPosition = bytesLeft;
			timeout = 0;
		}

	} else if(bytesLeft < sizeof(PDU_fromSmart))
	{
		MessageReceiving = true;
		lastPosition = sizeof(PDU_fromSmart) + 1;
		timeout = 0;
	}
}

/**
 * 1. Check Message
 * 2. Execute
 * 3. Reply
 */
err computePDU()
{
	if(checkChecksum_fromSmart(&receiveBuffer_BTS))
	{
		//Message is fine
		//HAL_UART_Transmit(&huart2, receiveBuffer_BTS.PDU_fromSmart_byteStream, sizeof(receiveBuffer_BTS), 0xFFFF);
		newMessage(&receiveBuffer_BTS);
		return ERR_OK;
	}else
	{
		//Message is corrupted
		//HAL_UART_Transmit(&huart2, receiveBuffer_BTS.PDU_fromSmart_byteStream, sizeof(receiveBuffer_BTS), 0xFFFF);
		//HAL_UART_Transmit(&huart2, "\r\n Message corrupted, reinitialize... \r\n", 41, 0xFFFF);

		return ERR_BTS_CHECKSUM;
	}
//	  PDU nextMessage;
//	  nextMessage.PDU_s.startbyte = 0x55;
//	  nextMessage.PDU_s.command = 0x00;//COMREC_MAN_REPLY
//	  nextMessage.PDU_s.value2 = 1; //r0_pumpOnOff
//	  nextMessage.PDU_s.value3 = 2; //r0_pumpPWM
//	  nextMessage.PDU_s.value4 = 3;
//	  nextMessage.PDU_s.value5 = 7;
//	  nextMessage.PDU_s.value6 = 8;
//	  nextMessage.PDU_s.value7 = 9;
//	  nextMessage.PDU_s.value8 = 10;
//	  nextMessage.PDU_s.value9 = 11;
//	  computeChecksum(&nextMessage);
}

void generateChecksum(PDU *msg)
{
	//Compute simple Checksum
	msg->PDU_s.checksum = computeChecksum(msg);
}

bool checkChecksum(PDU *msg)
{
	//Compute simple Checksum
	return (msg->PDU_s.checksum == computeChecksum(msg));
}

uint8_t computeChecksum(PDU *msg)
{
	register uint8_t sum = 0;
	//Compute simple Checksum
	for(uint8_t cnt = 0; cnt < sizeof(PDU)-1; cnt++)
	{
		sum += msg->PDU_byteStream[cnt];
	}
	return sum;
}

uint8_t computeChecksum_fromSmart(PDU_fromSmart *msg)
{
	register uint8_t sum = 0;
	//Compute simple Checksum
	for(uint8_t cnt = 0; cnt < sizeof(PDU_fromSmart)-1; cnt++)
	{
		sum += msg->PDU_fromSmart_byteStream[cnt];
	}
	return sum;
}

bool checkChecksum_fromSmart(PDU_fromSmart *msg)
{
	return (msg->PDU_fromSmart_s.checksum == computeChecksum_fromSmart(msg));
}

/* IRQ
void DMA1_Channel4_IRQHandler(void)
{
	DMA1->IFCR |= DMA_IFCR_CTCIF4 | DMA_IFCR_CGIF4;
	NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
	transferInProgress_BTS = false;
}
*/
