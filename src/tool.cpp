#include "../head_file/tool.h"

unsigned int hex_to_dec(const char source[], int digit) {
    unsigned int dec_number = 0;
    for (int i = 0; i < digit; ++i) {
        dec_number *= 16;
        if (source[i] >= 'A' && source[i] <= 'F') { dec_number += source[i] - 'A' + 10; }
        else { dec_number += source[i] - '0'; }
    }
    return dec_number;
}

void hex_to_bin(char target[], const char source[], int digit) {
    int tmp;
    for (int i = 0; i < digit; ++i) {
        if (source[i] >= 'A' && source[i] <= 'F') { tmp = source[i] - 'A' + 10; }
        else { tmp = source[i] - '0'; }
        for (int j = 3; j >= 0; --j) {
            target[(i << 2) + j] = tmp & 1;
            tmp = (tmp >> 1);
        }
    }
}