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

  startTCPServer(sendData, CLIENT_PORT);
  
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

void processWifiConfig(sendDataFunc *sendData, char *command) {

  int commandSize = strlen(command);

  char ssid[commandSize];
  char netMacAddress[commandSize];
  char passwordWifi[commandSize];
  char id[CLIENT_ID_LENGTH];
  char password[CLIENT_PASSWORD_LENGTH];
 

  getDataWifiConfig(command, ssid, netMacAddress, passwordWifi, id, password);

  int response = stopTCPServer(sendData, CLIENT_PORT);

  // analisar se é possível testar as credenciais de uma rede sem deixar de ser AP
  
  if (response == 1) {
    response = stopAccessPoint(sendData);
  }

  if (response == 1) {
    response = connectToWifi(sendData, ssid, passwordWifi);
  }

  if (response == 1) {
    response = startServer(sendData);
  }

  setCredentials(id, password);
  
  if (response == 0) {
    // se tiver um lcd mostrar aqui que a autenticação falhou
  }
}

void getDataWifiConfig(char *command, char *ssid, char *netMacAddress, char *passwordWifi, char *id, char *password) {
  int commaCount = 0;
  int emptyCharCount = 0;
  int startGet = 0;
  int wordCount = 0;
  int startEmpty = 0;
  
  for (int i = 0; i < commandSize; i++) {
    char c = command[i];

    if (c == EMPTY_CHAR) {
      startEmpty = 1;
      emptyCharCount++;
      wordCount = 0;
    } else if (startEmpty) {

      if (c == EMPTY_CHAR) {
        emptyCharCount++;
        wordCount = 0;
      } else if (emptyCharCount == 1) {
        passwordWifi[wordCount] = c;
        wordCount++;
      } else if (emptyCharCount == 2) {
        id[wordCount] = c;
        wordCount++;
      } else if (emptyCharCount == 3) {
        password[wordCount] = c;
        wordCount++;
      }
    } else if (startGet == 0 && c == ',') {
      commaCount++;
    } else {
      
      if (commaCount == 1) {
        // Verificar se ja pegou a aspas, pq não faz parte do ssid
        if (startGet == 0) {
          startGet = 1;
        } else {
          if (c == '"') {
            startGet = 0;
            wordCount = 0;
          } else {
            ssid[wordCount] = c;
            wordCount++;
          }
        }
        
      } else if (commaCount == 3) {
        // Verificar se ja pegou a aspas, pq não faz parte do ssid
        if (startGet == 0) {
          startGet = 1;
        } else {
          if (c == '"') {
            startGet = 0;
            wordCount = 0;
          } else {
            netMacAddress[wordCount] = c;
            wordCount++;
          }
        }
      }
    }
  }
}
