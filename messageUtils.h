/*
 * messageUtils.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef MESSAGEUTILS_H_
#define MESSAGEUTILS_H_

#include "utils.h"
#include "constants.h"

void buildHelloMassage(char *id, char *password, char *message);

void buildKeepAliveMessage(char *token, char *message);

void buildMessagePublish(char *topic, char *token, float value, int isIntValue, char *message);

int getMessageType(char *message);

void proccessDataMessage(char *message, char *topic, float *value, char *strValue);

void proccessRuleMessage(char *token, struct Condition *conditions, int *inputType, unsigned long *sampleTime, float *kp, float *ki, float *kd, char *message);

void removeEmptyChar(char *str, int size);

#endif /* MESSAGEUTILS_H_ */
