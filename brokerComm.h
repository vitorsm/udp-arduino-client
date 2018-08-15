/*
 * brokerComm.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef BROKERCOMM_H_
#define BROKERCOMM_H_

#include "espUtils.h"

const char *ssid = "Nome rede";

void startAccessPointConfig();
void stopAccessPointConfig();

void startPageServer();

void proccessRequest();
void proccessPost();

void proccessWifiConfig();
void proccessCredentialsConfig();

#endif /* BROKERCOMM_H_ */
