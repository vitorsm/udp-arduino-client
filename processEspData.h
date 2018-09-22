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

typedef int (sendDataFunc)(char *command, const int timeout, int debug, int maxAttempts);
int brokerIpAddressFound;
char brokerIpAddress[16];
typedef void (serialPrintFunc)(char *message, int isPrintln);

void proccessReceivedData(sendDataFunc *sendData, char *data, serialPrintFunc *serialPrint);
void getDataFromReceivedData(char *data, int *connectionId, int *messageLength, char *ipAddress, int *port, serialPrintFunc *serialPrint);
void proccessReceivedMessage(sendDataFunc *sendData, char *message, char *originIp, int originPort, serialPrintFunc *serialPrint);
void getBrokerIpAddress(char *ipAddress, serialPrintFunc *serialPrint);
//int getStartsMessage(char *message);
void getMessageBounds(char *message, int *startType, int *endsType, int *startMessage, serialPrintFunc *serialPrint);


#endif /* PROCESSESPDATA_H_ */
