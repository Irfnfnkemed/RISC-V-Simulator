#include "memory.h"

void memory::init() {
    for (int i = 0; i < 1000000; ++i) { memory_unit[i] = 0; }
    char source[10], tmp[10];
    unsigned int addr = 0;
    while (scanf("%s", source) != EOF) {
        if (source[0] == '@') {
            addr = hex_to_dec(source + 1, 8) >> 2;
        } else {
            tmp[6] = source[0];
            tmp[7] = source[1];
            for (int i = 2; i >= 0; --i) {
                scanf("%s", source);
                tmp[i << 1] = source[0];
                tmp[i << 1 | 1] = source[1];
            }
            memory_unit[addr] = hex_to_dec(tmp, 8);
            ++addr;
        }
    }
}

int memory::load_memory(int addr, int byte_number) {
    addr = addr >> 2;
    if (byte_number == 4) { return memory_unit[addr]; }//直接给出
    int fetch = (1 << (byte_number << 3)) - 1;
    return memory_unit[addr] & fetch;
}

void memory::store_memory(int addr, int source, int byte_number) {
    addr = addr >> 2;
    if (byte_number == 4) { memory_unit[addr] = source; }
    else {
        int fetch = (1 << (byte_number << 3)) - 1;
        source = source & fetch;//取source的byte_number个字节
        fetch = -(1 << (byte_number << 3));
        memory_unit[addr] = (memory_unit[addr] & fetch) | source;
    }
}