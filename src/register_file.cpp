#include "../head_file/register_file.h"

void register_file::init() {
    for (int i = 0; i < 32; ++i) { depend[i] = depend_next[i] = -1; }
}

int register_file::get_depend(int reg_) { return depend[reg_]; }

int register_file::get_value(int reg_) {
    if (depend[reg_] != -1) { return register_unit[reg_]; }
    return reg_;
}

void register_file::flush_tag(int tag_, int data_) {
    for (int i = 0; i < 32; ++i) {
        if (depend[i] == tag_) {
            register_unit_next[i] = data_;
            depend_next[i] = -1;
        }
    }
}

void register_file::set_tag(int tag_, int reg_) {
    depend_next[reg_] = tag_;
}

void register_file::flush() {
    for (int i = 0; i < 32; ++i) {
        register_unit[i] = register_unit_next[i];
        depend[i] = depend_next[i];
    }
}