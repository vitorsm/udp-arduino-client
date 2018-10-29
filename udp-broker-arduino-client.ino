#include "constants.h"
#include "messageUtils.c"
#include "utils.c"
#include <SoftwareSerial.h>
#include "espUtils.c"
#include "accessPoint.c"
#include "processEspData.c"
#include "digitalControl.c"
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


char brokerIp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int connectionId = 0;
long lastRequestBroker = 0;
int lengthTextLcd = 0;


SoftwareSerial esp8266(8, 9);

void setup() {

  Serial.begin(9600);
  esp8266.begin(19200);
  
  moduleReset(sendData);
  //connectToWifi(sendData, "2.4Ghz Virtua 302", "3207473600");

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando setup");
  
  for (int i = 0; i < 8; i++) {
    delay(100);  
    Serial.print(".");
    lcd.setCursor(i, 1);
    lcd.print(".");
  }
  Serial.println(".");
    
  for (int i = 0; i < 8; i++) {
    delay(100);  
    Serial.print(".");
    lcd.setCursor(8 + i, 1);
    lcd.print(".");
  }
  Serial.println(".");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando setup");

  lcd.setCursor(0, 1);
  lcd.print("Configurando ESP");
  
  setMultipleConnections(sendData);
  enableShowRemoteIp(sendData);
  //setStationMode(sendData);
  
  //startServer(sendData);
  
  startAccessPointConfig(sendData);
  showLocalIpAddress(sendData);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando setup");
  
  for (int i = 0; i < 16; i++) {
    delay(300);  
    Serial.print(".");
    lcd.setCursor(i, 1);
    lcd.print(".");
  }

  lcd.clear();
  //startServer(sendData);

  //Serial.println("Digital Control Init");
  //initDigitalControl();
  lcd.setCursor(0,0);
  lcd.print(F("Terminou setup"));
  Serial.println(F("Terminou setup"));
  
}

int was = 0;

