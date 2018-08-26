/*
 * processEspData.c
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#include "processEspData.h"

void proccessReceivedData(sendDataFunc *sendData, char *data, char *strLog) {

  int startMessage = 0;
  int startType = 0;
  int endsType = 0;
  
  getMessageBounds(data, &startType, &endsType, &startMessage);
  
  //int startMessage = getStartsMessage(data);

  if (DEBUG == 1) {
    char strStartMessage[4];
    char strStartType[4];
    char strEndsType[4];

    convertIntToBytes(startMessage, strStartMessage, 4);
    convertIntToBytes(startType, strStartType, 4);
    convertIntToBytes(endsType, strEndsType, 4);

    concatString(strLog, "\r\nstartMessage", strLog);
    concatString(strLog, strStartMessage, strLog);
    concatString(strLog, "\r\nstartType", strLog);
    concatString(strLog, strStartType, strLog);
    concatString(strLog, "\r\nendsType", strLog);
    concatString(strLog, strEndsType, strLog);

    concatString(strLog, "\r\nmensagem: \r\n", strLog);
    concatString(strLog, data, strLog);

//    free(strStartMessage);
//    free(strStartType);
//    free(strEndsType);
  }
  
//  if (startMessage >= 0 && startType >= 0 && endsType >= 0) {
//    char type[startType + endsType + 1];
//    subvectorBytes(data, startType, endsType, type);
//    type[startMessage] = 0;
//    
//    if (DEBUG == 1) {
//      concatString(strLog, "\r\ntype: ", strLog);
//      concatString(strLog, type, strLog);
//      concatString(strLog, "\r\n", strLog);
//    }
//    
//    if (compareBytes("+CWLAP1", type, 5) == 1) {
//      concatString(strLog, "\r\nMensagem de lista wifi: ", strLog);
//      
//      //proccessResponseListAPs(data);
//      processResponseListAPs(sendData, data);
//      
//    } else if (compareBytes("+IPD", type, 4)) {
//      int connectionId = 0;
//      int messageLength = 0;
//      char ipAddress[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//      int port = 0;
//  
//      getDataFromReceivedData(data, &connectionId, &messageLength, ipAddress, &port);
//  
//      if (DEBUG == 1) {
//        concatString(strLog, "metada: \r\nconnectionId: ", strLog);
//        concatString(strLog, connectionId, strLog);
//        concatString(strLog, " | messageLength: ", strLog);
//        concatString(strLog, messageLength, strLog);
//        concatString(strLog, " | ipAddress: ", strLog);
//        concatString(strLog, ipAddress, strLog);
//        concatString(strLog, " | port: ", strLog);
//        concatString(strLog, port, strLog);
//        concatString(strLog, "\r\n", strLog);
//      }
//  
//      //int startsMessage = getStartsMessage(data);
//  
//      if (startMessage >= 0) {
//        char message[startMessage + messageLength + 1];
//        subvectorBytes(data, startMessage, startMessage + messageLength, message);
//        message[startMessage + messageLength] = 0;
//  
//        proccessReceivedMessage(sendData, message, ipAddress, port, strLog);
//      }
//    } else if (DEBUG == 1) {
//      char test[3];
//      convertIntToBytes(startMessage, test, 3);
//      concatString(strLog, "Tipo indefinido: \r\n", strLog);
//      concatString(strLog, type, strLog);
//      concatString(strLog, "<<<-\r\n", strLog);
//      concatString(strLog, test, strLog);
//      concatString(strLog, "<<<-\r\n", strLog);
//      
//    }
//  }
    
  
}

void getDataFromReceivedData(char *data, int *connectionId, int *messageLength, char *ipAddress, int *port) {

  int dataLength = strlen(data);

  int commaCount = 0;
  
  int countStr = 0;
  char messageLengthStr[] = {0, 0, 0, 0, 0, 0, 0};
  char connectionIdStr[] = {0, 0};
  char portStr[] = {0, 0, 0, 0, 0, 0, 0, 0};
  
  for (int i = 0; i < dataLength; i++) {
    char c = data[i];

    if (c == ':') {
      break;
    } else if (c == ',') {
      commaCount++;
      countStr = 0;
    } else {

      if (commaCount == 1) {
        connectionIdStr[0] = c;
      } else if (commaCount == 2) {
        messageLengthStr[countStr] = c;
      } else if (commaCount == 3) {
        ipAddress[countStr] = c;
      } else if (commaCount == 4) {
        portStr[countStr] = c;
      }

      countStr++;
    }
  }
  
  *connectionId = convertBytesToInt(connectionIdStr);
  *messageLength = convertBytesToInt(messageLengthStr);
  *port = convertBytesToInt(portStr);

//  free(messageLengthStr);
//  free(connectionIdStr);
//  free(portStr);
}

void proccessReceivedMessage(sendDataFunc *sendData, char *message, char *originIp, int originPort, char *strLog) {

  //clearString(strLog, LOG_SIZE);
  
  char strMessageType[2];
  strMessageType[1] = 0;
  subvectorBytes(message, 0, 1, strMessageType);

  int messageType = convertBytesToInt(strMessageType);

  if (DEBUG == 1) {
    concatString(strLog, "\r\n", strLog);
    concatString(strLog, strMessageType, strLog);
    concatString(strLog, "vai processar: ", strLog);
    concatString(strLog, message, strLog);
    concatString(strLog, "\r\nMessage type: ", strLog);
    concatString(strLog, messageType, strLog);
  }

  char topic[MESSAGE_BODY_LENGTH - MESSAGE_TOKEN_LENGTH + 1]; // case MESSAGE_TYPE_DATA:
  
  
  float value = 12; //case MESSAGE_TYPE_DATA:
  char strValue[15]; //case MESSAGE_TYPE_DATA:
  switch (messageType) {
    case MESSAGE_TYPE_UPDATE_PARAM:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de atualizacao de parametros", strLog);
      }
      clearString(brokerIpAddress, 16);
      concatString(brokerIpAddress, originIp, brokerIpAddress);
      brokerIpAddressFound = 1;
      break;
    case MESSAGE_TYPE_DATA:
      proccessDataMessage(message, topic, &value, strValue);
      topic[MESSAGE_TOKEN_LENGTH] = 0;

      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de dados:\r\n\t", strLog);
        concatString(strLog, "value: ", strLog);
        concatString(strLog, strValue, strLog);
        concatString(strLog, "\r\n\ttopic: ", strLog);
        concatString(strLog, topic, strLog);
      }
      
      break;
    case MESSAGE_TYPE_PUBLISH:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de publish", strLog);
      }
      break;
    case MESSAGE_TYPE_SUBSCRIBE:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de subscribe", strLog);
      }
      break;
    case MESSAGE_TYPE_KEEP_ALIVE:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de keep alive", strLog);
      }
      break;
    case MESSAGE_TYPE_HELLO:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de hello", strLog);
      }
      break;
    case MESSAGE_TYPE_NETWORKS:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de network", strLog);
        processWifiConfig(sendData, message);
      }
      break;
    case MESSAGE_TYPE_CREDENTIALS_REQUEST:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de requisicao de credenciais", strLog);
      }
      break;
    case MESSAGE_TYPE_ERROR:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem de erro", strLog);
      }
      break;
    default:
      if (DEBUG == 1) {
        concatString(strLog, "\r\recebeu uma mensagem sem um tipo válido", strLog);
      }
      break;
  }

//  free(strMessageType);
//  free(strValue);
//  free(topic);
}

void getBrokerIpAddress(char *ipAddress) {
  if (brokerIpAddressFound == 1) {
    clearString(ipAddress, 16);
    concatString("", brokerIpAddress, ipAddress);
  }
}
//
//int getStartsMessage(char *message) {
//
//  int strLength = strlen(message);
//
//  int indexReturn = -1;
//  for (int i = 0; i < strLength; i++) {
//    char c = message[i];
//    if (c == ':') {
//      indexReturn =  i + 1;
//    }
//  }
//
//  return indexReturn;
//}

void getMessageBounds(char *message, int *startType, int *endsType, int *startMessage) {

  int strLength = strlen(message);

  int _startMessage = -1;
  int i = 0;
  for (i = 0; i < strLength; i++) {
    char c = message[i];
    if (c == ':') {
      _startMessage =  i + 1;
    }
  }

  i = _startMessage - 2;
  int commaIndex = -1;
  int plusIndex = -1;
  
  if (_startMessage >= 0) {
    
    while (i >= 0) {
      char c = message[i];
      if (c == ',') {
        commaIndex = i;
      } else if (c == '+') {
        plusIndex = i;
        break;
      }
      i--;
    }
    
  }



  *(startType) = plusIndex;
  *(endsType) = commaIndex;
  *(startMessage) = _startMessage;
    
}

