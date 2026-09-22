#include <kb.h>

int prevKey, key = -1;

int getKey() {
    uint8_t key_number, temp_value, Row[4] = {ROW1, ROW2, ROW3, ROW4};
    for (int i = 0; i < 4; i++) {
        temp_value = Check_Row(Row[i]);
        switch (temp_value) {
            case 0x01: return i*3+1; 
            case 0x02: return i*3+2;
            case 0x04: return i*3+3;
            default: return -1;
        }
    }
}

int getPressedButton() {
    prevKey = key;
    int key = getKey();

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