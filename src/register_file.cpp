#include "register_file.h"

void register_file::init() {
    for (int i = 0; i < 32; ++i) { depend[i] = depend_next[i] = -1; }
}

void register_file::get(int reg_, int &depend_, int &value) {
    depend_ = depend[reg_];
    if (depend_ == -1) { value = register_unit[reg_]; }
    else { value = -1; }
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

int register_file::get_return_value() {
    return unsigned(register_unit[10]) & 255;
}

void register_file::flush() {
    for (int i = 0; i < 32; ++i) {
        register_unit[i] = register_unit_next[i];
        depend[i] = depend_next[i];
    }
}

void register_file::clear() {
    for (int i = 0; i < 32; ++i) { depend[i] = depend_next[i] = -1; }
}