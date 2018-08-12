#include "constants.h"
#include "messageUtils.c"
#include "utils.h"
#include <SoftwareSerial.h>

#define DEBUG true

char *assetId = "TEST_ID";
char *password = "pass";
int connectionId = 0;

SoftwareSerial esp8266(2, 3);

void setup() {

  Serial.begin(115200);
  esp8266.begin(115200);
  // reseta o modulo
  sendData("AT+RST", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=Intcode,julioviado", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSR", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1", 1000, DEBUG);
  String comma = ",";
//  String command = "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\"," + SERVER_PORT + comma + CLIENT_PORT + ",2";
  
  String command = "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",4410,4411,2";

//  String command = "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\"," + SERVER_PORT + ',' + CLIENT_PORT + ',' + '2';
  sendData(command, 1000, DEBUG);
}

void loop() {
  // put your main code here, to run repeatedly:

  String message = "";
  if (esp8266.available()) {
    Serial.println("verifica");
    if (esp8266.find("+")) {
      Serial.println("recebeu alguma coisa");
    }
      
  }
  
}

void proccessReceivedMessage(char *message) {

  char strMessageType[1];
  subvectorBytes(message, 0, 1, strMessageType);

  int messageType = convertBytesToInt(strMessageType);

  switch (messageType) {
    case MESSAGE_TYPE_UPDATE_PARAM:
      Serial.println("recebeu uma mensagem de atualizacao de parametros");
      break;
    case MESSAGE_TYPE_DATA:
      Serial.println("recebeu uma mensagem de dados");
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

void sendMessage(String message, String ipAddress, int port) {

  String cipSend = "AT+CIPSEND=" + connectionId;
  cipSend += "," + MESSAGE_LENGTH;
  cipSend += "," + ipAddress + "," + port;

  sendData(cipSend, 1000, DEBUG);
  sendData(message, 1000, DEBUG);

  // Verificar a necessidade disso
  String closeCommand = "AT+CIPCLOSE=";
  closeCommand += connectionId;

  sendData(closeCommand, 3000, DEBUG);
}

String sendData(String command, const int timeout, boolean debug) {

  String response = "";
  esp8266.print(command + "\r\n");

  int ok = 0;
  long int time = millis();
  while ( (time + timeout) > millis()) {

    while (esp8266.available()) {
      char c = esp8266.read(); // read the next character.
      response += c;
      ok = 1;
    }

    //if (ok == 1) break;

  }

  if (debug) {
    Serial.print(response);
  }

  return response;
}
