#include <kb.h>
#include "fonts.h"
#include "oled.h"
#include "sdk_uart.h"
#include <stdio.h>

int prevKey, key = -1;

int getKey() {
    uint8_t key_number, temp_value, Row[4] = {ROW1, ROW2, ROW3, ROW4};
    for (int i = 0; i < 4; i++) {
        temp_value = Check_Row(Row[i]);
        switch (temp_value) {
            case 0x04: return i*3+1;
            case 0x02: return i*3+2;
            case 0x01: return i*3+3;
            default: break;
        }
    }
    return -1;
}

void printOperation(char operation)
{
    oled_Fill(Black);
    oled_SetCursor(0, 0);
    oled_WriteString("Enter operation: ", Font_7x10, White);

    printOperationText(operation);
}

void printOperationText(char operation)
{
    for (int y = 15; y < 25; y++) {
        for (int x = 0; x < OLED_WIDTH; x++) {
            oled_DrawPixel(x, y, Black);
        }
    }

    oled_SetCursor(60, 15);
    oled_WriteChar(operation, Font_7x10, White);
    oled_UpdateScreen();
}

void printOperand(int currentOperand, int value)
{
    oled_SetCursor(0, 0);
    oled_WriteString(
        currentOperand == 1 ? "Enter operand 1: " : "Enter operand 2: ",
        Font_7x10,
        White
    );

    redrawNumber(value);
}

void redrawNumber(int value)
{
    for (int y = 15; y < 25; y++) {
        for (int x = 0; x < OLED_WIDTH; x++) {
            oled_DrawPixel(x, y, Black);
        }
    }

    char buffer[12];
    int length = snprintf(buffer, sizeof(buffer), "%d", value);

    oled_SetCursor(OLED_WIDTH - 1 - length * 7, 15);
    oled_WriteString(buffer, Font_7x10, White);

    oled_UpdateScreen();
}

void printError(char *message)
{
    oled_Fill(Black);

    oled_SetCursor(0, 0);
    oled_WriteString("Error:", Font_7x10, White);

    oled_SetCursor(0, 15);
    oled_WriteString(message, Font_7x10, White);

    oled_SetCursor(0, 45);
    oled_WriteString("Press any button..", Font_7x10, White);

    oled_UpdateScreen();
}

void printResult(int result) {
	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_WriteString("Result: ", Font_7x10, White);

	oled_SetCursor(0, 45);
	oled_WriteString("Press any button..", Font_7x10, White);

	redrawNumber(result);
}

void UART_PrintInt(int num) {
	char buffer[32];

	snprintf(buffer, sizeof(buffer), "Value: %d\r\n", num);
	UART_Transmit((uint8_t *)buffer);
}

int getPressedButton() {
    prevKey = key;
    key = getKey();

    if (key != prevKey) {
        if (key == 11) {
            return 0;
        } 

        if (key == 12) {
            return 11;
        }

        return key;
    }

    return -1;
}
