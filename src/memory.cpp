#include "../head_file/memory.h"
#include "../head_file/tool.h"

void memory::read() {
    char source[10], tmp[10];
    unsigned int addr = 0;
    while (scanf("%s", source) != EOF) {
        if (source[0] == '@') {
            addr = hex_to_dec(source + 1, 8) / 32;
        } else {
            tmp[6] = source[0];
            tmp[7] = source[1];
            for (int i = 2; i >= 0; --i) {
                scanf("%s", source);
                tmp[i << 1] = source[0];
                tmp[i << 1 | 1] = source[1];
            }
            memory_unit[addr] = hex_to_dec(tmp, 8) / 32;
            ++addr;
        }
    }
}

int memory::load_memory(int addr, int byte_number) {
    if (byte_number == 4) { return memory_unit[addr]; }//直接给出
    int fetch = (1 << (byte_number << 2)) - 1;
    return memory_unit[addr] & fetch;
}