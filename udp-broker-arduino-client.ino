#include "constants.h"
#include "messageUtils.c"
#include "utils.c"
#include <SoftwareSerial.h>
#include "espUtils.c"

#define DEBUG 1

String brokerIpAddress = "";

int connectionId = 0;
long lastRequestBroker = 0;

SoftwareSerial esp8266(2, 3);

void setup() {

  Serial.begin(9600);
  esp8266.begin(19200);
  
  moduleReset(sendData);
  connectToWifi(sendData, "2.4Ghz Virtua 302", "3207473600");
  
  for (int i = 0; i < 60; i++) {
    delay(100);  
    Serial.print(".");
  }
  Serial.println(".");
  
  setStationMode(sendData);
  
  for (int i = 0; i < 10; i++) {
    delay(100);  
    Serial.print(".");
  }
  Serial.println(".");
  
  setMultipleConnections(sendData);
  enableShowRemoteIp(sendData);
  
  startServer(sendData);
  
  Serial.println("Terminou setup");
}

void loop() {
 
  String message = "";
  
  while (esp8266.available()) {
    message += (char) esp8266.read();
  }

  if (message != "") {
        
    proccessReceivedData(message);
    
  }
  
  if (brokerIpAddress == "") {
    if (millis() - lastRequestBroker >= TIME_REQUEST_HELLO) {
      sendHelloMessage(sendData);
      lastRequestBroker = millis();
    }
  }
  
}

void proccessReceivedData(String data) {
  
  String type = data.substring(2, 6);
  
  if (DEBUG) {
    Serial.println("type: " + type);
  }

  if (type == "+IPD") {
    int connectionId = 0;
    int messageLength = 0;
    char ipAddress[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int port = 0;
    
    getDataFromReceivedData(data, &connectionId, &messageLength, ipAddress, &port);

    if (DEBUG) {
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
      
    } else if (DEBUG) {
      Serial.println("Não foi possível identificar o início da mensagem. Caracter ':' não encontrado");
    }
  }  
}

void proccessReceivedMessage(String message, String originIp, int originPort) {
  
  char messageProccess[message.length() + 1];
  convertStringToChar(message, messageProccess);
      
  char strMessageType[2];
  strMessageType[1] = 0;
  subvectorBytes(messageProccess, 0, 1, strMessageType);

  Serial.println("");
  Serial.println(strMessageType);
  
  int messageType = convertBytesToInt(strMessageType);

  Serial.print("vai processar: ");
  Serial.println(messageProccess);
  Serial.print("Message type: ");
  Serial.println(messageType);

  float value = 12;
  switch (messageType) {
    case MESSAGE_TYPE_UPDATE_PARAM:
      Serial.println("recebeu uma mensagem de atualizacao de parametros");
      brokerIpAddress = originIp;
      break;
    case MESSAGE_TYPE_DATA:
      char topic[MESSAGE_BODY_LENGTH - MESSAGE_TOKEN_LENGTH + 1];
      proccessDataMessage(messageProccess, topic, &value);
      topic[MESSAGE_TOKEN_LENGTH] = 0;
      
      Serial.println("recebeu uma mensagem de dados:");
      Serial.print("value: ");
      Serial.println(value);
      Serial.print("topic: ");
      Serial.println(topic);
      
      break;
    case MESSAGE_TYPE_PUBLISH:
      break;
    case MESSAGE_TYPE_SUBSCRIBE:
      break;
    case MESSAGE_TYPE_KEEP_ALIVE:
      break;
    case MESSAGE_TYPE_HELLO:
      break;
    default:
      break;
  }
}

void sendMessage(char *message, String ipAddress, int port) {

  String cipSend = "AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend += MESSAGE_LENGTH;
  cipSend += ",\"";
  cipSend += ipAddress;
  cipSend += "\",";
  cipSend += port;

  char charCipSend[cipSend.length() + 1];
  
  convertStringToChar(cipSend, charCipSend);
  
  sendData(charCipSend, 1000, DEBUG);
  
  char preparedMessage[MESSAGE_LENGTH + 1];
  prepareMessage(message, preparedMessage);
  
  sendData(message, 1000, DEBUG);

  // Verificar a necessidade disso
  String closeCommand = "AT+CIPCLOSE=";
  closeCommand += connectionId;

  //sendData(closeCommand, 3000, DEBUG);
}

int sendData(char *command, const int timeout, int debug) {
  
  String response = "";
  esp8266.print(command);
  esp8266.print("\r\n");

  int ok = 0;
  long int time = millis();
  while ( (time + timeout) > millis()) {

    while (esp8266.available()) {
      char c = esp8266.read(); // read the next character.
      response += c;
      ok = 1;
    }

    if (ok == 1) break;

  }

  if (debug) {
    Serial.print(response);
  }

  if (response.indexOf("OK") > 0) {
    return 1;
  } else {
    return 0;
  }
  
}


int getStartsMessage(String message) {

  int strLength = message.length();
  
  for (int i = 0; i < strLength; i++) {
    char c = message.charAt(i);
    if (c == ':') {
      return i + 1;
    }
  }

  return -1;
}

void convertStringToChar(String str, char *strChar) {

  int strLength = str.length();
  Serial.println("isso q vai converter: ");
  for (int i = 0; i < strLength; i++) {
    strChar[i] = str.charAt(i);
    Serial.print(str.charAt(i));
  }
  strChar[strLength] = 0;
  
  Serial.println("");
}

void getDataFromReceivedData(String data, int *connectionId, int *messageLength, char *ipAddress, int *port) {

  int dataLength = data.length();

  int commaCount = 0;
  
  int countStr = 0;
  char messageLengthStr[] = {0, 0, 0, 0, 0, 0, 0};
  char connectionIdStr[] = {0, 0};
  char portStr[] = {0, 0, 0, 0, 0, 0, 0, 0};
  
  for (int i = 0; i < dataLength; i++) {
    char c = data.charAt(i);

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

  if (DEBUG) {
    Serial.print("ConnectionID: ");
    Serial.println(connectionIdStr);
    Serial.print("Message length: ");
    Serial.println(messageLengthStr);
    Serial.print("Port: ");
    Serial.println(portStr);
  } 
  
  *connectionId = convertBytesToInt(connectionIdStr);
  *messageLength = convertBytesToInt(messageLengthStr);
  *port = convertBytesToInt(portStr);
}

void prepareMessage(char *originalMessage, char *preparedMessage) {

  for (int i = 0; i < MESSAGE_LENGTH; i++) {
    preparedMessage[i] = originalMessage[i];
  }

  preparedMessage[MESSAGE_LENGTH] = '>';
}