void loop() {
  
  String message = "";
  
  while (esp8266.available()) {
    message += (char) esp8266.read();
  }

  if (message != "") {
    if (DEBUG == 1){
      Serial.print(F("tamanho comando: "));
      Serial.println(message.length());
      Serial.println(message);
    }
    
    char strMessage[message.length() + 1];
    //message.toCharArray(strMessage, message.length() + 1);
    convertStringToChar(message, strMessage);

    if (DEBUG == 1) {
      Serial.print(F("Vai passar uma mensagem para ser processada: "));
      Serial.println(strMessage);
      Serial.println("---------------------------------------------------");
    }
    
    proccessReceivedData(sendData, strMessage, serialPrint, printConstantsMessage);

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

    if (DEBUG == 1) {
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
  Serial.print(F("vai converter: "));
  Serial.println(str);
  Serial.println(F("---------------------"));
  
  int strLength = str.length();
  
  for (int i = 0; i < strLength; i++) {
    charStr[i] = str.charAt(i);  
  }

  charStr[strLength] = 0;
}

void serialPrint(char *msg, int isPrintln) {
  if (isPrintln == 1) {
    Serial.println(msg);
  } else {
    Serial.print(msg);
  }
}

const int countMessages = 37;
const int countColumnMessages = 75;

const char MESSAGES[countMessages] [countColumnMessages] PROGMEM = {
  { "msg n encontrada" },
  { "Vai iniciar o processWifiConfig com: " }, //MESSAGE_INDEX_PROCESS_WIFI_CONFIG_1 1
  { "SSID: " }, //MESSAGE_INDEX_PROCESS_WIFI_CONFIG_SSID 2
  { "MACAddress: " }, //MESSAGE_INDEX_PROCESS_WIFI_CONFIG_MAC 3
  { "PassWifi: " }, //MESSAGE_INDEX_PROCESS_WIFI_CONFIG_PASS_W 4
  { "LoginToken: " }, //MESSAGE_INDEX_PROCESS_WIFI_CONFIG_LOGIN 5
  { "PassToken: " }, //MESSAGE_INDEX_PROCESS_WIFI_CONFIG_PASS_L 6
  { "Deu ruim, Clovis" }, //MESSAGE_INDEX_ERROR 7
  { "getDataWifiConfig: " }, //MESSAGE_INDEX_DATA_WIFI_CONFIG 8
  { "StartMessage: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_DATA_1 9
  { " | StartType: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_DATA_2 10
  { " | EndType: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_DATA_3 11
  { "Primeiro caracter mensagem: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_DATA_4 12
  { "Primeiro e ultimo caracter tipo: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_DATA_5 13
  { "Type AT message: " }, //MESSAGE_INDEX_MESSAGE_TYPE_AT 14
  { "Resposta: " }, //MESSAGE_INDEX_RESPONSE 15
  { "Mensagem de lista wifi: " }, //MESSAGE_INDEX_WIFI_LIST 16
  { "IP Address: " }, //MESSAGE_INDEX_IP_ADDRESS 17
  { "Port: " }, //MESSAGE_INDEX_PORT 18
  { "Pegou essa mensagem: " }, //MESSAGE_INDEX_GET_THIS 19
  { "Tipo de mensagem AT nao configurado" }, //MESSAGE_INDEX_AT_TYPE_UNDEFINED 20
  { "chegou no receivedMessage: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_MESSAGE_1 21
  { "Vai processar a seguinte mensagem: " }, //MESSAGE_INDEX_PROCESS_RECEIVED_MESSAGE_2 22
  { "Message Type: " }, //MESSAGE_INDEX_MESSAGE_TYPE 23
  { "Topic: "}, //MESSAGE_INDEX_TOPIC 24
  { "Mensagem de dados recebida: " }, //MESSAGE_INDEX_RECEIVED_MESSAGE 25
  { "Value: "}, //MESSAGE_INDEX_VALUE 26
  { "Topic: " }, //MESSAGE_INDEX_RECEIVED_TOPIC 27
  { "Mensagem de publish recebida: " }, //MESSAGE_INDEX_PUBLISH_MESSAGE_RECEIVED 28
  { "Recebeu uma mensagem de subscribe" }, //MESSAGE_INDEX_SUBSCRIBE_MESSAGE_RECEIVED 29
  { "Recebeu uma mensagem de keep alieve" }, //MESSAGE_INDEX_KEEP_ALIVE_MESSAGE_RECEIVED 30
  { "Recebeu uma mensagem de hello" }, //MESSAGE_INDEX_HELLO_MESSAGE_RECEIVED 31
  { "Recebeu uma mensagem de network" }, //MESSAGE_INDEX_NETWORK_MESSAGE_RECEIVED 32
  { "Recebeu uma mensagem de erro" }, //MESSAGE_INDEX_ERROR_MESSAGE_RECEIVED 33
  { "Recebeu uma mensagem sem um tipo válido" }, //MESSAGE_INDEX_MESSAGE_TYPE_UNDEFINED 34
  { "Ainda não tem IP" }, //MESSAGE_INDEX_IP_NOT_FOUND 35
  { "Vai pegar o IP dessa msg: " } //MESSAGE_INDEX_MESSAGE_TO_IP 36
};

void printLCDText(char *text, int keepLastText) {
   if (keepLastText != 1) {
    lengthTextLcd = 0;
    lcd.clear();
   }

  int xPos = 0;
  int yPos = 0;
  int strLen = strlen(text);

  if (strLen + lengthTextLcd >= 32 && lengthTextLcd > 0) {
    lengthTextLcd = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lengthTextLcd = strLen;
  } else {
    if (lengthTextLcd > 16) {
      yPos = 1;
      xPos = lengthTextLcd - 16;
    } else if (strLen + lengthTextLcd > 16) {
      yPos = 1;
      xPos = 0;
    }else {
      yPos = 0;
      xPos = lengthTextLcd;
    }
    lengthTextLcd += strLen;
  }
  
  lcd.setCursor(xPos, yPos);
  lcd.print(text);
}

void printLCD(int messageIndex, int keepLastText) {

 char text[countColumnMessages];
  if (messageIndex >= countColumnMessages) {
    memcpy_P(&text,&MESSAGES[0],sizeof text);  
  } else {
    memcpy_P(&text,&MESSAGES[messageIndex],sizeof text);
  }
  printLCDText(text, keepLastText);
}

void printConstantsMessage(int messageIndex, int isPrintln) {

  char text[countColumnMessages];
  if (messageIndex >= countColumnMessages) {
    memcpy_P(&text,&MESSAGES[0],sizeof text);  
  } else {
    memcpy_P(&text,&MESSAGES[messageIndex],sizeof text);
  }
    
  if (isPrintln == 1) {
    Serial.println(text);
  } else {
    Serial.print(text);
  }

//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print(text);
}
