#include "calc.h"
#include "fonts.h"
#include "oled.h"
#include "kb.h"
#include "usart.h"
#include "utils.h"
#include <stdio.h>


int state = 0;
int op1 = 0;
int op2 = 0;
char str[12];

char operation = '+';

void cycle() {

	oled_Reset();

	if (state == 0) {
		oled_WriteString("Enter operand1: ", Font_7x10, White);

		int length = snprintf(str, sizeof(str), "%d", op1);
		oled_SetCursor(127 - length*7, 15);
		oled_WriteString(str, Font_7x10, White);

		enterNumber(&state);
	}

	if (state == 1) {
		oled_WriteString("Enter operand2: ", Font_7x10, White);

		int length = snprintf(str, sizeof(str), "%d", op2);
		oled_SetCursor(127 - length*7, 15);
		oled_WriteString(str, Font_7x10, White);

		enterNumber(&state);
	}

	if(state == 2) {
		getOperation();
	}

	if(state == 3) {
		calculation();
	}

	if(state == 4) {
		clearWhenPressed();
	}
}

void clearWhenPressed() {
	int button = getPressedButton();

	if(button >= 0) {
		state = 0;
	}
}

void calculation() {
	int result = 0;

	switch(operation) {
		case '+':
			result = op1 + op2;
			break;

		case '-':
			result = op1 - op2;
			break;

		case '*' :
			result = op1 * op2;
			break;

		case '/' :
			//сделать проверку на деление на 0

			result = op1 / op2;
			break;
	}


	printNum(result);


	state++;


}

void printNum(int result) {
	oled_WriteString("Enter result: ", Font_7x10, White);

	int length = snprintf(str, sizeof(str), "%d", result);

	oled_SetCursor(127 - length*7, 15);
	oled_WriteString(str, Font_7x10, White);
}

void getOperation() {
	int button = getPressedButton();

	switch(button) {
		case 1:
			operation = '+';
			break;
		case 2:
			operation = '-';
			break;
		case 3:
			operation = '*';
			break;

		case 4:
			operation = '/';
			break;

		case 11:
			state++;
			return;
			break;
	}

	printOperation(operation);
}

void printOperation(char operation) {
	oled_WriteString("Enter operation: ", Font_7x10, White);

	oled_SetCursor(60, 15);
	oled_WriteChar(operation, Font_7x10, White);
}


void enterNumber(int* state) {
	int button = getPressedButton();

	if(0 <= button && button <= 9) {
		int* num = *state == 0 ? &op1 : &op2;

		*num = *num * 10 + button;

		return;
	}

	if(button == 10) {
		int* num = *state == 0 ? &op1 : &op2;

		*num = *num / 10;

		return;
	}

	if (button == 11) {
		(*state)++;
		return;
	}
}