/*
 * accessPoint.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "accessPoint.h"

void startAccessPointConfig(sendDataFunc *sendData) {

  startAccessPoint(sendData, WIFI_SSID);
  
}

void stopAccessPointConfig(sendDataFunc *sendData) {
  
  stopAccessPoint(sendData);
  
}

void startServerConfig(sendDataFunc *sendData) {

  startTCPServer(sendData, CLIENT_PORT);
  
}

void stopServerConfig(sendDataFunc *sendData) {

  stopTCPServer(sendData, CLIENT_PORT);
  
}

void proccessResponseListAPs(char *command) {
  
}

