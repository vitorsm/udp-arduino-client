/*
 * espUtils.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "espUtils.h"


void moduleReset(sendDataFunc *sendData) {
  int result = 0;

  while (result == 0) {
    result = sendData("AT+RST", DEFAULT_TIMEOUT * 2, DEBUG);
  }
  
}

int connectToWifi(sendDataFunc *sendData, char *ssid, char *password) {
  char *part1 = "AT+CWJAP=\"";
  char *part2 = "\",\"";
  char *part3 = "\"";

  int charSize = strlen(part1) + strlen(part2) + strlen(part3) + strlen(ssid) + strlen(password) + 1;
  char command[charSize];
  for (int i = 0; i < charSize; i++) {
    command[i] = 0;
  }

  concatString(part1, ssid, command);
  concatString(command, part2, command);
  concatString(command, password, command);
  concatString(command, part3, command);
  command[charSize] = 0;
  
  //sendData("AT+CWJAP=\"2.4Ghz Virtua 302\",\"3207473600\"", DEFAULT_TIMEOUT * 2, DEBUG);
  sendData(command, DEFAULT_TIMEOUT * 2, DEBUG);
}

int setStationMode(sendDataFunc *sendData) {
  sendData("AT+CWMODE=1", DEFAULT_TIMEOUT, DEBUG);
}

void showLocalIpAddress(sendDataFunc *sendData) {
  sendData("AT+CIFSR", DEFAULT_TIMEOUT, DEBUG);
}

int setMultipleConnections(sendDataFunc *sendData) {
  sendData("AT+CIPMUX=1", DEFAULT_TIMEOUT, DEBUG);
}

int enableShowRemoteIp(sendDataFunc *sendData) {
  sendData("AT+CIPDINFO=1", DEFAULT_TIMEOUT, DEBUG);
}

int startServer(sendDataFunc *sendData) {

  char clientPortStr[] = {0, 0, 0, 0, 0, 0, 0, 0};
  convertIntToBytes(CLIENT_PORT, clientPortStr, 7);
  
  char *part1 = "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",0,";
  char *part2 = ",2";

  int charSize = strlen(part1) + strlen(part2) + strlen(clientPortStr);
  
  char charCommand[charSize + 1];
  
  concatString(part1, clientPortStr, charCommand);  
  concatString(charCommand, part2, charCommand);
  charCommand[charSize] = 0;
  
  sendData(charCommand, DEFAULT_TIMEOUT, DEBUG);
}

int sendHelloMessage(sendDataFunc *sendData) {

  char address[15];
  getNetworkAddress(address);

  
  char message[MESSAGE_LENGTH];
  buildHelloMassage(assetId, password, message);

  sendMessage(sendData, message, address, SERVER_PORT);
  
}

void sendMessage(sendDataFunc *sendData, char *message, char *ipAddress, int port) {
  
}

void getNetworkAddress(char *address) {
  address = "192.168.0.255";
}

void setCredentials(char *_assetId, char *_password) {
  int i = 0;
  char c = _assetId[i];
  while (c != 0) {
    assetId[i] = _assetId[0];
  }

  i = 0;
  c = _password[i];
  while (c != 0) {
    password[i] = _password[i];
  }
}

