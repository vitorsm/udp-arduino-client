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

void processWifiConfig(sendDataFunc *sendData, char *command, serialPrintFunc *serialPrint) {

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
    serialPrint("Vai iniciar o processWifiConfig com: ", 1);
    serialPrint(command, 1);
  }
  
  getDataWifiConfig(command, ssid, netMacAddress, passwordWifi, id, password, serialPrint);

  if (DEBUG == 1) {
    serialPrint("SSID: ", 0);
    serialPrint(ssid, 1);

    serialPrint("MACAddress: ", 0);
    serialPrint(netMacAddress, 1);

    serialPrint("PassWifi: ", 0);
    serialPrint(passwordWifi, 1);

    serialPrint("LoginToken: ", 0);
    serialPrint(id, 1);
    
    serialPrint("PassToken: ", 0);
    serialPrint(password, 1);
    
  }
  
  int response = stopTCPServer(sendData, CLIENT_PORT);

  // analisar se é possível testar as credenciais de uma rede sem deixar de ser AP

  if (DEBUG == 1) {
    serialPrint("passou1", 1);  
  }
  
  if (response == 1) {
    response = stopAccessPoint(sendData);
  }

  if (DEBUG == 1) {
    serialPrint("passou2", 1);  
  }
  
  if (response == 1) {
    response = connectToWifi(sendData, ssid, passwordWifi, serialPrint);
  }

  if (DEBUG == 1) {
    serialPrint("passou3", 1);  
  }
  
  if (response == 1) {
    response = startServer(sendData);
  }

  if (DEBUG == 1) {
    serialPrint("passou4", 1);  
  }
  
  setCredentials(id, password);
  
  if (response == 0) {
    // se tiver um lcd mostrar aqui que a autenticação falhou
    if (DEBUG == 1) {
      serialPrint("deu ruim", 1);  
    }
  }
}

//void getDataWifiConfig(char *command, char *ssid, char *netMacAddress, char *passwordWifi, char *id, char *password) {
//  int commaCount = 0;
//  int emptyCharCount = 0;
//  int startGet = 0;
//  int wordCount = 0;
//  int startEmpty = 0;
//
//  int commandSize = strlen(command);
//  
//  for (int i = 0; i < commandSize; i++) {
//    char c = command[i];
//
//    if (c == EMPTY_CHAR) {
//      startEmpty = 1;
//      emptyCharCount++;
//      wordCount = 0;
//    } else if (startEmpty) {
//
//      if (c == EMPTY_CHAR) {
//        emptyCharCount++;
//        wordCount = 0;
//      } else if (emptyCharCount == 1) {
//        passwordWifi[wordCount] = c;
//        wordCount++;
//      } else if (emptyCharCount == 2) {
//        id[wordCount] = c;
//        wordCount++;
//      } else if (emptyCharCount == 3) {
//        password[wordCount] = c;
//        wordCount++;
//      }
//    } else if (startGet == 0 && c == ',') {
//      commaCount++;
//    } else {
//      
//      if (commaCount == 1) {
//        // Verificar se ja pegou a aspas, pq não faz parte do ssid
//        if (startGet == 0) {
//          startGet = 1;
//        } else {
//          if (c == '"') {
//            startGet = 0;
//            wordCount = 0;
//          } else {
//            ssid[wordCount] = c;
//            wordCount++;
//          }
//        }
//        
//      } else if (commaCount == 3) {
//        // Verificar se ja pegou a aspas, pq não faz parte do ssid
//        if (startGet == 0) {
//          startGet = 1;
//        } else {
//          if (c == '"') {
//            startGet = 0;
//            wordCount = 0;
//          } else {
//            netMacAddress[wordCount] = c;
//            wordCount++;
//          }
//        }
//      }
//    }
//  }
//}

void getDataWifiConfig(char *command, char *ssid, char *netMacAddress, char *passwordWifi, char *id, char *password, serialPrintFunc *serialPrint) {
  int emptyCharCount = 0;

  if (DEBUG == 1) {
    serialPrint("getDataWifiConfig: ", 1);
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

