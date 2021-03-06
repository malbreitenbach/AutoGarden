/*
 * protocol_bt.c
 *
 *  Created on: 07.05.2019
 *      Author: Malte
 */

#include "protocol_bt.h"

void transmitReply(PDU *msg)
{
	generateChecksum(msg);
	sendPDU(msg);
}

void newMessage(PDU_fromSmart *msg)
{
	messageAvailable = true;
	currentMessage = *msg;
}

bool newMessageIsAvailable()
{
	return messageAvailable;
}

COMFROMSMART getRecMessageType()
{
	return currentMessage.PDU_fromSmart_s.command;
}

PDU_fromSmart* getRecMessage()
{
	messageAvailable = false;
	return &currentMessage;
}




