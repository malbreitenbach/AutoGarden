/*
 * BT_Serial.h
 *
 *  Created on: 29.04.2019
 *      Author: Malte
 */

#ifndef BT_SERIAL_H_
#define BT_SERIAL_H_

/* --- Includes --- */
#include "error.h"
#include "protocol_bt.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "usart.h"

/* --- Global Variables --- */
PDU nextMessage_BTS;
PDU_fromSmart receiveBuffer_BTS;
bool transferInProgress_BTS;

err sendPDU(PDU* pdu);
err iniBT_Serial(void);
err computePDU(void);

uint8_t getBytesLeft();
void bt_timeout();
void generateChecksum(PDU *msg);
bool checkChecksum(PDU *msg);
uint8_t computeChecksum(PDU *msg);
bool checkChecksum_fromSmart(PDU_fromSmart *msg);
uint8_t computeChecksum_fromSmart(PDU_fromSmart *msg);

#endif /* BT_SERIAL_H_ */
