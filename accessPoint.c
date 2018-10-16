/*
 * accessPoint.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "accessPoint.h"

void startAccessPointConfig(sendDataFunc *sendData) {

  startAccessPoint(sendData, WIFI_SSID);
  startServerConfig(sendData);
  
}

void stopAccessPointConfig(sendDataFunc *sendData) {
  
  stopAccessPoint(sendData);
  
}

void startServerConfig(sendDataFunc *sendData) {

//  startTCPServer(sendData, CLIENT_PORT);
  startServer(sendData);
  
}

void stopServerConfig(sendDataFunc *sendData) {

  stopTCPServer(sendData, CLIENT_PORT);
  
}

void processRequestNetworks(sendDataFunc *sendData) {
  listAPs(sendData); 
}

void processResponseListAPs(sendDataFunc *sendData, char *command) {
  int commandSize = strlen(command);

  // colocar aqui um tratamento de string para mandar a lista de redes wifi disponiveis
  // é necessário avaliar quais dados serão necessários enviar
  // o aplicativo irá ler os dados

  // implementar o build message networks information
}

void processWifiConfig(sendDataFunc *sendData, char *command, serialPrintFunc *serialPrint, printConstantsMessages *printConstants) {

  int commandSize = strlen(command);

  char ssid[commandSize];
  clearString(ssid, commandSize);
  
  char netMacAddress[commandSize];
  clearString(netMacAddress, commandSize);
  
  char passwordWifi[commandSize];
  clearString(passwordWifi, commandSize);
  
  char id[CLIENT_ID_LENGTH];
  clearString(id, commandSize);
  
  char password[CLIENT_PASSWORD_LENGTH];
  clearString(password, commandSize);
 
  if (DEBUG == 1) {
    printConstants(MESSAGE_INDEX_PROCESS_WIFI_CONFIG_1, 1);
    serialPrint(command, 1);
  }
  
  getDataWifiConfig(command, ssid, netMacAddress, passwordWifi, id, password, serialPrint, printConstants);

  if (DEBUG == 1) {
    printConstants(MESSAGE_INDEX_PROCESS_WIFI_CONFIG_SSID, 0);
    serialPrint(ssid, 1);

    printConstants(MESSAGE_INDEX_PROCESS_WIFI_CONFIG_MAC, 0);
    serialPrint(netMacAddress, 1);

    printConstants(MESSAGE_INDEX_PROCESS_WIFI_CONFIG_PASS_W, 0);
    serialPrint(passwordWifi, 1);

    printConstants(MESSAGE_INDEX_PROCESS_WIFI_CONFIG_LOGIN, 0);
    serialPrint(id, 1);
    
    printConstants(MESSAGE_INDEX_PROCESS_WIFI_CONFIG_PASS_L, 0);
    serialPrint(password, 1);
  }
  
  int response = stopTCPServer(sendData, CLIENT_PORT);

  // analisar se é possível testar as credenciais de uma rede sem deixar de ser AP

  if (response == 1) {
    response = stopAccessPoint(sendData);
  }
  
  if (response == 1) {
    response = connectToWifi(sendData, ssid, passwordWifi, serialPrint, printConstants);
  }
  
  if (response == 1) {
    response = startServer(sendData);
  }
  
  setCredentials(id, password);
  
  if (response == 0) {
    // se tiver um lcd mostrar aqui que a autenticação falhou
    if (DEBUG == 1) {
      printConstants(MESSAGE_INDEX_ERROR, 1);  
    }
  }
}

void getDataWifiConfig(char *command, char *ssid, char *netMacAddress, char *passwordWifi, char *id, char *password, serialPrintFunc *serialPrint, printConstantsMessages *printConstants) {
  int emptyCharCount = 0;

  if (DEBUG == 1) {
    printConstants(MESSAGE_INDEX_DATA_WIFI_CONFIG, 1);
    serialPrint(command, 1);
  }
  int commandSize = strlen(command);

  int wordCount = 0;
  
  for (int i = 0; i < commandSize; i++) {
    char c = command[i];

    if (c == EMPTY_CHAR) {
      emptyCharCount++;
      wordCount = 0;
    } else {
      if (emptyCharCount == 1) {
        ssid[wordCount] = c;
        wordCount++;
        ssid[wordCount] = 0;
        //Coloquei esse 10 aqui so pra ele nunca entrar nesse if
        //Quando eu coloco o MAC Address na mensagem ela nem chega no main
        //Por motivos de tempo nao me preocupei em descobrir o q aconteceu
      } else if (emptyCharCount == 10) {
        netMacAddress[wordCount] = c;
        wordCount++;
        netMacAddress[wordCount] = 0;
      } else if (emptyCharCount == 2) {
        passwordWifi[wordCount] = c;
        wordCount++;
        passwordWifi[wordCount] = 0;
      } else if (emptyCharCount == 3) {
        id[wordCount] = c;
        wordCount++;
        id[wordCount] = 0;
      } else if (emptyCharCount == 4) {
        password[wordCount] = c;
        wordCount++;
        password[wordCount] = 0;
      }
    }
  }

  
}

