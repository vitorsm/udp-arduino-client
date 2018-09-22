/*
 * constants.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define DEBUG 1

#define TIME_REQUEST_HELLO 8000

/***
 * Server constants
 */

#define SERVER_PORT 4410
#define SERVER_PORT_SEND 4409

/***
 * Server constants
 */

#define CLIENT_PORT 4411


#define CLIENT_ID_LENGTH 10
#define CLIENT_PASSWORD_LENGTH 10

/***
 * Message configuration constants
 */

#define EMPTY_CHAR 127

#define MESSAGE_TYPE_LENGTH 1
#define MESSAGE_TOKEN_LENGTH 20
//const int MESSAGE_HEADER_LENGTH = MESSAGE_TYPE_LENGTH + MESSAGE_TOKEN_LENGTH;
#define MESSAGE_HEADER_LENGTH 21
#define MESSAGE_BODY_LENGTH 30

#define MESSAGE_ID_LENGTH 10
#define MESSAGE_PASSWORD_LENGTH 10

#define MESSAGE_TOPIC_LENGTH 10
//	const int MESSAGE_BODY_CONTENT_LENGTH = MESSAGE_BODY_LENGTH - MESSAGE_TOPIC_LENGTH;
#define MESSAGE_BODY_CONTENT_LENGTH 20

#define MESSAGE_ERROR_NAME_LENGTH 10
#define MESSAGE_ERROR_DETAILS_LENGTH 20

//	const int MESSAGE_LENGTH = MESSAGE_HEADER_LENGTH + MESSAGE_BODY_LENGTH;
#define MESSAGE_LENGTH 51


#define MAXIMUM_ATTEMPTS 5

#define MESSAGE_TYPE_PUBLISH 1
#define MESSAGE_TYPE_SUBSCRIBE 2
#define MESSAGE_TYPE_KEEP_ALIVE 3
#define MESSAGE_TYPE_UPDATE_PARAM 4
#define MESSAGE_TYPE_DATA 5
#define MESSAGE_TYPE_HELLO 6
#define MESSAGE_TYPE_NETWORKS 7
#define MESSAGE_TYPE_CREDENTIALS_REQUEST 8
#define MESSAGE_TYPE_ERROR 9

/***
 * Configuration message configuration
 */

#define SSID_MAX_LENGTH_NAME 20
#define BSSID_LENGTH 15
#define MAX_NETWORKS 30



/***
 * Digital Control
 */

#define PORTS_AMOUNT 5
#define PORT_1 1
#define PORT_2 2
#define PORT_3 3
#define PORT_4 4
#define PORT_5 5

#define MAX_AMOUNT_INPUT 10

/***
 * Configurações mensagens AT
 * /
 */

#define MAX_SIZE_AT_TYPE_MESSAGE 10

#endif /* CONSTANTS_H_ */
