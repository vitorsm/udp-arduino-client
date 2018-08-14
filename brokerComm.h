/*
 * brokerComm.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef BROKERCOMM_H_
#define BROKERCOMM_H_

typedef int (sendDataFunc)(char *command, const int timeout, int debug);

void startAccessPoint(sendDataFunc *sendData);
void stopAccessPoint(sendDataFunc *sendData);

void startPageServer(sendDataFunc *sendData);

void proccessRequest(sendDataFunc *sendData);
void proccessPost(sendDataFunc *sendData);

void proccessWifiConfig(sendDataFunc *sendData);
void proccessCredentialsConfig(sendDataFunc *sendData);

#endif /* BROKERCOMM_H_ */
