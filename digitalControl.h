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

unsigned long lastTimeChange[PORTS_AMOUNT];
float setPoint[PORTS_AMOUNT];
//float input[PORTS_AMOUNT];
struct Input *inputs[MAX_AMOUNT_INPUT];
int inputsId[PORTS_AMOUNT];
int pinIndexControl[PORTS_AMOUNT];
float iIncrement[PORTS_AMOUNT];
float lastInput[PORTS_AMOUNT];
float kp[PORTS_AMOUNT];
float ki[PORTS_AMOUNT];
float kd[PORTS_AMOUNT];
char typeIO[PORTS_AMOUNT]; //byte: 1: input, 2: PID_output, 3: binary_output,
int sampleTime[PORTS_AMOUNT];
struct Condition *conditions[PORTS_AMOUNT];
int pinsId[PORTS_AMOUNT];
int outMax;
int outMin;
unsigned long lastTimeSendInputs[PORTS_AMOUNT];


void initDigitalControl();

void setTunnings(float _kp, float _ki, float _kd, int portNumber);

void updateInputValue(int id, float value);

void digitalControl(int *values);

float computePID(int portNumber);

int computeCondition(int portNumber);

int getIndexInputById(int id);

int verifyCondition(char operation, float conditionValue, float inputValue);

void setParams(char *token, char *message);

void setInput(char *message);

int updateInputValues(int *values);

int sendInputValue(int inputId, float value);

#endif /* DIGITALCONTROL_H_ */
