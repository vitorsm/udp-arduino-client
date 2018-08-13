/*
 * espUtils.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef ESPUTILS_H_
#define ESPUTILS_H_

#define DEFAULT_TIMEOUT 1000
#define MAX_ATTEMPTS 10
#define DEBUG 1

#include "utils.h"
#include "constants.h"

typedef int (sendDataFunc)(char *command, const int timeout, int debug);

void moduleReset(sendDataFunc *sendData);

int connectToWifi(sendDataFunc *sendData, char *ssid, char *password);

int setStationMode(sendDataFunc *sendData);

void showLocalIpAddress(sendDataFunc *sendData);

int setMultipleConnections(sendDataFunc *sendData);

int enableShowRemoteIp(sendDataFunc *sendData);

int startServer(sendDataFunc *sendData);

int sendHelloMessage(sendDataFunc *sendData);

void sendMessage(sendDataFunc *sendData, char *message, char *ipAddress, int port);

#endif /* ESPUTILS_H_ */
