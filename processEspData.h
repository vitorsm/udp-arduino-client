/*
 * processEspData.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef PROCESSESPDATA_H_
#define PROCESSESPDATA_H_

#include "utils.h"
#include "constants.h"
#include "accessPoint.h"
#include "digitalControl.h"

int brokerIpAddressFound;

void proccessReceivedData(char *data);
void getDataFromReceivedData(char *data, int *connectionId, int *messageLength, char *ipAddress, int *port);
void proccessReceivedMessage(char *message, char *originIp, int originPort);
void getBrokerIpAddress(char *ipAddress);
//int getStartsMessage(char *message);
void getMessageBounds(char *message, int *startType, int *endsType, int *startMessage);


#endif /* PROCESSESPDATA_H_ */
