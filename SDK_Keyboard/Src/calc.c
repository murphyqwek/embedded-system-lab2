#include "calc.h"
#include "fonts.h"
#include "oled.h"
#include "kb.h"
#include "usart.h"


int state = 0;
int op1 = 0;
int op2 = 0;

char operation = '+';

void cycle(void) {
	if(state == 0 || state == 1) {
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

	if(button != 0) {
		//очищаем экран и принтуем ввод первого числа

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

	(*state)++;
}

