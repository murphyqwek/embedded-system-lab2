#ifndef UTILS
#define UTILS

int getKey();
int getPressedButton();
void UART_PrintInt(int num);

void printOperand(int currentOperand, int value);
void redrawNumber(int value);

void printOperation(char operation);
void printOperationText(char operation);

void printResult(int num);

void printError(char *message);

#endif
