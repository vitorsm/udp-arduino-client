/*
 * espUtils.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "espUtils.h"


void moduleReset(sendDataFunc *sendData) {
  
  sendData("AT+RST", DEFAULT_TIMEOUT * 2, DEBUG, MAXIMUM_ATTEMPTS);
  
}

int connectToWifi(sendDataFunc *sendData, char *ssid, char *password, serialPrintFunc *serialPrint) {
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
//  serialPrint("cw:", 1);
//  serialPrint(command, 1);
  
  int response = sendData(command, DEFAULT_TIMEOUT * 2, DEBUG, MAXIMUM_ATTEMPTS);
//  free(part1);
//  free(part2);
//  free(part3);
//  free(command);
  
  return response;
}

int setStationMode(sendDataFunc *sendData) {
  return sendData("AT+CWMODE_CUR=1", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
}

int showLocalIpAddress(sendDataFunc *sendData) {
  return sendData("AT+CIFSR", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
}

int setMultipleConnections(sendDataFunc *sendData) {
  return sendData("AT+CIPMUX=1", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
}

int enableShowRemoteIp(sendDataFunc *sendData) {
  return sendData("AT+CIPDINFO=1", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
}

int startServer(sendDataFunc *sendData) {

  char clientPortStr[] = {0, 0, 0, 0, 0, 0, 0, 0};
  convertIntToBytes(CLIENT_PORT, clientPortStr, 7);
  
  char *part1 = "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",0,";
  char *part2 = ",2";

  int charSize = strlen(part1) + strlen(part2) + strlen(clientPortStr) + 1;
  
  char charCommand[charSize + 1];
  
  concatString(part1, clientPortStr, charCommand);  
  concatString(charCommand, part2, charCommand);
  charCommand[charSize] = 0;
  
  int response = sendData(charCommand, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);

//  free(part1);
//  free(part2);
//  free(charCommand);
  
  return response;
}

int startAccessPoint(sendDataFunc *sendData, char *ssid) {

    char *part1 = "AT+CWSAP_CUR=\"";
    //char *part2 = "\",\"\",";
    char *part2 = "\",\"senha\",";
    char *part3 = ",";
    char *part4 = ",1,0";

    //AT+CWSAP_CUR="Nome rede",,1⸮

    char channelAP[2];
    char encryptionModeAP[2];

    convertIntToBytes(CHANNEL_AP, channelAP, 2);
    convertIntToBytesIgnoreNegative(ENCRYPTATION_MODE_AP, encryptionModeAP, 2);
    
    int charSize = strlen(part1) + strlen(part2) + strlen(part3) + strlen(part4) + strlen(ssid) + strlen(channelAP) + strlen(encryptionModeAP) + 1;
    
    char strCommand[charSize];
  
    concatString(part1, ssid, strCommand);
    concatString(strCommand, part2, strCommand);
    concatString(strCommand, channelAP, strCommand);
    concatString(strCommand, part3, strCommand);
    concatString(strCommand, encryptionModeAP, strCommand);
    concatString(strCommand, part4, strCommand);

    strCommand[charSize] = 0;
    
//    int resp = sendData(strCommand, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
//    //analisar se precisa de um delay
//    if (resp == 1)
//      resp = sendData("AT+CWMODE_CUR=2", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);

    int resp = sendData("AT+CWMODE_CUR=2", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
    if (resp == 1)
      resp = sendData(strCommand, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);

//    free(part1);
//    free(part2);
//    free(part3);
//    free(part4);
//    free(strCommand);
//    free(channelAP);
//    free(encryptionModeAP);
    
    return resp;
}


int stopAccessPoint(sendDataFunc *sendData) {
  
  int response = setStationMode(sendData);
  
  if (response == 1) {
    response = sendData("AT+CWQAP", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
  }

  return response;
}


int startTCPServer(sendDataFunc *sendData, int port) {
//  char *part1 = "AT+CIPSERVER=1,";
//
//  int sizeStrPort = 7;
//  char portStr[sizeStrPort];
//  convertIntToBytes(port, portStr, sizeStrPort);
//
//  int commandSize = strlen(part1) + strlen(portStr) + 1;
//  char strCommand[commandSize];
//  concatString(part1, portStr, strCommand);

  return sendData("AT+CIPSERVER=1", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
}

int stopTCPServer(sendDataFunc *sendData, int port) {
  char *part1 = "AT+CIPSERVER=0,";

  int sizeStrPort = 7;
  char portStr[sizeStrPort];
  convertIntToBytes(port, portStr, sizeStrPort);

  int commandSize = strlen(part1) + strlen(portStr) + 1;
  char strCommand[commandSize];
  concatString(part1, portStr, strCommand);

  int response = sendData(strCommand, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);

//  free(part1);
//  free(portStr);
//  free(strCommand);
  
  return response;
}

int listAPs(sendDataFunc *sendData) {
  
  return sendData("AT+CWLAP", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
//  return sendData("AT+CWLAP=,,,1,,", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
  
}

int sendHelloMessage(sendDataFunc *sendData) {

  char address[15];
  getNetworkAddress(sendData, address);

  
  char message[MESSAGE_LENGTH];
  buildHelloMassage(assetId, password, message);

  int response = sendMessage(sendData, message, address, SERVER_PORT);
  
//  free(address);
//  free(message);
  
  return response;
}

int sendMessage(sendDataFunc *sendData, char *message, char *ipAddress, int port) {
  
}

void getNetworkAddress(sendDataFunc *sendData, char *address) {
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

