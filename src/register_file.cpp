#include "register_file.h"

void register_file::init() {
    memset(depend, 0xff, 32 * sizeof(int));
    memset(depend_next, 0xff, 32 * sizeof(int));
    memset(register_unit, 0, 32 * sizeof(int));
    memset(register_unit_next, 0, 32 * sizeof(int));
    memset(modify, 0, 32);
}

void register_file::get(u_int8_t reg_, int &depend_, int &value) {
    depend_ = depend[reg_];
    if (depend_ == -1) { value = register_unit[reg_]; }
    else { value = -1; }
}

int register_file::get(u_int8_t reg_) { return register_unit[reg_]; }

void register_file::flush_tag(int tag_, int data_, u_int8_t reg_) {
    if (reg_ > 0) {
        register_unit_next[reg_] = data_;
        if (depend[reg_] == tag_ && !modify[reg_]) { depend_next[reg_] = -1; }
    }
}

void register_file::set_tag(int tag_, u_int8_t reg_) {
    depend_next[reg_] = tag_;
    modify[reg_] = true;
}

int register_file::get_return_value() {
    return unsigned(register_unit[10]) & 255;
}

void register_file::flush() {
    for (int i = 1; i < 32; ++i) {
        register_unit[i] = register_unit_next[i];
        depend[i] = depend_next[i];
    }
    memset(modify, 0, 32);
    register_unit[0] = register_unit_next[0] = 0;
    depend[0] = depend_next[0] = -1;
}

void register_file::clear() {
    register_unit[0] = register_unit_next[0] = 0;
    depend[0] = depend_next[0] = -1;
    memset(modify, 0, 32);
    for (int i = 1; i < 32; ++i) {
        register_unit[i] = register_unit_next[i];
        depend[i] = depend_next[i] = -1;
    }
}
