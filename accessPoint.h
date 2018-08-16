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

//const char *SSID_A = "Nome rede";

void startAccessPointConfig(sendDataFunc *sendData);
void stopAccessPointConfig(sendDataFunc *sendData);

void startServerConfig(sendDataFunc *sendData);
void stopServerConfig(sendDataFunc *sendData);

void proccessResponseListAPs(char *command);
void proccessRequestNetworks(sendDataFunc *sendData);

void proccessWifiConfig(sendDataFunc *sendData);
void proccessCredentialsConfig(sendDataFunc *sendData);

#endif /* ACCESSPOINT_H_ */
