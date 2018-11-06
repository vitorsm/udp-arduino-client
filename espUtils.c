/*
 * espUtils.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "espUtils.h"
//#include "processEspData.h"

void moduleReset(sendDataFunc *sendData) {
  
  sendData("AT+RST", DEFAULT_TIMEOUT * 2, DEBUG, MAXIMUM_ATTEMPTS);
  
}

int connectToWifi(sendDataFunc *sendData, char *ssid, char *password, serialPrintFunc *serialPrint, printConstantsMessages *printConstants) {
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
  
  int response = sendData(command, DEFAULT_TIMEOUT * 2, DEBUG, MAXIMUM_ATTEMPTS);
  
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
  
  return response;
}

int startAccessPoint(sendDataFunc *sendData, char *ssid) {

    char *part1 = "AT+CWSAP_CUR=\"";
    char *part2 = "\",\"senha\",";
    char *part3 = ",";
    char *part4 = ",1,0";

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
    
//    //analisar se precisa de um delay

    int resp = sendData("AT+CWMODE_CUR=2", DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
    if (resp == 1)
      resp = sendData(strCommand, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
    
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

int sendHelloMessage(sendDataFunc *sendData, serialPrintFunc *serialPrint, printLCDFunc *printLCD) {

  char address[15];
  getNetworkAddress(sendData, address);

  if (DEBUG == 1) {
    serialPrint(address, 1);
    serialPrint(assetId, 1);
    serialPrint(password, 1);
  }
  
  char message[MESSAGE_LENGTH];
  buildHelloMassage(assetId, password, message);
  
  int response = sendMessage(sendData, message, address, SERVER_PORT, printLCD, serialPrint);
  
//  free(address);
//  free(message);
  
  return response;
}

int sendMessage(sendDataFunc *sendData, char *message, char *ipAddress, int port, printLCDFunc *printLCD, serialPrintFunc *serialPrint) {
  int response = prepareToSendUdpMessage(sendData, ipAddress, port, strlen(message), printLCD, serialPrint);

  if (response == 1)
    response = sendData(message, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);

  if (!response) {
    printLCD(MESSAGE_INDEX_ERROR, 0);
    printLCD(MESSAGE_INDEX_NEW_ATTEMPT, 1);
    return sendMessage(sendData, message, ipAddress, port, printLCD, serialPrint);
  }
 
  return response;
}

void getNetworkAddress(sendDataFunc *sendData, char *address) {
  address[0] = '1';
  address[1] = '9';
  address[2] = '2';
  address[3] = '.';
  address[4] = '1';
  address[5] = '6';
  address[6] = '8';
  address[7] = '.';
  address[8] = '4';
  address[9] = '3';
  address[10] = '.';
  address[11] = '2';
  address[12] = '5';
  address[13] = '5';
  address[14] = 0;
}

void setCredentials(char *_assetId, char *_password) {
  clearString(assetId, CLIENT_ID_LENGTH);
  clearString(password, CLIENT_PASSWORD_LENGTH);
  
  int i = 0;
  char c = _assetId[i];
  while (c != 0) {
    c = _assetId[i];
    assetId[i] = c;
    i++;
  }

  i = 0;
  c = _password[i];
  while (c != 0) {
    c = _password[i];
    password[i] = c;
    i++;
  }
}


int prepareToSendUdpMessage(sendDataFunc *sendData, char *addressIp, int port, int messageSize, printLCDFunc *printLCD, serialPrintFunc *serialPrint) {
  char *part1 = "AT+CIPSEND=0,"; //length
  char *part2 = ",\"";
  char *part3 = "\",";
  
  char portStr[6];
  clearString(portStr, 6);
  convertIntToBytes(port, portStr, 6);
  
  char messageSizeStr[7];
  clearString(messageSizeStr, 7);
  convertIntToBytes(messageSize, messageSizeStr, 7);
  
  int commandSize = strlen(part1) + strlen(part2) + strlen(part3) + strlen(addressIp) + strlen(portStr) + strlen(messageSizeStr) + 1;
    
  char strCommand[commandSize];
  clearString(strCommand, commandSize);

  concatString(part1, messageSizeStr, strCommand);
  concatString(strCommand, part2, strCommand);
  concatString(strCommand, addressIp, strCommand);
  concatString(strCommand, part3, strCommand);
  concatString(strCommand, portStr, strCommand);
  return sendData(strCommand, DEFAULT_TIMEOUT, DEBUG, MAXIMUM_ATTEMPTS);
}


