/*
 * protocol_bt.h
 *
 *  Created on: 29.04.2019
 *      Author: Malte
 */

#ifndef PROTOCOL_BT_H_
#define PROTOCOL_BT_H_

#include <stdint.h>
#include <stdbool.h>
//#include "BT_Serial.h"

#define STARTBYTE_DATA 0x54
#define STARTBYTE_COMMAND 0x55

//#define COMMAND_MANUAL_CONTROL 0
//#define COMMAND_READ_SENSORY 1
// #define COMMAND_UPDATE_TIME 2
//#define COMMAND_DATA_REQUEST 3
//#define COMMAND_RESYNC 4
//#define COMMAND_AVAILABILITY_REQUEST 5
//#define COMMAND_CHECK_DATA_AVAILABILITY 6
//#define COMMAND_REQUEST_DATA_INFO 7

/* --- typedef --- */
typedef enum
{
	COMFROMSMART_MANUAL_CONTROL = 0,
	COMFROMSMART_READ_SENSORY = 1,
	COMFROMSMART_UPDATE_TIME = 2,
	COMFROMSMART_DATA_REQUEST = 3,
	COMFROMSMART_RESYNC = 4,
	COMFROMSMART_AVAILABILITY_REQUEST = 5,
	COMFROMSMART_CHECK_DATA_AVAILABILITY = 6,
	COMFROMSMART_REQUEST_DATA_INFO = 7
}COMFROMSMART;

typedef struct
{
	uint8_t startbyte;
	uint8_t command;
	uint8_t value2;
	uint8_t value3;
	uint8_t value4;
	uint8_t value5;
	uint8_t value6;
	uint8_t value7;
	uint8_t value8;
	uint8_t value9;
	uint8_t checksum;
}PDU_struct;

typedef union
{
	PDU_struct PDU_s;
	uint8_t PDU_byteStream[sizeof(PDU_struct)];
}PDU;

typedef struct
{
	uint8_t startbyte;
	uint8_t command;
	uint8_t value2;
	uint8_t value3;
	uint8_t value4;
	uint8_t value5;
	uint8_t value6;
	uint8_t checksum;
}PDU_struct_fromSmart;

typedef union
{
	PDU_struct_fromSmart PDU_fromSmart_s;
	uint8_t PDU_fromSmart_byteStream[sizeof(PDU_struct_fromSmart)];
}PDU_fromSmart;

typedef enum
{
	COMREC_MAN_REPLY, COMREC_TIMEDIFF, COMREC_RESYNC, COMREC_AVAILIBILITY, COMREC_EMPTY, COMREC_REQUEST_DATA_INFO
}COMREC;

/* --- Prototypes --- */
void transmitReply(PDU *msg);
void newMessage(PDU_fromSmart *msg);
bool newMessageIsAvailable();
COMFROMSMART getRecMessageType();
PDU_fromSmart* getRecMessage();

/* --- Variables --- */
volatile PDU_fromSmart currentMessage;
volatile bool messageAvailable;





#endif /* PROTOCOL_BT_H_ */
