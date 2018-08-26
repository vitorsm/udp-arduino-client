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

struct Condition {
  int inputId;
  char operation;
  float value;
  struct Condition *next;
};

struct Input {
  int id;
  float value;
};


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
char typeIO[PORTS_AMOUNT]; //byte: 0: input, 1: output
unsigned long sampleTime[PORTS_AMOUNT];
struct Condition *conditions[PORTS_AMOUNT];

void initDigitalControl();

void setTunnings(float _kp, float _ki, float _kd, int portNumber);

void updateInputValue(int id, float value);

void digitalControl(float *ports);

float computePID(int portNumber);

int computeCondition(int portNumber);

int getIndexInputById(int id);

int verifyCondition(char operation, float conditionValue, float inputValue);


#endif /* DIGITALCONTROL_H_ */
