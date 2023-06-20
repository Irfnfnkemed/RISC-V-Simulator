#include "../head_file/memory.h"
#include "../head_file/tool.h"

void memory::read() {
    char source[10];
    unsigned int addr = 0;
    while (scanf("%s", source) != EOF) {
        if (source[0] == '@') {
            addr = hex_to_dec(source + 1, 8) / 32;
        } else {
            memory_unit[addr][6] = source[0];
            memory_unit[addr][7] = source[1];
            for (int i = 2; i >= 0; --i) {
                scanf("%s", source);
                memory_unit[addr][i << 1] = source[0];
                memory_unit[addr][i << 1 | 1] = source[1];
            }
            ++addr;
        }
    }
}

void memory::load_memory(char target[], int addr, int byte_number) {
    hex_to_bin(target, memory_unit[addr], byte_number);
}