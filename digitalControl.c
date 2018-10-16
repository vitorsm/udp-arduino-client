/*
 * digitalControl.c
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#include "digitalControl.h"


void initDigitalControl() {

  for (int i = 0; i < PORTS_AMOUNT; i++) {
    lastTimeChange[i] = 0;
    setPoint[i] = 0;
    iIncrement[i] = 0;
    lastInput[i] = 0;
    kp[i] = 0;
    ki[i] = 0;
    kd[i] = 0;
    typeIO[0] = -1;
    sampleTime[i] = 0;
    conditions[i] = NULL;
    inputIndexControl[i] = -1;
    lastTimeSendInputs[0];
  }

  for (int i = 0; i < MAX_AMOUNT_INPUT; i++) {
    inputs[i] = NULL;
  }
}

void setTunnings(float _kp, float _ki, float _kd, int portNumber) {
  // analisar se é preciso dividir os k pelo sampleTime
  kp[portNumber] = _kp;
  ki[portNumber] = _ki;
  kd[portNumber] = _kd;
}

void updateInputValue(int id, float value) {

  int inputIndex = getIndexInputById(id);
  struct Input *input = inputs[inputIndex];
  input->value = value;
  
}

void digitalControl(float *ports) {

  for (int i = 0; i < PORTS_AMOUNT; i++) {

    // verifica se a porta é de saída binária ou PID
    if (typeIO[i] == 1 || typeIO[i] == 2) {
      // verifica se ta ligado
      if (computeCondition(i) == 1) {
        float output = 0;
        // verifica se a porta é de saída binária
        if (typeIO[i] == 2)
          output = 1;
        else
          output = computePID(i);
        
        if (output >= 0)
          ports[i] = output;
          
      } else {
        ports[i] = 0;
      }
    }
  }
}

float computePID(int portNumber) {

  unsigned long timeNow = millis();
  
  long timeChange = timeNow - lastTimeChange[portNumber];
  int inputIndex = inputIndexControl[portNumber];

  if (timeChange < sampleTime[portNumber] || inputIndex < 0) {
    return -1;
  }
  

  float error = setPoint[portNumber] - inputs[inputIndex]->value;

  iIncrement[portNumber] += (ki[portNumber] * error);
//  if (iIncrement[portNumber] > outMax[portNumber]) iIncrement[portNumber] = outMax[portNumber];
//  else if (iIncrement[portNumber] < outMin[portNumber]) iIncrement[portNumber] = outMin[portNumber];

  float deltaInput = inputs[inputIndex]->value - lastInput[portNumber];

  float output = kp[portNumber] * error + iIncrement[portNumber] - kd[portNumber] * deltaInput;
//  if (output > outMax[portNumber]) output = outMax[portNumber];
//  else if (output < outMin[portNumber]) output = outMin[portNumber];

  lastInput[portNumber] = inputs[inputIndex]->value;
  lastTimeChange[portNumber] = timeNow;

  return output;
  
}

int computeCondition(int portNumber) {
  
  struct Condition *condition = conditions[portNumber];

  if (condition == NULL) 
    return 1;
    
  int indexInput = getIndexInputById(condition->inputId);

  if (indexInput < 0)
    return 0;

  struct Input *input = inputs[indexInput];

  int response = verifyCondition(condition->operation, condition->value, input->value);

  while (response == 1 && condition->next != NULL) {
    
    condition = condition->next;
    
    indexInput = getIndexInputById(condition->inputId);
    if (indexInput < 0)
      return 0;

    input = inputs[indexInput];
    response = verifyCondition(condition->operation, condition->value, input->value);
  }

  return response;
}

int getIndexInputById(int id) {

  for (int i = 0; i < MAX_AMOUNT_INPUT; i++) {
    if (inputs[i] != NULL) {
      if (inputs[i]->id == id)
        return i;
    }
  }

  return -1;
}

int verifyCondition(char operation, float conditionValue, float inputValue) {

  switch(operation) {

    case 1: // ==
      if (conditionValue == inputValue)
        return 1;
      else
        return 0;
    case 2: // >
      if (conditionValue > inputValue)
        return 1;
      else
        return 0;
    case 3: // <
      if (conditionValue < inputValue)
        return 1;
      else
        return 0;
    case 4: // >=
      if (conditionValue >= inputValue)
        return 1;
      else
        return 0;
    case 5: // <=
      if (conditionValue <= inputValue)
        return 1;
      else
        return 0;
    case 6: // !=
      if (conditionValue != inputValue)
        return 1;
      else
        return 0;
    default:
      break;
      
  }
}

int sendInputValue(sendDataFunc *sendData, int inputId, float value) {

  char topic[5];
  char strValue[5];

  convertIntToBytes(inputId, topic, 5);
  convertFloatToBytes(value, strValue, 5);
    
///  buildMessagePublish(topic, char *token, float value, int isIntValue, char *message)
  
}
