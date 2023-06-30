#include "program_counter.h"

void program_counter::init() {
    pc = 0;
    stop = stop_next = clean = false;
}

void program_counter::set_offset(int offset_, bool begin_, bool clean_) {
    if (!clean) {
        offset = offset_;
        begin = begin_;
        clean = clean_;
    }
}

bool program_counter::is_stop() { return stop; }

void program_counter::set_freeze() { freeze = true; }

int program_counter::get_pc() { return pc; }

void program_counter::set_stop(bool stop_) { stop_next = stop_; }

void program_counter::flush() {
    if (freeze) { freeze = false; }
    else {
        stop = stop_next;
        if (!stop) {
            if (begin) { pc = offset; }
            else { pc += offset; }
        }
        offset = 4;
        begin = false;
    }
}

void program_counter::clear() {
    stop = stop_next = clean = false;
    if (begin) { pc = offset; }
    else { pc += offset; }
    offset = 4;
    begin = freeze = false;
}
