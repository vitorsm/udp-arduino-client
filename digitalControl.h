/*
 * digitalControl.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#ifndef DIGITALCONTROL_H_
#define DIGITALCONTROL_H_

#include "utils.h"
#include "constants.h"
#include "messageUtils.h"

typedef void (serialPrintFunc)(char *message, int isPrintln);
typedef void (printConstantsMessages)(int messageIndex, int isPrintln);
typedef void (printLCDFunc)(int messageIndex, int keepLastText);
typedef int (sendDataFunc)(char *command, const int timeout, int debug, int maxAttempts);

unsigned long lastTimeChange[PORTS_AMOUNT];
float setPoint[PORTS_AMOUNT];
//float input[PORTS_AMOUNT];
struct Input *inputs[MAX_AMOUNT_INPUT];
int inputIndexControl[PORTS_AMOUNT];
float iIncrement[PORTS_AMOUNT];
float lastInput[PORTS_AMOUNT];
float kp[PORTS_AMOUNT];
float ki[PORTS_AMOUNT];
float kd[PORTS_AMOUNT];
char typeIO[PORTS_AMOUNT]; //byte: 0: input, 1: PID_output, 2: binary_output, 3: pulse_output
unsigned long sampleTime[PORTS_AMOUNT];
struct Condition *conditions[PORTS_AMOUNT];
int pinsId[PORTS_AMOUNT];

unsigned long lastTimeSendInputs[PORTS_AMOUNT];


void initDigitalControl();

void setTunnings(float _kp, float _ki, float _kd, int portNumber);

void updateInputValue(int id, float value);

void digitalControl(float *ports);

float computePID(int portNumber);

int computeCondition(int portNumber);

int getIndexInputById(int id);

int verifyCondition(char operation, float conditionValue, float inputValue);

void setParams(char *message);

void setInput(char *message);

int sendInputValues(sendDataFunc *sendData);

int sendInputValue(sendDataFunc *sendData, int inputId, float value);

#endif /* DIGITALCONTROL_H_ */
