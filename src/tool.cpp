#include "../head_file/tool.h"

int hex_to_dec(const char source[], int digit) {
    int dec_number = 0;
    for (int i = 0; i < digit; ++i) {
        dec_number = dec_number << 4;
        if (source[i] >= 'A' && source[i] <= 'F') { dec_number += source[i] - 'A' + 10; }
        else { dec_number += source[i] - '0'; }
    }
    return dec_number;
}

int sign_extend(int source, int digit) {
    if (digit < 32 && source >> (digit - 1)) {
        int fetch = -(1 << digit);
        source = source | fetch;
    }
    return source;
}

int fetch(int source, int high, int low) {
    int fetch_num = (1 << (high - low + 1)) - 1;
    return (source >> low) & fetch_num;
}