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
char token[MESSAGE_TOKEN_LENGTH + 1];
char brokerIpAddress[16];

void moduleReset();

int connectToWifi(char *ssid, char *password);

int setStationMode();

int showLocalIpAddress();

int setMultipleConnections();

int enableShowRemoteIp();

int startServer();

void getNetworkAddress(char *address);

int startAccessPoint(char *ssid);

int stopAccessPoint();

int startTCPServer(int port);

int stopTCPServer(int port);

int listAPs();

//Criar um arquivo para tratar de conversas com a rede de nos

int sendPublishMessage(float value, char *topic);

int sendHelloMessage();

int sendMessage(char *message, char *ipAddress, int port);

void setCredentials(char *assetId, char *password);

int prepareToSendUdpMessage(char *addressIp, int port, int messageSize);


#endif /* ESPUTILS_H_ */
