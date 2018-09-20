#include "constants.h"
#include "messageUtils.c"
#include "utils.c"
#include <SoftwareSerial.h>
#include "espUtils.c"
#include "accessPoint.c"
#include "processEspData.c"
#include "digitalControl.c"

char brokerIp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int connectionId = 0;
long lastRequestBroker = 0;



SoftwareSerial esp8266(2, 3);

void setup() {

  Serial.begin(9600);
  esp8266.begin(19200);
  
  moduleReset(sendData);
  //connectToWifi(sendData, "2.4Ghz Virtua 302", "3207473600");
  
  for (int i = 0; i < 20; i++) {
    delay(100);  
    Serial.print(".");
  }
  Serial.println(".");
    
  for (int i = 0; i < 20; i++) {
    delay(100);  
    Serial.print(".");
  }
  Serial.println(".");
  
  setMultipleConnections(sendData);
  enableShowRemoteIp(sendData);
  //setStationMode(sendData);
  
  //startServer(sendData);
  
  startAccessPointConfig(sendData);
  showLocalIpAddress(sendData);

  for (int i = 0; i < 100; i++) {
    delay(100);  
    Serial.print(".");
  }

  //startServer(sendData);

  //Serial.println("Digital Control Init");
  //initDigitalControl();
  
  Serial.println("Terminou setup");
}

int was = 0;

void loop() {
  
  String message = "";
  
  while (esp8266.available()) {
    message += (char) esp8266.read();
  }

  if (message != "") {
    Serial.print("tamanho comando: ");
    Serial.println(message.length());
    
    char strMessage[message.length() + 1];
    //message.toCharArray(strMessage, message.length() + 1);
    convertStringToChar(message, strMessage);

    Serial.print("mensagem para funcaoi: ");
    Serial.println(strMessage);
    
//    if (DEBUG == 1) {
      Serial.print("mensagem para funcaoi: ");
//      Serial.println(strMessage);
//    }
    
    proccessReceivedData(sendData, strMessage, serialPrintln);

//    free(strMessage);
  }

//  if (was == 0) {
//    was = 1;
//    listAPs(sendData);
//  }
}

int sendData(char *command, const int timeout, int debug, int maxAttempts) {
  
  int okCommand = 0;
  int attempts = 0;

  String response;
  while (okCommand != 1 && attempts <= maxAttempts) {
    response = "";

    if (okCommand != 2) {
      attempts++;
      esp8266.print(command);
      esp8266.print("\r\n");
    }

    int ok = 0;
    long int time = millis();
    while ( (time + timeout) > millis()) {
  
      while (esp8266.available()) {
        char c = esp8266.read(); // read the next character.
        response += c;
        //ok = 1;
      }

//      if ((response.indexOf("+") > 0 || response.indexOf("OK") > 0) && response.length() > 1) {
//        ok = 1;
//        break;
//      }
//        if (response.indexOf("\r\n") > 0) {
//          ok = 1;
//          break;          
//        }
      //if (ok == 1) break;
    }

    if (debug == 1) {
      Serial.print(response);
    }
  
    if (response.indexOf("OK") > 0 || response.indexOf("busy")) {
      okCommand = 1;
//    } else if(response.indexOf("busy") > 0) {
//      okCommand = 2;
    } else {
      okCommand = 0;
    }
  }
  
  return okCommand;
}


void prepareMessage(char *originalMessage, char *preparedMessage) {

  for (int i = 0; i < MESSAGE_LENGTH; i++) {
    preparedMessage[i] = originalMessage[i];
  }

  preparedMessage[MESSAGE_LENGTH] = '>';
}

void convertStringToChar(String str, char *charStr) {
  Serial.print("vai converter: ");
  Serial.println(str);
  Serial.println("---------------------");
  
  int strLength = str.length();
  
  for (int i = 0; i < strLength; i++) {
    charStr[i] = str.charAt(i);  
  }

  charStr[strLength] = 0;
}

void serialPrintln(char *msg) {
  Serial.println(msg);
}

void serialPrint(char *msg) {
  Serial.print(msg);
}


