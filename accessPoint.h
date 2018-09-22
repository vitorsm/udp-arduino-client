/*
 * accessPoint.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef ACCESSPOINT_H_
#define ACCESSPOINT_H_

#define WIFI_SSID "Nome rede"
#include "espUtils.h"
#include "constants.h"

typedef int (sendDataFunc)(char *command, const int timeout, int debug, int maxAttempts);
typedef void (serialPrintFunc)(char *message, int isPrintln);

//const char *SSID_A = "Nome rede";

void startAccessPointConfig(sendDataFunc *sendData);
void stopAccessPointConfig(sendDataFunc *sendData);

void startServerConfig(sendDataFunc *sendData);
void stopServerConfig(sendDataFunc *sendData);

void processResponseListAPs(sendDataFunc *sendData, char *command);
void processRequestNetworks(sendDataFunc *sendData);

void processWifiConfig(sendDataFunc *sendData, char *command, serialPrintFunc *serialPrint);
void processCredentialsConfig(sendDataFunc *sendData);

// private
void getDataWifiConfig(char *command, char *ssid, char *netMacAddress, char *passwordWifi, char *id, char *password, serialPrintFunc *serialPrint);

#endif /* ACCESSPOINT_H_ */
