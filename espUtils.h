/*
 * espUtils.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef ESPUTILS_H_
#define ESPUTILS_H_

typedef int (sendDataFunc)(char *command, const int timeout, int debug);

void moduleReset(sendDataFunc *func);

int connectToWifi(sendDataFunc *func, char *ssid, char *password);

int setStationMode(sendDataFunc *func);

void showLocalIpAddress(sendDataFunc *func);

int voidMultipleConnections(sendDataFunc *func);

int enableShowRemoteIp(sendDataFunc *func);

int startServer(sendDataFunc *func);

int sendHelloMessage(sendDataFunc *func);

void sendMessage(sendDataFunc *func, char *message, char *ipAddress, int port);

#endif /* ESPUTILS_H_ */
