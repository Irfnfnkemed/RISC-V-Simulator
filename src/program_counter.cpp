#include "program_counter.h"

void program_counter::init() {
    pc = 0;
    stop = false;
}

void program_counter::set_offset(int offset_, bool begin_) {
    offset = offset_;
    begin = begin_;
}

bool program_counter::is_stop(){ return stop;}

int program_counter::get_pc() { return pc; }

void program_counter::set_stop(bool stop_) { stop = stop_; }

void program_counter::flush() {
    if (!stop) {
        if (begin) { pc = offset; }
        else { pc += offset; }
        offset = 4;
        begin = false;
    }
}
