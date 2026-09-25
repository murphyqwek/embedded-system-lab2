#include "calc.h"
#include "fonts.h"
#include "oled.h"
#include "kb.h"
#include "usart.h"
#include "utils.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>


int state = 0;
int op1 = 0;
int op2 = 0;
char str[12];

char operation = '+';

void cycle(void)
{
    switch (state) {
        case 0:
        case 1:
            enterNumber(&state);
            break;

        case 2:
            getOperation();
            break;

        case 3:
            calculation();
            break;

        case 4:
            clearWhenPressed();
            break;
    }
}

void clearWhenPressed() {
	int button = getPressedButton();

	if(button >= 0) {
		state = 0;
		op1 = 0;
		op2 = 0;
		operation = '+';
		oled_Fill(Black);
		printOperand(1, op1);
	}
}

void calculation(void)
{
    int64_t result = 0;

    switch (operation) {
        case '+':
            result = (int64_t)op1 + op2;
            break;

        case '-':
            result = (int64_t)op1 - op2;
            break;

        case '*':
            result = (int64_t)op1 * op2;
            break;

        case '/':
            if (op2 == 0) {
                printError("Division by zero");
                state = 4;
                return;
            }

            result = (int64_t)op1 / op2;
            break;

        default:
            printError("Invalid operation");
            state = 4;
            return;
    }

    if (result < INT_MIN || result > INT_MAX) {
        printError("Integer overflow");
    } else {
        printResult((int)result);
    }

    state = 4;
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
			state = 3;
			return;
			break;
	}

	if(button != -1) {
		printOperationText(operation);
	}
}


void enterNumber(int* state) {
	int button = getPressedButton();

	int* num = *state == 0 ? &op1 : &op2;

	if (button >= 0 && button <= 9) {
	    if (*num > (INT_MAX - button) / 10) {
	        *num = 0;
	    } else {
	        *num = *num * 10 + button;
	    }
	}

	if(button == 10) {
		*num = *num / 10;
	}

	if(button <= 10 && button != -1) {
		redrawNumber(*num);

		return;
	}

	if (button == 11) {
		(*state)++;

		if(*state == 1) {
			printOperand(2, 0);
		}

		if(*state == 2) {
			printOperation(operation);
		}


		return;
	}
}
