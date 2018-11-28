/*
 * digitalControl.c
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#include "digitalControl.h"


void initDigitalControl() {
//  int firstPin = 14;

  outMax = 1023;
  outMin = 0;
  
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
    
    conditions[i] = malloc(sizeof(struct  Condition));
    conditions[i]->inputId = -1;

    pinsId[i] = -1;
    
    inputsId[i] = -1;
//    pinIndexControl[i] = firstPin + i;
    lastTimeSendInputs[0];
  }

  for (int i = 0; i < MAX_AMOUNT_INPUT; i++) {
    inputs[i] = malloc(sizeof(struct  Input));
    inputs[i]->id = -1;
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

void digitalControl(int *values) {

  for (int i = 0; i < PORTS_AMOUNT; i++) {

    // verifica se a porta é de saída binária ou PID
    if (typeIO[i] == '2' || typeIO[i] == '3') {
      // verifica se ta ligado
      if (computeCondition(i) == 1) {
        int output = 0;
        // verifica se a porta é de saída binária
        if (typeIO[i] == '3')
          output = 1;
        else
          output = computePID(i);
        
        if (output >= 0)
          values[i] = (int) output;
          
      } else {
        values[i] = 0;
      }
    }
  }
}

float computePID(int portNumber) {

  unsigned long timeNow = millis();
  
  long timeChange = timeNow - lastTimeChange[portNumber];
  int inputId = inputsId[portNumber];

  int inputIndex = getIndexInputById(inputId);
  
  if (timeChange < sampleTime[portNumber] || inputIndex < 0 || inputId < 0) {
    return -1;
  }
  
  float error = setPoint[portNumber] - inputs[inputIndex]->value;

  char strError[] = {0, 0, 0, 0, 0, 0};
  convertFloatToBytes(error, strError, 5);
  serialPrint("erro: ", 0);
  serialPrint(strError, 1);
  
  iIncrement[portNumber] += (ki[portNumber] * error);
  if (iIncrement[portNumber] > outMax) iIncrement[portNumber] = outMax;
  else if (iIncrement[portNumber] < outMin) iIncrement[portNumber] = outMin;

  char strIIncrement[] = {0, 0, 0, 0, 0, 0};
  convertFloatToBytes(iIncrement[portNumber], strIIncrement, 5);
  serialPrint("deltaI: ", 0);
  serialPrint(strIIncrement, 1);
  
  float deltaInput = inputs[inputIndex]->value - lastInput[portNumber];

  float output = kp[portNumber] * error + iIncrement[portNumber] - kd[portNumber] * deltaInput;
  if (output > outMax) output = outMax;
  else if (output < outMin) output = outMin;
  
  lastInput[portNumber] = inputs[inputIndex]->value;
  lastTimeChange[portNumber] = timeNow;
  
  return output;
}

int computeCondition(int portNumber) {
  
  struct Condition *condition = conditions[portNumber];

  if (condition->inputId < 0) 
    return 1;
    
  int indexInput = getIndexInputById(condition->inputId);

  if (indexInput < 0)
    return 0;

  struct Input *input = inputs[indexInput];

  int response = verifyCondition(condition->operation, condition->value, input->value);

  while (response == 1 && condition->next->inputId >= 0) {
    
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
//    if (inputs[i] != NULL) {
    if (inputs[i]->id > 0) {
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

int updateInputValues(int *values) {

  unsigned long timeNow = millis();
  
  for (int i = 0; i < PORTS_AMOUNT; i++) {
    // verificando se o pino é de entrada
    if (typeIO[i] == '1') {
      long timeChange = timeNow - lastTimeChange[i];

      int indexInput = getIndexInputById(pinsId[i]);
      inputs[indexInput]->value = values[i];
      
      // verifica se ja passou o tempo para enviar
      if (timeChange >= sampleTime[i]) {
        sendInputValue(inputs[i]->id, inputs[i]->value);
        
        lastTimeChange[i] = timeNow;
      }
    }
  }
  
}

int sendInputValue(int inputId, float value) {

  char topic[MESSAGE_TOPIC_LENGTH + 1];
  convertIntToBytes(inputId, topic, MESSAGE_TOPIC_LENGTH);
  topic[MESSAGE_TOPIC_LENGTH] = 0;

  sendPublishMessage(value, topic);  
}

void setParams(char *token, char *message) {

  clearString(token, MESSAGE_TOKEN_LENGTH);
  proccessRuleMessage(token, conditions, pinsId, typeIO, sampleTime, kp, ki, kd, setPoint, inputsId, message);
  token[MESSAGE_TOKEN_LENGTH] = 0;

  for (int i = 0; i < MAX_AMOUNT_INPUT; i++) {
    inputs[i]->id = -1;
  }

  int countInputs = 0;
  int types[PORTS_AMOUNT];

  int countInput = 14;
  int countOutput = 10;
  
  for (int i = 0; i < PORTS_AMOUNT; i++) {
    if (typeIO[i] == '2') {
      types[i] = 1;
      inputs[countInputs]->id = inputsId[i];
      inputs[countInputs]->value = 0;
      inputs[countInputs]->localPin = 0;
      pinIndexControl[i] = countOutput;
      countInputs++;
      countOutput++;
    } else if (typeIO[i] == '1') {
      types[i] = 0;
      inputs[countInputs]->id = pinsId[i];
      inputs[countInputs]->value = 0;
      inputs[countInputs]->localPin = 1;
      pinIndexControl[i] = countInput;
      countInputs++;
      countInput++;
    } else {
      types[i] = 1;
      pinIndexControl[i] = countOutput;
      countOutput++;
    }
  }

  for (int i = 0; i < PORTS_AMOUNT; i++) {
    struct Condition *c = conditions[i];
    while (c->inputId >= 0) {
      int existeInputId = getIndexInputById(c->inputId);
      // tem q verificar porq se for uma entrada interna ela ja pode ter sido acrescentada
      if (existeInputId < 0) {
        inputs[countInputs]->id = c->inputId;
        inputs[countInputs]->value = 0;
        countInputs++;
      }
      c = c->next;
    }
  }

  
  updateTypeIO(types);
}


