/*
 * processEspData.c
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#include "processEspData.h"

void proccessReceivedData(sendDataFunc *sendData, char *data, serialPrintFunc *serialPrint, printConstantsMessages *printConstants, printLCDFunc *printLCD) {

  int startMessage = 0;
  int startType = 0;
  int endsType = 0;
  
  getMessageBounds(data, &startType, &endsType, &startMessage, serialPrint, printConstants);
  
  //int startMessage = getStartsMessage(data);
  //printLCD(MESSAGE_INDEX_RECEIVED_MESSAGE, 0);
  
  if (DEBUG == 1) {
    char strStartMessage[4];
    char strStartType[4];
    char strEndsType[4];

    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_MESSAGE_2, 0);

    convertIntToBytes(startMessage, strStartMessage, 4);
    convertIntToBytes(startType, strStartType, 4);
    convertIntToBytes(endsType, strEndsType, 4);

    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_DATA_1, 0);
    serialPrint(strStartMessage, 0);
    
    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_DATA_2, 0);
    serialPrint(strStartType, 0);

    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_DATA_3, 0);
    serialPrint(strEndsType, 1);

    char messageTest[] = {data[startMessage], data[startMessage + 1], 0};
    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_DATA_4, 0);
    serialPrint(messageTest, 1);

    char typeTest[] = {data[startType], data[endsType], 0};
    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_DATA_5, 0);
    serialPrint(typeTest, 1);
    
  }
  
  if (startMessage >= 0 && startType >= 0 && endsType >= 0) {
    char type[MAX_SIZE_AT_TYPE_MESSAGE];
    clearString(brokerIpAddress, MAX_SIZE_AT_TYPE_MESSAGE);
    
    subvectorBytes(data, startType, endsType, type);
   
    if (DEBUG == 1) {
      printConstants(MESSAGE_INDEX_MESSAGE_TYPE_AT, 0);
      serialPrint(type, 1);
      
      int resp = compareBytes("+IPD", type, 4);
      char strResp[4];
      convertIntToBytes(resp, strResp, 4);
      
      printConstants(MESSAGE_INDEX_RESPONSE, 0);
      serialPrint(strResp, 1);
    }
    
    if (compareBytes("+CWLAP1", type, 5) == 1) {
      printConstants(MESSAGE_INDEX_WIFI_LIST, 1);
      processResponseListAPs(sendData, data);
    } else if (compareBytes("+IPD", type, 4) == 1) {
      
      int connectionId = 0;
      int messageLength = 0;
      char ipAddress[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int port = 0;
      
      getDataFromReceivedData(data, &connectionId, &messageLength, ipAddress, &port, serialPrint, printConstants);

      if (DEBUG == 1) {
          //printar os dados recebidos
          printConstants(MESSAGE_INDEX_IP_ADDRESS, 0);
          serialPrint(ipAddress, 1);
          char strPort[7];
          clearString(strPort, 7);
          convertIntToBytes(port, strPort, 7);
          
          printConstants(MESSAGE_INDEX_PORT, 0);
          serialPrint(strPort, 1);
      }

      if (startMessage >= 0) {
        char message[messageLength + 1];
        subvectorBytes(data, startMessage, startMessage + messageLength, message);
        message[messageLength] = 0;

        if (DEBUG == 1) {
          printConstants(MESSAGE_INDEX_GET_THIS, 1);
          serialPrint(message, 1);
        }

        proccessReceivedMessage(sendData, message, ipAddress, port, serialPrint, printConstants, printLCD);
      }

    } else if (DEBUG == 1) {
      printConstants(MESSAGE_INDEX_AT_TYPE_UNDEFINED, 1);
    }
  }
    
  
}

void getDataFromReceivedData(char *data, int *connectionId, int *messageLength, char *ipAddress, int *port, serialPrintFunc *serialPrint, printConstantsMessages *printConstants) {

  int dataLength = strlen(data);

  int commaCount = 0;
  
  int countStr = 0;
  char messageLengthStr[] = {0, 0, 0, 0, 0, 0, 0};
  char connectionIdStr[] = {0, 0};
  char portStr[] = {0, 0, 0, 0, 0, 0, 0, 0};

  if (DEBUG == 1) {
    printConstants(MESSAGE_INDEX_MESSAGE_TO_IP, 0);
    serialPrint(data, 1);  
  }
  
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

  char portStrTeste[] = {0, 0, 0, 0, 0, 0, 0, 0};
  convertIntToBytes(*port, portStrTeste, 8);
  serialPrint(portStrTeste, 0);
}

void proccessReceivedMessage(sendDataFunc *sendData, char *message, char *originIp, int originPort, serialPrintFunc *serialPrint, printConstantsMessages *printConstants, printLCDFunc *printLCD) {

  serialPrint(message, 1);
  
  char strMessageType[2];
  strMessageType[1] = 0;
  serialPrint(message, 1);
  subvectorBytes(message, 0, 1, strMessageType);
  serialPrint(message, 1);
  
  printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_MESSAGE_1, 0);
  serialPrint(strMessageType, 1);

  serialPrint(message, 1);
  
  int messageType = convertBytesToInt(strMessageType);

  serialPrint(message, 1);
  char topic[MESSAGE_BODY_LENGTH - MESSAGE_TOKEN_LENGTH + 1]; // case MESSAGE_TYPE_DATA:


  float value = 12; //case MESSAGE_TYPE_DATA:
  char strValue[15]; //case MESSAGE_TYPE_DATA:
  
  serialPrint(message, 1);
  if (DEBUG == 1) {
    printConstants(MESSAGE_INDEX_PROCESS_RECEIVED_MESSAGE_2, 1);
    serialPrint(message, 1);
    
    printConstants(MESSAGE_INDEX_MESSAGE_TYPE, 0);
    serialPrint(strMessageType, 1);
    printConstants(MESSAGE_INDEX_TOPIC, 0);
    serialPrint(topic, 1);
  }
  
  switch (messageType) {
    case MESSAGE_TYPE_UPDATE_PARAM:
      if (DEBUG == 1) {
        //concatString(strLog, "\r\recebeu uma mensagem de atualizacao de parametros", strLog);
        serialPrint("Recebeu uma mensagem de atualizacao de parametros", 1);
        serialPrint(message, 1);
      }

      clearString(brokerIpAddress, 16);
      concatString(brokerIpAddress, originIp, brokerIpAddress);
//      brokerIpAddressFound = 1;

      setParams(token, message);

//      token, conditions, pinsId, typeIO, sampleTime, kp, ki, kd, setPoint, message

      if (DEBUG == 1) {
        serialPrint(token, 1);
        printConstants(MESSAGE_INDEX_PINS_ID, 1);
        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char strPinId[MESSAGE_INPUT_ID_LENGTH + 1];
           convertIntToBytes(pinsId[i], strPinId, MESSAGE_INPUT_ID_LENGTH);
           strPinId[MESSAGE_INPUT_ID_LENGTH] = 0;
           serialPrint(strPinId, 1);
        }

        printConstants(MESSAGE_INDEX_TYPES_IO, 1);
        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char typeIO[] = { typeIO[i], 0 };
           serialPrint(typeIO, 1);
        }

        printConstants(MESSAGE_INDEX_SAMPLES_TIME, 1);
        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char strSampleTime[MESSAGE_SAMPLE_LENGTH + 1];
           convertIntToBytes(sampleTime[i], strSampleTime, MESSAGE_SAMPLE_LENGTH);
           strSampleTime[MESSAGE_SAMPLE_LENGTH] = 0;
           serialPrint(strSampleTime, 1);
        }

        printConstants(MESSAGE_INDEX_K_PARAM, 1);
        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char strKp[MESSAGE_K_LENGTH + 1];
           convertFloatToBytes(kp[i], strKp, MESSAGE_K_LENGTH);
           strKp[MESSAGE_K_LENGTH] = 0;
           serialPrint(strKp, 1);
        }

        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char strKi[MESSAGE_K_LENGTH + 1];
           convertFloatToBytes(ki[i], strKi, MESSAGE_K_LENGTH);
           strKi[MESSAGE_K_LENGTH] = 0;
           serialPrint(strKi, 1);
        }
        
        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char strKd[MESSAGE_K_LENGTH + 1];
           convertFloatToBytes(kd[i], strKd, MESSAGE_K_LENGTH);
           strKd[MESSAGE_K_LENGTH] = 0;
           serialPrint(strKd, 1);
        }        

        printConstants(MESSAGE_INDEX_SETPOINT, 1);
        for (int i = 0; i < PORTS_AMOUNT; i++) {
           char strSample[MESSAGE_SAMPLE_LENGTH + 1];
           convertIntToBytes(kd[i], strSample, MESSAGE_SAMPLE_LENGTH);
           strSample[MESSAGE_SAMPLE_LENGTH] = 0;
           serialPrint(strSample, 1);
        }
        
        for (int i = 0; i < PORTS_AMOUNT; i++) {
          struct Condition *c = conditions[i];
          while (c->inputId >= 0) {
            printConstants(MESSAGE_INDEX_CONDITION, 1);
            
            char strInputId[MESSAGE_INPUT_ID_LENGTH + 1];
            convertIntToBytes(c->inputId, strInputId, MESSAGE_INPUT_ID_LENGTH);
            strInputId[MESSAGE_INPUT_ID_LENGTH] = 0;
            serialPrint(strInputId, 1);
            
            char valueStr[MESSAGE_VALUE_LENGTH + 1];
            convertFloatToBytes(c->value, valueStr, MESSAGE_VALUE_LENGTH);
            valueStr[MESSAGE_VALUE_LENGTH] = 0;
            serialPrint(valueStr, 1);
            
            c = c->next;
          }
        }
      }
      
      lastTimeChange[0] = 1;
      
      break;
    case MESSAGE_TYPE_DATA:
      proccessDataMessage(message, topic, &value, strValue);
      topic[MESSAGE_TOKEN_LENGTH] = 0;

      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_MESSAGE_TYPE, 1);
        printConstants(MESSAGE_INDEX_VALUE, 0);
        serialPrint(strValue, 1);

        printConstants(MESSAGE_INDEX_RECEIVED_TOPIC, 0);
        serialPrint(topic, 1);
      }
      
      break;
    case MESSAGE_TYPE_PUBLISH:
      if (DEBUG == 1) {
        //concatString(strLog, "\r\recebeu uma mensagem de publish", strLog);
        printConstants(MESSAGE_INDEX_PUBLISH_MESSAGE_RECEIVED, 1);
      }
      break;
    case MESSAGE_TYPE_SUBSCRIBE:
      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_SUBSCRIBE_MESSAGE_RECEIVED, 1);
      }
      break;
    case MESSAGE_TYPE_KEEP_ALIVE:
      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_KEEP_ALIVE_MESSAGE_RECEIVED, 1);
      }
      break;
    case MESSAGE_TYPE_HELLO:
      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_HELLO_MESSAGE_RECEIVED, 1);
      }
      break;
    case MESSAGE_TYPE_NETWORKS:
      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_NETWORK_MESSAGE_RECEIVED, 1);
        processWifiConfig(sendData, message, serialPrint, printConstants, printLCD);
      }
      break;
    case MESSAGE_TYPE_CREDENTIALS_REQUEST:
      if (DEBUG == 1) {
        serialPrint("recebeu uma mensagem de requisicao de credenciais", 1);
      }
      break;
    case MESSAGE_TYPE_ERROR:
      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_ERROR_MESSAGE_RECEIVED, 1);
      }
      break;
    default:
      if (DEBUG == 1) {
        printConstants(MESSAGE_INDEX_MESSAGE_TYPE_UNDEFINED, 1);
      }
      break;
  }

}

void getBrokerIpAddress(char *ipAddress, serialPrintFunc *serialPrint, printConstantsMessages *printConstants) {
  if (brokerIpAddressFound == 1) {
    clearString(ipAddress, 16);
    concatString("", brokerIpAddress, ipAddress);
  } else if (DEBUG == 1) {
    printConstants(MESSAGE_INDEX_IP_NOT_FOUND, 1);
  }
}


void getMessageBounds(char *message, int *startType, int *endsType, int *startMessage, serialPrintFunc *serialPrint, printConstantsMessages *printConstants) {

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

