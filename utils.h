/*
 * utils.h
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef UTILS_H_
#define UTILS_H_

/***
 * Convert from integer value to byte array as a string
 */
void convertIntToBytes(int value, char *charValue, int numbersOfBytes);
void convertIntToBytesIgnoreNegative(int value, char *charValue, int numbersOfBytes);

/***
 * Convert from float value to byte array as a string
 */
void convertFloatToBytes(float value, char *charValue, int numbersOfBytes);

/***
 * Convert from byte array to int value
 */
int convertBytesToInt(char *value);

/***
 * Convert from byte array to float value
 */
float convertBytesToFloat(char *value);

/***
 * Compare if values are equals ignore void value
 */
int compareBytes(char *value1, char *value2, int size);

void subvectorBytes(char *vector, int starts, int end, char *result);

void concatString(char *str1, char *str2, char *result);

#endif /* UTILS_H_ */
