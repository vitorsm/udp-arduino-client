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

#define CHANNEL_AP 1
#define ENCRYPTATION_MODE_AP 0 // 0: OPEN, 2: WPA_PSK, 3: WPA2_PSK, WPA_WPA2_PSK

#include "utils.h"
#include "constants.h"

char assetId[MESSAGE_ID_LENGTH];
char password[MESSAGE_PASSWORD_LENGTH];

typedef int (sendDataFunc)(char *command, const int timeout, int debug, int maxAttempts);
typedef void (serialPrintFunc)(char *message, int isPrintln);

void moduleReset(sendDataFunc *sendData);

int connectToWifi(sendDataFunc *sendData, char *ssid, char *password, serialPrintFunc *serialPrint);

int setStationMode(sendDataFunc *sendData);

int showLocalIpAddress(sendDataFunc *sendData);

int setMultipleConnections(sendDataFunc *sendData);

int enableShowRemoteIp(sendDataFunc *sendData);

int startServer(sendDataFunc *sendData);

void getNetworkAddress(sendDataFunc *sendData, char *address);

int startAccessPoint(sendDataFunc *sendData, char *ssid);

int stopAccessPoint(sendDataFunc *sendData);

int startTCPServer(sendDataFunc *sendData, int port);

int stopTCPServer(sendDataFunc *sendData, int port);

int listAPs(sendDataFunc *sendData);

//Criar um arquivo para tratar de conversas com a rede de nos

int sendHelloMessage(sendDataFunc *sendData);

int sendMessage(sendDataFunc *sendData, char *message, char *ipAddress, int port);

void setCredentials(char *assetId, char *password);

#endif /* ESPUTILS_H_ */
