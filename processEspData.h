/*
 * processEspData.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef PROCESSESPDATA_H_
#define PROCESSESPDATA_H_

#include "utils.h"

void proccessReceivedData(char *data);
void proccessReceivedMessage(String message, String originIp, int originPort);

#endif /* PROCESSESPDATA_H_ */
