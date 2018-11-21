/*
 * accessPoint.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef ACCESSPOINT_H_
#define ACCESSPOINT_H_

#define WIFI_SSID "Rede MC"
#include "espUtils.h"
#include "constants.h"

int wifiConnected;

//const char *SSID_A = "Nome rede";

void startAccessPointConfig();
void stopAccessPointConfig();

void startServerConfig();
void stopServerConfig();

void processResponseListAPs(char *command);
void processRequestNetworks();

void processWifiConfig(char *command);
void processCredentialsConfig();

// private
void getDataWifiConfig(char *command, char *ssid, char *netMacAddress, char *passwordWifi, char *id, char *password);

void conectWifi();
#endif /* ACCESSPOINT_H_ */
