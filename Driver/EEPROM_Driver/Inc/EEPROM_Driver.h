/*
 * EEPROM_Driver.h
 *
 *  Created on: 18.10.2018
 *      Author: Malte
 */

#ifndef EEPROM_DRIVER_H_
#define EEPROM_DRIVER_H_

/* --- Includes --- */
#include "stm32f1xx_hal.h"

/* ### --- Manual --- ###
 * Connection:
 * PA5     ------> SPI1_SCK (SCK)
 * PA6     ------> SPI1_MISO(SO)
 * PA7     ------> SPI1_MOSI(SI)
 * PA8     ------> SPI1_SS	(CS)
 * 3V3	   ------> Vcc, WP, HOLD
 * GND	   ------> Vss
 *
 * EEPROM_Ini() -> EEPROM_WriteE() -> Status check (Ready?)
 * EEPROM_Read_blocking / EEPROM_Write_blocking
 * EEPROM_SendCommand(OPCODE_CE); (Wait until WIP-flags is not active)
 */


/* --- Defines --- */

#define OPCODE_READ 0x03
#define OPCODE_WRITE 0x02
#define OPCODE_WREN 0x06
#define OPCODE_WRDI 0x04
#define OPCODE_RDSR 0x05
#define OPCODE_CE 0x07


/* --- Variables --- */
SPI_TypeDef *spi;
typedef enum
{
	EEPROM_STATUS_READY = 0, EEPROM_STATUS_WIP, EEPROM_STATUS_WRITE_NE, EEPROM_STATUS_RIP
}EEPROM_STATUS;

typedef enum
{
	EEPROM_RESULT_ERROR = 0, EEPROM_RESULT_OK, EEPROM_RESULT_ERR_RIP, EEPROM_RESULT_ERR_WIP, EEPROM_RESULT_ERR_TIMEOUT
}EEPROM_RESULT;

typedef enum
{
	EEPROM_WRITE_INACTIVE = 0, EEPROM_WRITE0_WAIT, EEPROM_WRITE1_OPCODE, EEPROM_WRITE1A_ADR, EEPROM_WRITE1B_ADR, EEPROM_WRITE1C_ADR, EEPROM_WRITE2_SEND, EEPROM_WRITE3_EXIT
}EEPROM_WRITE;

typedef enum
{
	EEPROM_READ_INACTIVE = 0, EEPROM_READ0_WAIT, EEPROM_READ1_OPCODE, EEPROM_READ1A_ADR, EEPROM_READ1B_ADR, EEPROM_READ1C_ADR, EEPROM_READ2A_RECEIVE, EEPROM_READ2B_RECEIVE, EEPROM_READ3_EXIT
}EEPROM_READ;

//Write
EEPROM_WRITE write_state;
int32_t write_len;
uint32_t write_adr;
uint8_t *write_sou;

//Read
EEPROM_READ read_state;
int32_t read_len;
uint32_t read_adr;
uint8_t *read_dest;

//Cooldown
int32_t coolD;

SPI_HandleTypeDef hspi1;



/* --- Functions --- */
EEPROM_STATUS EEPROM_Ini(void);
EEPROM_STATUS EEPROM_ReadStatus(void);

EEPROM_STATUS EEPROM_WriteE(void);
EEPROM_STATUS EEPROM_SendCommand(uint8_t opCode);

EEPROM_RESULT EEPROM_Read_Byte(uint32_t address, uint8_t *dest);
EEPROM_RESULT EEPROM_Read(uint32_t address, uint8_t *dest, uint32_t len);
/**
 * Reads data from the EEPROM.
 * @param address starting address location on EEPROM
 * @param dest destination array
 * @param len length of data to be written
 * @param result status of reading
 * @param timeout # of attempts before timeout
 * @return feedback of writing attempt
 */
EEPROM_RESULT EEPROM_Read_blocking(uint32_t address, uint8_t *dest, uint32_t len, EEPROM_RESULT *result, uint32_t timeout);


EEPROM_RESULT EEPROM_Write_Byte(uint32_t address, uint8_t *source);
EEPROM_RESULT EEPROM_Write(uint32_t address, uint8_t *source, uint32_t len);
/**
 * Writes data to the EEPROM.
 * @param address starting address location on EEPROM
 * @param source source array
 * @param len length of data to be written
 * @param result status of writing
 * @param timeout # of attempts before timeout
 * @return feedback of writing attempt
 */
EEPROM_RESULT EEPROM_Write_blocking(uint32_t address, uint8_t *source, uint32_t len, EEPROM_RESULT *result, uint32_t timeout);

void freeDR(void);
void selectSlave(uint8_t status);
void tickEnableIR(void);


#endif /* EEPROM_DRIVER_H_ */
