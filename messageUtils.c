/*
 * messageUtils.c
 *
 *  Created on: 28 de jul de 2018
 *      Author: vitor
 */

#include "messageUtils.h"

void buildHelloMassage(char *id, char *password, char *message) {

	for (int i = 0; i < MESSAGE_LENGTH; i++) {
		message[i] = EMPTY_CHAR;
	}

	//build header
	message[0] = '0' + MESSAGE_TYPE_HELLO;

	//build body
	for (int i = 0; i < CLIENT_ID_LENGTH; i++) {
		if (id[i] == 0) break;

		message[i + MESSAGE_HEADER_LENGTH] = id[i];
	}

	for (int i = 0; i < CLIENT_PASSWORD_LENGTH; i++) {
		if (password[i] == 0) break;

		message[i + MESSAGE_HEADER_LENGTH + CLIENT_ID_LENGTH] = password[i];
	}

}

void buildKeepAliveMessage(char *token, char *message) {

	for (int i = 0; i < MESSAGE_LENGTH; i++) {
		message[i] = EMPTY_CHAR;
	}

	//build header
	message[0] = '0' + MESSAGE_TYPE_PUBLISH;

	for (int i = 0; i < MESSAGE_TOKEN_LENGTH; i++) {
		if (token[i] == 0) break;

		message[i + 1] = token[i];
	}

}

void buildMessagePublish(char *topic, char *token, float value, int isIntValue, char *message) {

	for (int i = 0; i < MESSAGE_LENGTH; i++) {
		message[i] = EMPTY_CHAR;
	}

	//build header
	message[0] = '0' + MESSAGE_TYPE_PUBLISH;

	for (int i = 0; i < MESSAGE_TOKEN_LENGTH; i++) {
		if (token[i] == 0) break;

		message[i + 1] = token[i];
	}

	//build body
	for (int i = 0; i < MESSAGE_TOPIC_LENGTH; i++) {
		if (topic[i] == 0) break;

		message[i + MESSAGE_HEADER_LENGTH] = topic[i];
	}

	char contentValue[MESSAGE_BODY_CONTENT_LENGTH];
	if (isIntValue == 0) {
		convertFloatToBytes(value, contentValue, MESSAGE_BODY_CONTENT_LENGTH);
	} else {
		convertIntToBytes(value, contentValue, MESSAGE_BODY_CONTENT_LENGTH);
	}


	for (int i = 0; i < MESSAGE_BODY_CONTENT_LENGTH; i++) {
		if (contentValue[i] == 0) break;

		message[i + MESSAGE_HEADER_LENGTH + MESSAGE_TOPIC_LENGTH] = contentValue[i];
	}

}


int getMessageType(char *message) {

	return message[0] - '0';

}

void proccessDataMessage(char *message, char *topic, float *value, char *strValue) {

	char receivedToken[MESSAGE_TOKEN_LENGTH];
	char receivedTopic[MESSAGE_TOPIC_LENGTH];
	char receivedValue[MESSAGE_BODY_CONTENT_LENGTH];

	subvectorBytes(message, 1, 1 + MESSAGE_TOKEN_LENGTH, receivedToken);
	subvectorBytes(message, MESSAGE_HEADER_LENGTH, MESSAGE_HEADER_LENGTH + MESSAGE_TOPIC_LENGTH, receivedTopic);
	subvectorBytes(message, MESSAGE_HEADER_LENGTH + MESSAGE_TOPIC_LENGTH, MESSAGE_LENGTH, receivedValue);

	for (int i = 0; i < MESSAGE_TOPIC_LENGTH; i++) {
		if (receivedTopic[i] == EMPTY_CHAR) {
			topic[i] = 0;
		} else {
			topic[i] = receivedTopic[i];
		}
	}

	//remove empty char
	removeEmptyChar(receivedToken, MESSAGE_TOKEN_LENGTH);
	removeEmptyChar(receivedValue, MESSAGE_BODY_CONTENT_LENGTH);

  concatString("", receivedValue, strValue);
	float floatValue = convertBytesToFloat(receivedValue);
	*value = floatValue;  
}

void proccessRuleMessage(char *token, struct Condition *conditions, int *inputType, unsigned long *sampleTime, float *kp, float *ki, float *kd, char *message) {

// msg: tipo de mensagem, token do modulo, listaInputTypes, listaKps, listaKis, listaKds, listaSampleTimes, listaConditions(separadas por EMPTY_CHAR e cada condition por &)

  char receivedToken[MESSAGE_TOKEN_LENGTH];
  subvectorBytes(message, 1, 1 + MESSAGE_TOKEN_LENGTH, receivedToken);
  
  // definir quantidade de pinos
  // definir tamanho maximo dos ks
  // definir tamanho maxiom dos samplePoints
  
}

void addEmptyChar(char *str, int size) {
	for (int i = 0; i < size; i++) {
		if (str[i] == 0) {
			str[i] = EMPTY_CHAR;
		}
	}
}

void removeEmptyChar(char *str, int size) {
	for (int i = 0; i < size; i++) {
		if (str[i] == EMPTY_CHAR) {
			str[i] = 0;
		}
	}
}
