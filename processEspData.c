/*
 * processEspData.c
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#include "processEspData.h"

void proccessReceivedData(char *data) {

  // TO DO: Verificar se a posicao realmente pode ser usada para pegar o tipo
  char type[4 + 1];
  subvectorBytes(data, 2, 6, type)
  
  if (DEBUG == 1) {
    Serial.print("type: ");
    Serial.print(type);
  }

  if (compareBytes("+CWLAP", type, 4) == 1) {
    
    Serial.print("Mensagem de lista wifi: ");
    Serial.println(messageChar);
    
    proccessResponseListAPs(messageChar);
    
  } else if (compareBytes("+IPD", type, 4)) {
    
  }
  
  if (type == "+CWLAP") {
    
    proccessResponseListAPs(messageChar);

    
  } else if (type == "+IPD") {
    int connectionId = 0;
    int messageLength = 0;
    char ipAddress[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int port = 0;
    
    getDataFromReceivedData(data, &connectionId, &messageLength, ipAddress, &port);

    if (DEBUG == 1) {
      Serial.print("metada: \r\nconnectionId: ");
      Serial.print(connectionId);
      Serial.print(" | messageLength: ");
      Serial.print(messageLength);
      Serial.print(" | ipAddress: ");
      Serial.print(ipAddress);
      Serial.print(" | port: ");
      Serial.println(port);
    }
    
    int startsMessage = getStartsMessage(data);

    if (startsMessage >= 0) {
      
      String message = data.substring(startsMessage, startsMessage + messageLength);
      
      proccessReceivedMessage(message, ipAddress, port);
      
    } else if (DEBUG == 1) {
      Serial.println("Não foi possível identificar o início da mensagem. Caracter ':' não encontrado");
    }
  }  
}

