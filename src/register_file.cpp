#include "../head_file/register_file.h"

void register_file::init() {
    for (int i = 0; i < 32; ++i) { depend[i] = -1; }
}

int register_file::get_depend(int reg_) { return depend[reg_]; }

int register_file::get_value(int reg_) {
    if (depend[reg_] != -1) { return register_unit[reg_]; }
    return reg_;
}

void register_file::response_CDB(int tag_, int reg_, int data_) {
    for (int i = 0; i < 32; ++i) {
        if (depend[i] == tag_) {
            if (i == reg_) { register_unit[i] = data_; }
            depend[i] = -1;
        }
    }
}