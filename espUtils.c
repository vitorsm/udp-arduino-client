/*
 * espUtils.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "espUtils.h"


void moduleReset(sendDataFunc *func) {
  
}

int connectToWifi(sendDataFunc *func, char *ssid, char *password);

int setStationMode(sendDataFunc *func);

void showLocalIpAddress(sendDataFunc *func);

int voidMultipleConnections(sendDataFunc *func);

int enableShowRemoteIp(sendDataFunc *func);

int startServer(sendDataFunc *func);

int sendHelloMessage(sendDataFunc *func);

void sendMessage(sendDataFunc *func, char *message, char *ipAddress, int port);
