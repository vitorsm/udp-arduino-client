/*
 * utils.c
 *
 *  Created on: 25 de jul de 2018
 *      Author: vitor
 */

#include "utils.h"

int countDigitNumbers(int value) {
	int count = 1;
	while (value >= 10) {
		value = value / 10;
		count++;
	}

	return count;
}

int getDividerNumber(int value) {
	int divider = 1;
	while (value >= 10) {
		value = value / 10;
		divider = divider * 10;
	}

	return divider;
}

void convertIntToBytesIgnoreNegative(int value, char *charValue, int numbersOfBytes) {
	if (value <= 0) {
		value = -value;
	}

  clearString(charValue, numbersOfBytes);
  
	int count = countDigitNumbers(value);
	int divider = getDividerNumber(value);

	int digitNumbers = count;
	for (int i = 0; i < count && i < numbersOfBytes; i++) {
		int digit = value / divider;
		charValue[i] = '0' + digit;
		value = value - digit * divider;
		divider = divider / 10;

		int newCount = countDigitNumbers(value);
		int difference = digitNumbers - newCount;
		while (difference > 1 && i + 1 < count) {
			i++;
			charValue[i] = '0';
			difference--;
			divider = divider / 10;
		}

		digitNumbers = newCount;
	}

}

void convertIntToBytes(int value, char *charValue, int numbersOfBytes) {

	int isNegative = 0;
	if (value <= 0) {
		isNegative = 1;
		value = -value;
	}
//
//	for (int i = 0; i < numbersOfBytes; i++) {
//		charValue[i] = 0;
//	}
  clearString(charValue, numbersOfBytes);

	int count = countDigitNumbers(value);
	int divider = getDividerNumber(value);

	int digitNumbers = count;
	for (int i = 0; i < count && i < numbersOfBytes - isNegative; i++) {
		int digit = value / divider;
		charValue[i + isNegative] = '0' + digit;
		value = value - digit * divider;
		divider = divider / 10;

		int newCount = countDigitNumbers(value);
		int difference = digitNumbers - newCount;
		while (difference > 1 && i + 1 < count) {
			i++;
			charValue[i] = '0';
			difference--;
			divider = divider / 10;
		}

		digitNumbers = newCount;
	}

	if (isNegative == 1) {
		charValue[0] = '-';
	}
}

int convertBytesToInt(char *value) {

	int isNegative = 0;
	int count = strlen(value);

	if (count > 0 && value[0] == '-') {
		isNegative = 1;
	}

	int divider = 1;
	// trocar isso pela funcao pow
	for (int i = isNegative; i < count - 1; i++) {
		divider = divider * 10;
	}
	//int divider = (int) pow(10.0, (double) count);

	int valueReturn = 0;

	for (int i = isNegative; i < count; i++) {
		valueReturn += divider * (value[i] - '0');
		divider = divider / 10;
	}

	if (isNegative) {
		valueReturn = -1 * valueReturn;
	}

	return valueReturn;
}

void getIntPartFromFloat(char *value, char *intPart, char *decimalPart) {

	int count = strlen(value);
	int isIntPart = 1;
	int countResult = 0;

	// inicializar com 0
	for (int i = 0; i < count; i++) {
		intPart[i] = 0;
		decimalPart[i] = 0;
	}

	for (int i = 0; i < count; i++) {
		if (value[i] == '.' || value[i] == ',') {
			isIntPart = 0;
			countResult = 0;
		} else {
			if (isIntPart) {
				intPart[countResult] = value[i];
			} else {
				decimalPart[countResult] = value[i];
			}
			countResult++;
		}
	}
}

float convertBytesToFloat(char *value) {

	int isNegative = 0;
	int count = strlen(value);

	if (count > 0 && value[0] == '-') {
		isNegative = 1;
	}

	char intPart[count];
	char decimalPart[count];

	getIntPartFromFloat(value, intPart, decimalPart);

	int countDecimalPart = strlen(decimalPart);
	double mult = 1;
	// trocar isso pela funcao pow
	for (int i = 0; i < countDecimalPart; i++) {
		mult = mult / 10;
	}

	double result = convertBytesToInt(intPart);
	double decimal = convertBytesToInt(decimalPart) * mult;
	if (result < 0) {
		decimal = decimal * -1;
	}

	result = result + decimal;

	return result;
}

int getDecimalPartFromFloat(float value, float precision) {

	int mult = 10;
	int newValue = 0;
	float difference = 0;
	value = value - ((int) value);

	do {

		newValue = mult * value;
		difference = value - newValue / (float) mult;
		mult = mult * 10;

	} while(difference >= precision);

	return newValue;
}

void convertFloatToBytes(float value, char *charValue, int numbersOfBytes) {
	int countDecimalPlaces = 5;

	int isNegative  = 0;
	if (value < 0) {
		isNegative = 1;
		value = -value;
	}

	int intIntPart = (int) value;
	int intDecimalPartValue = getDecimalPartFromFloat(value, 0.00000000001);

	int countIntPart = countDigitNumbers(intIntPart);
	int countDecimalPart = countDigitNumbers(intDecimalPartValue);

	char intPart[countIntPart];
	char tempDecimalPart[countDecimalPart];

	convertIntToBytesIgnoreNegative(intIntPart, intPart, countIntPart);
	convertIntToBytesIgnoreNegative(intDecimalPartValue, tempDecimalPart, countDecimalPart);

	char decimalPart[countDecimalPlaces];
	int decimalPartSize = strlen(tempDecimalPart);
	for (int i = 0; i < countDecimalPlaces; i++) {
		if (i < decimalPartSize) {
			decimalPart[i] = tempDecimalPart[i];
		} else {
			decimalPart[i] = '0';
		}
	}

	for (int i = 0; i < numbersOfBytes - isNegative; i++) {

		if (i < countIntPart) {
			charValue[i + isNegative] = intPart[i];
		} else if (i == countIntPart) {
			charValue[i + isNegative] = '.';
		} else if (i - countIntPart <= countDecimalPlaces) {
			charValue[i + isNegative] = decimalPart[i - countIntPart - 1];
		} else {
			charValue[i + isNegative] = 0;
		}

	}

	if (isNegative == 1) {
		charValue[0] = '-';
	}
	printf("valor: %s", charValue);
}

int compareBytes(char *value1, char *value2, int size) {
  
	for (int i = 0; i < size; i++) {
		if (value1[i] != value2[i]) return 0;
	}

	return 1;
}

void subvectorBytes(char *vector, int starts, int end, char *result) {

	for (int i = 0; i < end - starts; i++) {
		result[i] = 0;
	}

	for (int i = starts; i < end; i++) {
		result[i - starts] = vector[i];
	}
}

void concatString(char *str1, char *str2, char *result) {

	int size1 = strlen(str1);
	int size2 = strlen(str2);

	for (int i = 0; i < size1; i++) {
		result[i] = str1[i];
	}

	for (int i = 0; i < size2; i++) {
		result[i + size1] = str2[i];
	}

  result[size1 + size2] = 0;
}

int getMin(float n1, float n2) {

    if (n1 < n2)
      return n1;
    else
      return n2;
}

void clearString(char *str, int size) {
  for (int i = 0; i < size; i++) {
    str[i] = 0;
  }
}
