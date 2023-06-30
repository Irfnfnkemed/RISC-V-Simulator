#include "memory.h"

void memory::init() {
    memset(memory_unit, 0, size * sizeof(u_int8_t));
    char source[10];
    unsigned int addr = 0;
    while (scanf("%s", source) != EOF) {
        if (source[0] == '@') {
            addr = hex_to_dec(source + 1, 8);
        } else {
            memory_unit[addr] = hex_to_dec(source, 2);
            ++addr;
        }
    }
}

int memory::load_memory(int addr, int byte_number, bool sign_extended) {
    int out = 0;
    for (int i = byte_number - 1; i >= 0; --i) {
        out = out << 8;
        out = out | int(memory_unit[addr + i]);
    }
    if (sign_extended) { return sign_extend(out, byte_number << 3); }
    else { return out; }
}

void memory::store_memory(int addr, int source, int byte_number) {
    for (int i = 0; i < byte_number; ++i) {
        memory_unit[addr + i] = source & 0b11111111;
        source = source >> 8;
    }
}