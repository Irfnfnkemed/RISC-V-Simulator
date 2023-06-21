#include "../head_file/tool.h"

int hex_to_dec(const char source[], int digit) {
    int dec_number = 0;
    for (int i = 0; i < digit; ++i) {
        dec_number *= 16;
        if (source[i] >= 'A' && source[i] <= 'F') { dec_number += source[i] - 'A' + 10; }
        else { dec_number += source[i] - '0'; }
    }
    return dec_number;
}

void dec_to_bin(bool target[], int source, int digit) {
    int now = 0;
    while (source) {
        target[now++] = source & 1;
        source = source >> 1;
    }
    for (; now < digit; ++now) { target[now] = 0; }
}

int bin_to_dec(const bool source[], int digit) {
    int out = 0;
    for (int i = digit - 1; i >= 0; --i) {
        out = (out << 1) | source[i];
    }
    return out;
}

int sign_extend(int source, int digit) {
    if (source >> (digit - 1)) { source = source & (-1); }
    return source;
}
